#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

/* BFS to count reachable collectibles. Returns 1 of all collectibles are
 * reachable from player, 0 otherwise */
int	map_check_collectibles(t_map *map, t_point player, int total_collect)
{
	t_queue	q;
	char	**map_copy;
	int		found;

	map_copy = map_duplicate(map);
	if (!map_copy)
		return (MEM_ALLOC_ERR_CODE);
	if (!queue_init(&q, map->width * map->height))
	{
		map_free_copy(map_copy);
		return (MEM_ALLOC_ERR_CODE);
	}
	found = 0;
	q.data[q.tail++] = player;
	map_copy[player.y][player.x] = VISITED;
	while (q.head < q.tail)
	{
		t_point	cur = q.data[q.head++];
		int		cx = cur.x;
		int		cy = cur.y;
		if (cx < 0 || cy < 0 || cx >= (int)map->width || cy >= (int)map->height)
			continue ;
		if (cx + 1 < (int)map->width)
		{
			char c = map_copy[cy][cx + 1];
			if (c != MAP_WALL_SYMBOL && c != VISITED)
			{
				if (c == MAP_COLLECT_SYMBOL)
					++found;
				if (q.tail < q.qcap)
					q.data[q.tail++] = (t_point){cx + 1, cy};
				map_copy[cy][cx + 1] = VISITED;
			}
		}
		if (cx - 1 >= 0)
		{
			char c = map_copy[cy][cx - 1];
			if (c != MAP_WALL_SYMBOL && c != VISITED)
			{
				if (c == MAP_COLLECT_SYMBOL)
					++found;
				if (q.tail < q.qcap)
					q.data[q.tail++] = (t_point){cx - 1, cy};
				map_copy[cy][cx - 1] = VISITED;
			}
		}
		if (cy + 1 < (int)map->height)
		{
			char c = map_copy[cy + 1][cx];
			if (c != MAP_WALL_SYMBOL && c != VISITED)
			{
				if (c == MAP_COLLECT_SYMBOL)
					++found;
				if (q.tail < q.qcap)
					q.data[q.tail++] = (t_point){cx, cy + 1};
				map_copy[cy + 1][cx] = VISITED;
			}
		}
		if (cy - 1 >= 0)
		{
			char c = map_copy[cy - 1][cx];
			if (c != MAP_WALL_SYMBOL && c != VISITED)
			{
				if (c == MAP_COLLECT_SYMBOL)
					++found;
				if (q.tail < q.qcap)
					q.data[q.tail++] = (t_point){cx, cy - 1};
				map_copy[cy - 1][cx] = VISITED;
			}
		}
	}
	free(q.data);
	map_free_copy(map_copy);
	return (found == total_collect);
}

/* Reuse BFS idea for exit reachability (returns 1 if exit is reachable) */
int	map_check_exit(t_map *map, t_point player, t_point exit)
{
	t_queue	q;
	char	**map_copy;
	int		reached;

	map_copy = map_duplicate(map);
	if (!map_copy)
		return (MEM_ALLOC_ERR_CODE);
	if (!queue_init(&q, map->width * map->height))
	{
		map_free_copy(map_copy);
		return (MEM_ALLOC_ERR_CODE);
	}
	reached = 0;
	q.data[q.tail++] = player;
	map_copy[player.y][player.x] = VISITED;
	while (q.head < q.tail)
	{
		t_point	cur = q.data[q.head++];
		int		cx = cur.x;
		int		cy = cur.y;
		if (cx < 0 || cy < 0 || cx >= (int)map->width || cy >= (int)map->height)
			continue ;
		if (cx == (int)exit.x && cy == (int)exit.y)
		{
			reached = 1;
			break ;
		}
		if (cx + 1 < (int)map->width)
		{
			char c = map_copy[cy][cx + 1];
			if (c != MAP_WALL_SYMBOL && c != VISITED)
			{
				if (q.tail < q.qcap)
					q.data[q.tail++] = (t_point){cx + 1, cy};
				map_copy[cy][cx + 1] = VISITED;
			}
		}
		if (cx - 1 >= 0)
		{
			char c = map_copy[cy][cx - 1];
			if (c != MAP_WALL_SYMBOL && c != VISITED)
			{
				if (q.tail < q.qcap)
					q.data[q.tail++] = (t_point){cx - 1, cy};
				map_copy[cy][cx - 1] = VISITED;
			}
		}
		if (cy + 1 < (int)map->height)
		{
			char c = map_copy[cy + 1][cx];
			if (c != MAP_WALL_SYMBOL && c != VISITED)
			{
				if (q.tail < q.qcap)
					q.data[q.tail++] = (t_point){cx, cy + 1};
				map_copy[cy + 1][cx] = VISITED;
			}
		}
		if (cy - 1 >= 0)
		{
			char c = map_copy[cy - 1][cx];
			if (c != MAP_WALL_SYMBOL && c != VISITED)
			{
				if (q.tail < q.qcap)
					q.data[q.tail++] = (t_point){cx, cy - 1};
				map_copy[cy - 1][cx] = VISITED;
			}
		}
	}
	free(q.data);
	map_free_copy(map_copy);
	return (reached);
}

int	map_check_reachability(t_map *map, t_point player,
	t_point exit, int total_collect)
{	
	int	res;

	res = map_check_collectibles(map, player, total_collect);
	if (!res)
	{
        write(STDERR_FILENO, MAP_CLTS_NOT_REACH_ERR_MSG,
			ft_strlen(MAP_CLTS_NOT_REACH_ERR_MSG));
        return (ERROR_CODE);
	}
	res = map_check_exit(map, player, exit);
	if (!res)
	{
        write(STDERR_FILENO, MAP_NO_WAY_TO_EXIT_ERR_MSG,
			ft_strlen(MAP_NO_WAY_TO_EXIT_ERR_MSG));
        return (ERROR_CODE);
	}
	if (res == MEM_ALLOC_ERR_CODE)
		return (MEM_ALLOC_ERR_CODE);
	return (SUCCESS_CODE);
}

int	queue_init(t_queue *q, size_t qcap)
{
	q->head = 0;
	q->tail = 0;
	q->qcap = qcap;
	q->data = (t_point *)malloc(q->qcap * sizeof (t_point));
	if (!q->data)
		return (ERROR_CODE);
	return (SUCCESS_CODE);
}
