/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:51:36 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/08 19:38:48 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>

/* Reads map file content chunk by chunk
 * increasing the cnt_size dynamically until
 * all file content will be read.
 *     file_cnt - all map file content;
 *     cnt_size - all map file content size */
int map_init(t_map *map, const char *file_path)
{
    int     fd;
    int     res;
    size_t  i;
    size_t  cnt_size;
    char    *file_cnt;

    // Checking if map file extension is correct
    if (!map_check_file_ext(file_path))
    {
        write(STDERR_FILENO, MAP_EXT_ERR_MSG, ft_strlen(MAP_EXT_ERR_MSG));
        return (ERROR_CODE);
    }

    // Let's open the map file
    fd = open(file_path, O_RDONLY);
    if (fd < 0)
    {
        write(STDERR_FILENO, MAP_OPEN_ERR_MSG, ft_strlen(MAP_OPEN_ERR_MSG));
        return (ERROR_CODE);
    }

    // Allocating memory for all map file content buffer
    file_cnt = (char *)malloc(MAP_INC_CHUNK_SIZE * sizeof (char));
    if (!file_cnt)
    {
        write(STDERR_FILENO, MEM_ALLOC_ERR_MSG, ft_strlen(MEM_ALLOC_ERR_MSG));
        return (ERROR_CODE);
    }
    
    res = map_read_cnt(fd, &file_cnt, &cnt_size);
    if (!res)
    {
        free(file_cnt);
        return (ERROR_CODE);
    }
    if (cnt_size == 0)
    {
        write(STDERR_FILENO, MAP_EMPTY_ERR_MSG, ft_strlen(MAP_EMPTY_ERR_MSG));
        return (ERROR_CODE);
    }

    // Let's print all read content
    ft_printf("\n%s\n", file_cnt);
    ft_printf("\n%u\n", cnt_size);

    // Let's count how many raws our map has
    // A valid map must have at least 3 raws
    //
    // Let's count new line symbols first
    //
    // We remember we may have two situations
    // '$' here means a new lines symbol
    //
    // First situation (we have $ at the end of the last raw)
    // We get this if we just execute 'echo "1000000001" >> file'
    // 1111111111$
    // 1000000001$
    //
    // Second situation (we don't have $ at the end of the last raw)
    // We could get this if we executed 'echo -n "1000000001" >> file'
    // 1111111111$
    // 1000000001

    size_t  nls_num; // number of new lines found

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
    
    ft_printf("nls_num = %u\n", nls_num);

    if (nls_num < MAP_MIN_RAWS_NUM)
    {
        write(STDERR_FILENO, MAP_MIN_RAWS_ERR_MSG,
            ft_strlen(MAP_MIN_RAWS_ERR_MSG));
        return (ERROR_CODE);
    }

    // Now let's split all the found new lines into an array
    i = 0;

    map->height = nls_num;
    // Allocating memory for the map
    map->matrix = (char **)malloc(map->height * sizeof (char *));
    if (!map->matrix)
    {
        write(STDERR_FILENO, MEM_ALLOC_ERR_MSG, ft_strlen(MEM_ALLOC_ERR_MSG));
        return (0);
    }

    size_t  raw_i;
    size_t  cnt_i;

    raw_i = 0;
    cnt_i = 0;
    while (raw_i < map->height)
    {
        i = 0;
        while (file_cnt[cnt_i] != '\n' && cnt_i < cnt_size)
        {
            ++cnt_i;
            ++i;
        }
        ++cnt_i;
        map->width = i;

        map->matrix[raw_i] = (char *)malloc((map->width + 1) * sizeof (char));
        if (!map->matrix[raw_i])
        {
            --raw_i;
            while (raw_i > 0)
            {
                free(map->matrix[raw_i]);
                --raw_i;
            }
            free(map->matrix[raw_i]);
            free(map->matrix);
            write(STDERR_FILENO, MEM_ALLOC_ERR_MSG, ft_strlen(MEM_ALLOC_ERR_MSG));
            return (ERROR_CODE);
        }

        // Now let's copy this raw into our matrix
        size_t  j;

        j = 0;
        while (j < i)
        {
            map->matrix[raw_i][j] = file_cnt[cnt_i - i + j - 1];
            ++j;
        }
        map->matrix[raw_i][j] = '\0';

        ++raw_i;
    }

    // Printing the map
    for (size_t ri = 0; ri < map->height; ++ri)
        ft_printf("%s\n", map->matrix[ri]);

    // Free all the stuff
    i = 0;
    while (i < (size_t)map->height)
    {
        free(map->matrix[i]);
        ++i;
    }
    free(map->matrix);

    free(file_cnt);
    close(fd);

    return (SUCCESS_CODE);
}

/* Reads the map file content into `file_cnt`. We read the map
 * file chunk by chunk in an infinite loop. If the end of file
 * is reached, we exit the loop; otherwise, we read the next chunk,
 * reallocate memory for `file_cnt` (increasing it by the size of
 * the previously read chunk), and append the new content to
 * `file_cnt`. On success, returns the number of bytes read from
 * the map file; if the return value is -1, a memory allocation
 * error has occurred. A return value of 0 is also considered an
 * error, since the map file cannot be empty.
 * WHY DO WE SEPARATE EXIT ERROR CODES HERE?! */
int map_read_cnt(int fd, char **file_cnt, size_t *cnt_size)
{
    char    *cnt_chunk;
    int     res;

    cnt_chunk = (char *)malloc(MAP_INC_CHUNK_SIZE * sizeof (char));
    if (!cnt_chunk)
    {
        write(STDERR_FILENO, MEM_ALLOC_ERR_MSG, ft_strlen(MEM_ALLOC_ERR_MSG));
        return (ERROR_CODE);
    }
    *cnt_size = 0;
    while (1)
    {
        res = map_read_cnt_chunk(fd, file_cnt, cnt_size, &cnt_chunk);
        if (res != SUCCESS_CODE)
            break ;
    }
    if (cnt_chunk)
        free(cnt_chunk);
    if (!res)
        return (ERROR_CODE);
    return (SUCCESS_CODE);
}

int map_read_cnt_chunk(int fd, char **file_cnt, size_t *cnt_size, char **chunk)
{
    char    *tmp_cnt;
    int     rlen;

    rlen = read(fd, *chunk, MAP_INC_CHUNK_SIZE);
    if (rlen <= 0)
    {
        if (rlen < 0)
            perror(READ_FILE_ERR);
        return (BREAK);
    }
    else
    {
        tmp_cnt = ft_realloc(*file_cnt, *cnt_size, *cnt_size + rlen);
        if (!tmp_cnt)
        {
            write(STDERR_FILENO, MEM_ALLOC_ERR_MSG, ft_strlen(MEM_ALLOC_ERR_MSG));
            free(*chunk);
            *chunk = NULL;
            return (ERROR_CODE);
        }
        *file_cnt = tmp_cnt;
        copy_content(*file_cnt, *chunk, rlen, *cnt_size);
        *cnt_size += rlen;
    }
    return (SUCCESS_CODE);
}

/* Copies `size` bytes of content from `src` into `dst` starting with `offest` */
void    copy_content(char *dst, const char *src,
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

void    map_free(t_map *map)
{
    size_t  i;

    i = 0;
    while (i < map->height)
    {
        free(map->matrix[i]);
        ++i;
    }
    free(map->matrix);
}

/* Checks if map provided by the user in argv[1]
 * has an appropriate extension. Returns 1 if
 * everything is okay */
int map_check_file_ext(const char *file_path)
{
    int i;
    int ext_len;
    int path_len;

    if (!file_path)
        return (ERROR_CODE);
    path_len = ft_strlen(file_path);
    ext_len = ft_strlen(MAP_FILE_EXTENSION);
    if (path_len <= ext_len)
        return (ERROR_CODE);
    i = 1;
    while (i <= ext_len)
    {
        if (file_path[path_len - i] != MAP_FILE_EXTENSION[ext_len - i])
            return (ERROR_CODE);
        ++i;
    }
    return (SUCCESS_CODE);
}

/* First checks map file extension */
int map_check(const t_map *map)
{
   (void)map;

   ft_printf("%s\n", "checking the map...");
   return (SUCCESS_CODE);
}
