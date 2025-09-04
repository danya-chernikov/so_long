/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:51:36 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/04 17:31:56 by dchernik         ###   ########.fr       */
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
    //size_t  i;
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
        return (0);
    }

    // Let's open the map file
    fd = open(file_path, O_RDONLY);
    if (fd < 0)
    {
        perror("open(): Could not open the map file");
        return (0);
    }

    // Allocating memory for a chunk
    read_chunk = (char *)malloc(MAP_INC_CHUNK_SIZE * sizeof (char));
    if (!read_chunk)
    {
        write(2, MEM_ALLOC_ERR, ft_strlen(MEM_ALLOC_ERR));
        return (0);
    }

    // Allocating memory for all map file content buffer
    file_cnt = (char *)malloc(MAP_INC_CHUNK_SIZE * sizeof (char));
    if (!file_cnt)
    {
        write(2, MEM_ALLOC_ERR, ft_strlen(MEM_ALLOC_ERR));
        return (0);
    }

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
            ft_realloc(file_cnt, cnt_size, cnt_size + rlen);
            copy_content(file_cnt, read_chunk, rlen, cnt_size);
            cnt_size += rlen;
        }
    }

    // Let's print all read content
    ft_printf("\n%s\n", file_cnt);

    free(read_chunk);
    free(file_cnt);

    // Allocating memory for the map
    /*map->matrix = (char **)malloc(map->height * sizeof (char *));
    if (!map->matrix)
    {

        return (0);
    }
    i = 0;
    while (i < map->height)
    {
        map->matrix[i] = (char *)malloc(map->width * sizeof (char));
        if (!map->matrix[i])
        {
            --i;
            while (i >= 0)
            {
                free(map->matrix[i]);
                --i;
            }
            free(map->matrix);
        }
        ++i;
    }*/
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
    int i;

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
