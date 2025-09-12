/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:23:54 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/12 19:03:54 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "game_logic.h"
#include <stdlib.h>

/* Copies `size` bytes of content from `src` into
 * `dst` starting with `offest` */
void    map_copy_content(char *dst, const char *src,
            const size_t size, const size_t offset)
{
    size_t  i;

    i = 0;
    while (i < size)
    {
        dst[offset + i] = src[i];
        ++i;
    }
}

/* Determines the map's height, i.e. the number of rows that the
 * map consists of.
 * A valid map must have at least 3 raws.
 * Let's count new line symbols first.
 *     nld_num - number of new lines found
 *         
 * We remember we may have two situations:
 * ('$' here means a new lines symbol)
 *     First situation: we have $ at the end of the last raw
 *         We get this if we just execute 'echo "1000000001" >> file'
 *             1111111111$
 *             1000000001$
 *         Second situation: we don't have $ at the end of the last raw
 *             We could get this if we executed 'echo -n "1000000001" >> file'
 *                 1111111111$
 *                 1000000001
 */
void    map_calc_height(t_map *map, char *file_cnt, size_t cnt_size)
{
    size_t  nls_num;
    size_t  i;

    i = 0;
    nls_num = 1;
    if (file_cnt[cnt_size - 1] == '\n')
        --nls_num;
    while (i < cnt_size)
    {
        if (file_cnt[i] == '\n')
            ++nls_num;
        ++i;
    } 
    map->height = nls_num;
}

/* Splits and copies each row from the map file content into the
 * map matrix. By 'row', we mean a substring located between
 * newline characters */
int map_split_raws(t_map *map, char *file_cnt, size_t cnt_size)
{
    size_t  line_len;
    size_t  raw_i;
    size_t  cnt_i;

    map->matrix = (char **)malloc(map->height * sizeof (char *));
    if (!map->matrix)
    {
        write(STDERR_FILENO, MEM_ALLOC_ERR_MSG, ft_strlen(MEM_ALLOC_ERR_MSG));
        return (ERROR_CODE);
    }
    raw_i = 0;
    cnt_i = 0;
    while (raw_i < map->height)
    {
        if (!map_detect_raw(map, file_cnt, cnt_size,
            pack_args(3, (void *)&raw_i, (void *)&cnt_i, (void *)&line_len)))
            return (ERROR_CODE);
        map_copy_raw_into_matrix(map, file_cnt,
            pack_args(3, (void *)&raw_i, (void *)&cnt_i, (void *)&line_len));
        ++raw_i;
    } // while (raw_i < map->height)
    return (SUCCESS_CODE);
}

int map_detect_raw(t_map *map, char *file_cnt, size_t cnt_size, void **pack)
{
    size_t  *raw_i;
    size_t  *cnt_i;
    size_t  *line_len;
    
    raw_i = (size_t *)pack[0];
    cnt_i = (size_t *)pack[1];
    line_len = (size_t *)pack[2];

    *line_len = map_calc_raw_width(map, file_cnt, cnt_size, cnt_i);
    map->matrix[*raw_i] = (char *)malloc((map->width + 1) * sizeof (char));
    if (!map->matrix[*raw_i])
    {
        --(*raw_i);
        while (*raw_i > 0)
        {
            free(map->matrix[*raw_i]);
            --(*raw_i);
        }
        free(map->matrix[*raw_i]);
        free(map->matrix);
        write(STDERR_FILENO, MEM_ALLOC_ERR_MSG, ft_strlen(MEM_ALLOC_ERR_MSG));
        return (ERROR_CODE);
    }
    return (SUCCESS_CODE);
}

/* Calculates the length of the next line of the map
 * that is currently being parsed */
size_t  map_calc_raw_width(t_map *map, char *file_cnt,
    size_t cnt_size, size_t *cnt_i)
{
    size_t  line_len;

    line_len = 0;
    while (file_cnt[*cnt_i] != '\n' && *cnt_i < cnt_size)
    {
        ++(*cnt_i);
        ++line_len;
    }
    ++(*cnt_i);
    map->width = line_len;
    return (line_len);
}
