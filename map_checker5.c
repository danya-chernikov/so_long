#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

int	map_clt_hndl_right_neighbor(t_queue *q, char **map_copy, size_t map_width, t_point cur)
{
	int		found;
	char	c;

	found = 0;
	if (cur.x + 1 < (int)map_width)
	{
		c = map_copy[cur.y][cur.x + 1];
		if (c != MAP_WALL_SYMBOL && c != VISITED)
		{
			if (c == MAP_COLLECT_SYMBOL)
				++found;
			if (q->tail < q->qcap)
				q->data[(q->tail)++] = (t_point){cur.x + 1, cur.y};
			map_copy[cur.y][cur.x + 1] = VISITED;
		}
	}
	return (found);
}

int	map_clt_hndl_left_neighbor(t_queue *q, char **map_copy, t_point cur)
{
	int		found;
	char	c;

	found = 0;
	if (cur.x - 1 >= 0)
	{
		c = map_copy[cur.y][cur.x - 1];
		if (c != MAP_WALL_SYMBOL && c != VISITED)
		{
			if (c == MAP_COLLECT_SYMBOL)
				++found;
			if (q->tail < q->qcap)
				q->data[q->tail++] = (t_point){cur.x - 1, cur.y};
			map_copy[cur.y][cur.x - 1] = VISITED;
		}
	}
	return (found);
}

int	map_clt_hndl_down_neighbor(t_queue *q, char **map_copy, size_t map_height, t_point cur)
{
	int		found;
	char	c;

	found = 0;
	if (cur.y + 1 < (int)map_height)
	{
		c = map_copy[cur.y + 1][cur.x];
		if (c != MAP_WALL_SYMBOL && c != VISITED)
		{
			if (c == MAP_COLLECT_SYMBOL)
				++found;
			if (q->tail < q->qcap)
				q->data[q->tail++] = (t_point){cur.x, cur.y + 1};
			map_copy[cur.y + 1][cur.x] = VISITED;
		}
	}
	return (found);
}

int	map_clt_hndl_up_neighbor(t_queue *q, char **map_copy, t_point cur)
{
	int		found;
	char	c;

	found = 0;
	if (cur.y - 1 >= 0)
	{
		c = map_copy[cur.y - 1][cur.x];
		if (c != MAP_WALL_SYMBOL && c != VISITED)
		{
			if (c == MAP_COLLECT_SYMBOL)
				++found;
			if (q->tail < q->qcap)
				q->data[q->tail++] = (t_point){cur.x, cur.y - 1};
			map_copy[cur.y - 1][cur.x] = VISITED;
		}
	}
	return (found);
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
