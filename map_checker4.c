#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

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

char	**map_duplicate(t_map *map)
{
	char	**map_copy;
	size_t	i;
	size_t	j;

	map_copy = (char **)malloc(sizeof (char *) * (map->height + 1));
	if (!map_copy)
		return (NULL);
	i = 0;
	while (i < map->height)
	{
		map_copy[i] = (char *)malloc(map->width + 1);
		if (!map_copy[i])
			return (NULL);
		j = 0;
		while (map->matrix[i][j])
		{
			map_copy[i][j] = map->matrix[i][j];
			++j;
		}
		map_copy[i][j] = '\0';
		++i;
	}
	map_copy[i] = NULL;
	return (map_copy);
}

void	map_free_copy(char **map_copy)
{
	size_t	i;

	if (!map_copy)
		return ;
	i = 0;
	while (map_copy[i])
	{
		free(map_copy[i]);
		++i;
	}
	free(map_copy);
}
