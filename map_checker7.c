#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

/* GOOD! */
void	map_exit_hndl_up_neighbor(t_queue *q, char **map_copy, t_point cur)
{
	char	c;

	if (cur.y - 1 >= 0)
	{
		c = map_copy[cur.y - 1][cur.x];
		if (c != MAP_WALL_SYMBOL && c != VISITED)
		{
			if (q->tail < q->qcap)
				q->data[q->tail++] = (t_point){cur.x, cur.y - 1};
			map_copy[cur.y - 1][cur.x] = VISITED;
		}
	}
}

/* GOOD! */
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
