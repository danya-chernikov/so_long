/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:51:54 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/21 03:26:04 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

void	map_check_collectibles_loop(t_map *map, char **map_copy,
			t_queue *q, int *found)
{
	t_point	cur;

	while (q->head < q->tail)
	{
		cur = q->data[q->head++];
		if (cur.x < 0 || cur.y < 0
			|| cur.x >= (int)map->width || cur.y >= (int)map->height)
			continue ;
		*found += map_clt_hndl_right_neighbor(q, map_copy, map->width, cur);
		*found += map_clt_hndl_left_neighbor(q, map_copy, cur);
		*found += map_clt_hndl_down_neighbor(q, map_copy, map->height, cur);
		*found += map_clt_hndl_up_neighbor(q, map_copy, cur);
	}
}

int	map_clt_hndl_right_neighbor(t_queue *q, char **map_copy,
	size_t map_width, t_point cur)
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

int	map_clt_hndl_down_neighbor(t_queue *q, char **map_copy,
	size_t map_height, t_point cur)
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
