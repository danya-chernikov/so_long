#include "libft/libft.h"
#include "game_logic.h"

/* Returns 1 if the map is closed, 0 otherwise */
int		map_check_if_closed(t_map *map)
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

