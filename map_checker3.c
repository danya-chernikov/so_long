/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:46:05 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/12 19:03:54 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

/* Now let's copy this raw into our matrix */
void    map_copy_raw_into_matrix(t_map *map, char *file_cnt, void **pack)
{
    size_t  j;
    size_t  raw_i;
    size_t  cnt_i;
    size_t  line_len;
    
    raw_i = *(size_t *)pack[0];
    cnt_i = *(size_t *)pack[1];
    line_len = *(size_t *)pack[2];
    j = 0;
    while (j < line_len)
    {
        map->matrix[raw_i][j] = file_cnt[cnt_i - line_len + j - 1];
        ++j;
    }
    map->matrix[raw_i][j] = '\0';
    free(pack);
}

void    map_matrix_free(t_map *map)
{
	ft_printf("map_matrix_free()\n");
    size_t  i;

    i = 0;
    while (i < map->height)
    {
        free(map->matrix[i]);
        ++i;
    }
    free(map->matrix);
}

/* First checks map file extension */
int map_check(t_map *map)
{
	(void)map;	
	// Printing the map
	ft_printf("%s\n", "checking the map...");
	return (SUCCESS_CODE);
}

void	map_print(t_map *map)
{
	size_t	i;

	i = 0;
    while (i < map->height)
	{
        ft_printf("%s\n", map->matrix[i]);
		++i;
	}
}
