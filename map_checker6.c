#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

/* GOOD! */
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

/* GOOD! */
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

/* GOOD! */
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
