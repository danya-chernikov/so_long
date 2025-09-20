#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

/* Reuse BFS idea for exit reachability (returns 1 if exit is reachable) */
/* GOOD! */
int	map_check_exit(t_game_data *gdata, t_point player)
{
	t_queue	q;
	char	**map_copy;
	int		reached;

	map_copy = map_duplicate(&gdata->map);
	if (!map_copy)
		return (MEM_ALLOC_ERR_CODE);
	if (!queue_init(&q, gdata->map.width * gdata->map.height))
	{
		map_free_copy(map_copy);
		return (MEM_ALLOC_ERR_CODE);
	}
	reached = 0;
	q.data[q.tail++] = player;
	map_copy[player.y][player.x] = VISITED;
	map_check_exit_loop(gdata, map_copy, &q, &reached);
	free(q.data);
	map_free_copy(map_copy);
	return (reached);
}

/* GOOD! */
void	map_check_exit_loop(t_game_data *gdata, char **map_copy,
			t_queue *q, int *reached)
{
	t_point	cur;

	while (q->head < q->tail)
	{
		cur = q->data[q->head++];
		if (cur.x < 0 || cur.y < 0 ||
			cur.x >= (int)gdata->map.width || cur.y >= (int)gdata->map.height)
			continue ;
		if (cur.x == (int)gdata->exit_coord.x &&
			cur.y == (int)gdata->exit_coord.y)
		{
			*reached = 1;
			break ;
		}
		map_exit_hndl_right_neighbor(q, map_copy, gdata->map.width, cur);
		map_exit_hndl_left_neighbor(q, map_copy, cur);
		map_exit_hndl_down_neighbor(q, map_copy, gdata->map.height, cur);
		map_exit_hndl_up_neighbor(q, map_copy, cur);
	}
}

/* GOOD! */
void	map_exit_hndl_right_neighbor(t_queue *q, char **map_copy, size_t map_width, t_point cur)
{
	char	c;

	if (cur.x + 1 < (int)map_width)
	{
		c = map_copy[cur.y][cur.x + 1];
		if (c != MAP_WALL_SYMBOL && c != VISITED)
		{
			if (q->tail < q->qcap)
				q->data[q->tail++] = (t_point){cur.x + 1, cur.y};
			map_copy[cur.y][cur.x + 1] = VISITED;
		}
	}
}

/* GOOD! */
void	map_exit_hndl_left_neighbor(t_queue *q, char **map_copy, t_point cur)
{
	char	c;

	if (cur.x - 1 >= 0)
	{
		c = map_copy[cur.y][cur.x - 1];
		if (c != MAP_WALL_SYMBOL && c != VISITED)
		{
			if (q->tail < q->qcap)
				q->data[q->tail++] = (t_point){cur.x - 1, cur.y};
			map_copy[cur.y][cur.x - 1] = VISITED;
		}
	}
}

/* GOOD! */
void	map_exit_hndl_down_neighbor(t_queue *q, char **map_copy, size_t map_height, t_point cur)
{
	char	c;

	if (cur.y + 1 < (int)map_height)
	{
		c = map_copy[cur.y + 1][cur.x];
		if (c != MAP_WALL_SYMBOL && c != VISITED)
		{
			if (q->tail < q->qcap)
				q->data[q->tail++] = (t_point){cur.x, cur.y + 1};
			map_copy[cur.y + 1][cur.x] = VISITED;
		}
	}
}
