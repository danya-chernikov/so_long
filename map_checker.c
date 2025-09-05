/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:51:36 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/05 23:03:41 by dchernik         ###   ########.fr       */
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
    size_t  i;
    int     fd;
    size_t  rlen;
    size_t  cnt_size;
    char    *read_chunk;
    char    *file_cnt;

    (void)map;

    // Checking if map file extension is correct
    if (!map_check_file_ext(file_path))
    {
        write(2, MAP_FILE_EXT_ERR, ft_strlen(MAP_FILE_EXT_ERR));
        return (EXIT_FAILURE);
    }

    // Let's open the map file
    fd = open(file_path, O_RDONLY);
    if (fd < 0)
    {
        perror("open(): Could not open the map file");
        return (EXIT_FAILURE);
    }

    // Allocating memory for a chunk
    read_chunk = (char *)malloc(MAP_INC_CHUNK_SIZE * sizeof (char));
    if (!read_chunk)
    {
        write(2, MEM_ALLOC_ERR, ft_strlen(MEM_ALLOC_ERR));
        return (EXIT_FAILURE);
    }

    // Allocating memory for all map file content buffer
    file_cnt = (char *)malloc(MAP_INC_CHUNK_SIZE * sizeof (char));
    if (!file_cnt)
    {
        write(2, MEM_ALLOC_ERR, ft_strlen(MEM_ALLOC_ERR));
        return (EXIT_FAILURE);
    }

    char    *tmp_cnt;

    rlen = 0;
    cnt_size = 0;
    // Reading the map file chunk by chunk
    while (1)
    {
        rlen = read(fd, read_chunk, MAP_INC_CHUNK_SIZE);
        if (!rlen) // end-of-file
            break;
        else // Reading the next chunk
        {
            // Let's copy its content into common file content
            // Let's allocate more memory for newly read chunk
            tmp_cnt = ft_realloc(file_cnt, cnt_size, cnt_size + rlen);
            if (!tmp_cnt)
            {
                free(file_cnt);
                write(2, MEM_ALLOC_ERR, ft_strlen(MEM_ALLOC_ERR));
                return (EXIT_FAILURE);
            }
            file_cnt = tmp_cnt;
            copy_content(file_cnt, read_chunk, rlen, cnt_size);
            cnt_size += rlen;
        }
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

    if (nls_num < 3)
    {
        write(STDERR_FILENO, MAP_FORMAT_MIN_RAWS,
                ft_strlen(MAP_FORMAT_MIN_RAWS));
        exit(EXIT_FAILURE);
    }


    // Now let's split all the found new lines into an array
    i = 0;

    map->height = nls_num;
    // Allocating memory for the map
    map->matrix = (char **)malloc(map->height * sizeof (char *));
    if (!map->matrix)
    {
        write(STDERR_FILENO, MEM_ALLOC_ERR, ft_strlen(MEM_ALLOC_ERR));
        return (EXIT_FAILURE);
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
            write(STDERR_FILENO, MEM_ALLOC_ERR, ft_strlen(MEM_ALLOC_ERR));
            return (EXIT_FAILURE);
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

    i = 0;
    while (i < (size_t)map->height)
    {
        free(map->matrix[i]);
        ++i;
    }
    free(map->matrix);

    free(read_chunk);
    free(file_cnt);
    close(fd);
    return (1);
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
        return (0);
    path_len = ft_strlen(file_path);
    ext_len = ft_strlen(MAP_FILE_EXTENSION);
    if (path_len <= ext_len)
        return (0);
    i = 1;
    while (i <= ext_len)
    {
        if (file_path[path_len - i] != MAP_FILE_EXTENSION[ext_len - i])
            return (0);
        ++i;
    }
    return (1);
}

/* First checks map file extension */
int map_check(const t_map *map)
{
   (void)map;

   ft_printf("%s\n", "checking the map...");
   return (1);
}
