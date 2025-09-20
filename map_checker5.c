#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

/* BFS to count reachable collectibles. Returns 1 of all collectibles are
 * reachable from player, 0 otherwise */
int	map_check_collectibles(t_map *map, t_point player, int total_collect)
{
	t_point	*queue;
	char	**map_copy;
	int		qcap;
	int		head;
	int		tail;
	int		found;

	map_copy = map_duplicate(map);
	if (!map_copy)
		return (MEM_ALLOC_ERR_CODE);
	qcap = map->width * map->height;
	queue = (t_point *)malloc(qcap * sizeof (t_point));
	if (!queue)
	{
		map_free_copy(map_copy);
		return (MEM_ALLOC_ERR_CODE);
	}
	head = 0;
	tail = 0;
	found = 0;
	queue[tail++] = player;
	while (head < tail)
	{
		t_point	cur = queue[head++];
		int		cx = cur.x;
		int		cy = cur.y;
		if (cx < 0 || cy < 0 || cx >= (int)map->width || cy >= (int)map->height)
			continue ;
		if (map_copy[cy][cx] == MAP_WALL_SYMBOL || map_copy[cy][cx] == VISITED)
			continue ;
		if (map_copy[cy][cx] == MAP_COLLECT_SYMBOL)
			++found;
		if ((map_copy[cy][cx] == MAP_EXIT_SYMBOL) ||
			(map_copy[cy][cx] == MAP_SEA_SYMBOL) ||
			(map_copy[cy][cx] == MAP_COLLECT_SYMBOL) ||
			(map_copy[cy][cx] == MAP_PLAYER_POS_SYMBOL))
			map_copy[cy][cx] = VISITED;

		/* push neighbors */
		if (tail + 4 <= qcap)
		{
			queue[tail++] = (t_point){cx + 1, cy};
			queue[tail++] = (t_point){cx - 1, cy};
			queue[tail++] = (t_point){cx, cy + 1};
			queue[tail++] = (t_point){cx, cy - 1};
		}
	}
	free(queue);
	map_free_copy(map_copy);
	return (found == total_collect);
}

/* Reuse BFS idea for exit reachability (returns 1 if exit is reachable) */
int	map_check_exit(t_map *map, t_point player, t_point exit)
{
	t_point	*queue;
	char	**map_copy;
	int		qcap;
	int		head;
	int		tail;
	int		reached;

	map_copy = map_duplicate(map);
	if (!map_copy)
		return (MEM_ALLOC_ERR_CODE);
	qcap = map->width * map->height;
	queue = (t_point *)malloc(qcap * sizeof (t_point));
	if (!queue)
	{
		map_free_copy(map_copy);
		return (MEM_ALLOC_ERR_CODE);
	}
	head = 0;
	tail = 0;
	reached = 0;
	queue[tail++] = player;
	while (head < tail)
	{
		t_point	cur = queue[head++];
		int		cx = cur.x;
		int		cy = cur.y;
		if (cx < 0 || cy < 0 || cx >= (int)map->width || cx >= (int)map->height)
			continue ;
		if (map_copy[cy][cx] == MAP_WALL_SYMBOL || map_copy[cy][cx] == VISITED)
			continue ;
		if (cx == (int)exit.x && cy == (int)exit.y)
		{
			reached = 1;
			break ;
		}
		if ((map_copy[cy][cx] == MAP_EXIT_SYMBOL) ||
			(map_copy[cy][cx] == MAP_SEA_SYMBOL) ||
			(map_copy[cy][cx] == MAP_COLLECT_SYMBOL) ||
			(map_copy[cy][cx] == MAP_PLAYER_POS_SYMBOL))
			map_copy[cy][cx] = VISITED;
		if (tail + 4 <= qcap)
		{
			queue[tail++] = (t_point){cx + 1, cy};
			queue[tail++] = (t_point){cx - 1, cy};
			queue[tail++] = (t_point){cx, cy + 1};
			queue[tail++] = (t_point){cx, cy - 1};
		}
	}
	free(queue);
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
