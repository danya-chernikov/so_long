/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:51:49 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/21 02:51:52 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

/* GOOD! */
int	map_check_cnt_is_valid(const t_map *map)
{
	size_t  raw_i;
	size_t  col_i;

	raw_i = 0;
	while (raw_i < map->height)
	{
		col_i = 0;
		while (col_i < map->width)
		{
			if (map->matrix[raw_i][col_i] != MAP_SEA_SYMBOL &&
				map->matrix[raw_i][col_i] != MAP_WALL_SYMBOL &&
				map->matrix[raw_i][col_i] != MAP_COLLECT_SYMBOL &&
				map->matrix[raw_i][col_i] != MAP_PLAYER_POS_SYMBOL &&
				map->matrix[raw_i][col_i] != MAP_EXIT_SYMBOL)
				return (0);
			++col_i;
		}
		++raw_i;
	}
	return (1);
}

/* Returns 1 if the map is closed, 0 otherwise */
/* GOOD! */
int	map_check_if_closed(const t_map *map)
{
	size_t	raw_i;

	raw_i = 0;
	while (raw_i < map->height)
	{
		if (map->matrix[raw_i][0] != '1' ||
			map->matrix[raw_i][ft_strlen(map->matrix[raw_i]) - 1] != '1')
			return (0);
		++raw_i;
	}
	return (1);
}

/* Checks that the map does not contain
 * duplicate initial player positions or
 * exit tiles, and verifies that there is
 * at least one collectible object, one
 * initial position, and one exit */
/* GOOD! */
int	map_check_duplicates(const t_map *map)
{
	size_t  raw_i;
	size_t  col_i;
	short	exit_cnt;
	short	pos_cnt;

	exit_cnt = 0;
	pos_cnt = 0;
	raw_i = 0;
	while (raw_i < map->height)
	{
		col_i = 0;
		while (col_i < map->width)
		{
			if (map->matrix[raw_i][col_i] == MAP_PLAYER_POS_SYMBOL)
				++pos_cnt;
			else if (map->matrix[raw_i][col_i] == MAP_EXIT_SYMBOL)
				++exit_cnt;
			++col_i;
		}
		++raw_i;
	}
	if (pos_cnt == 0 || exit_cnt == 0 || pos_cnt == 2 || exit_cnt == 2)
		return (0);
	return (1);
}

/* GOOD! */
int	map_check_reachability(t_game_data *gdata)
{
	int	res;

	find_player(gdata);
	find_exit(gdata);
	res = map_check_collectibles(&gdata->map, gdata->player_tile,
		gdata->total_collectibles);
	if (!res)
	{
        write(STDERR_FILENO, MAP_CLTS_NOT_REACH_ERR_MSG,
			ft_strlen(MAP_CLTS_NOT_REACH_ERR_MSG));
        return (ERROR_CODE);
	}
	res = map_check_exit(gdata, gdata->player_tile);
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

/* BFS to count reachable collectibles. Returns 1 of all collectibles are
 * reachable from player, 0 otherwise */
/* GOOD! */
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
	map_check_collectibles_loop(map, map_copy, &q, &found);
	free(q.data);
	map_free_copy(map_copy);
	return (found == total_collect);
}
