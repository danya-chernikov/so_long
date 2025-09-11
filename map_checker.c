/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:51:36 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/11 15:09:30 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* We need stdio.h for perror */
#include "libft/libft.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>

/* Initializes the map provided by user with
 * `file_path`.
 *     1. Checks if the map file extension is correct;
 *     2. Opens the map file;
 *     3. Allocates memory for all map file buffer;
 * Reads map file content chunk by chunk
 * increasing the cnt_size dynamically until
 * all file content will be read.
 *     file_cnt - all map file content;
 *     cnt_size - all map file content size */
int map_init(t_map *map, const char *file_path)
{
    int     fd;
    char    *file_cnt;

    if (!map_check_file_ext(file_path))
    {
        write(STDERR_FILENO, MAP_EXT_ERR_MSG, ft_strlen(MAP_EXT_ERR_MSG));
        return (ERROR_CODE);
    }
    fd = open(file_path, O_RDONLY);
    if (fd < 0)
    {
        perror(FILE_OPEN_ERR_MSG);
        return (ERROR_CODE);
    }
    file_cnt = (char *)malloc(MAP_INC_CHUNK_SIZE * sizeof (char));
    if (!file_cnt)
    {
        write(STDERR_FILENO, MEM_ALLOC_ERR_MSG, ft_strlen(MEM_ALLOC_ERR_MSG));
        return (ERROR_CODE);
    }
	if (!map_move_raws_into_matrix(map, fd, &file_cnt))
		return (ERROR_CODE);
    free(file_cnt);
    close(fd);
    return (SUCCESS_CODE);
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

/* Does this function have an appropriate name? */
int	map_move_raws_into_matrix(t_map *map, int fd, char **file_cnt)
{
    int     res;
    size_t  cnt_size;

	res = map_read_cnt(fd, file_cnt, &cnt_size);
	if (!res)
	{
		free(*file_cnt);
		file_cnt = NULL;
		return (ERROR_CODE);
	}
	if (cnt_size == 0)
	{
		write(STDERR_FILENO, MAP_EMPTY_ERR_MSG, ft_strlen(MAP_EMPTY_ERR_MSG));
		return (ERROR_CODE);
	}
	map_calc_height(map, *file_cnt, cnt_size);
	if (map->height < MAP_MIN_RAWS_NUM)
	{
		write(STDERR_FILENO, MAP_MIN_RAWS_ERR_MSG,
			ft_strlen(MAP_MIN_RAWS_ERR_MSG));
		return (ERROR_CODE);
	} 
	if (!map_split_raws(map, *file_cnt, cnt_size))
		return (ERROR_CODE);
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

/* !!! RESOLVE PROBLEM WITH FT_REALLOC() !!! */
int map_read_cnt_chunk(int fd, char **file_cnt, size_t *cnt_size, char **chunk)
{
    char    *tmp_cnt;
    int     rlen;

    rlen = read(fd, *chunk, MAP_INC_CHUNK_SIZE);
    if (rlen <= 0)
    {
        if (rlen < 0)
            perror(FILE_READ_ERR_MSG); // CORRECT THIS! THE FUNCTION WILL RETURN SUCCESS_CODE ANYWAY! MUST RETURN ERROR_CODE INSTEAD IF rlen < 0
        return (BREAK);
    }
    else
    {
        tmp_cnt = ft_realloc(*file_cnt, *cnt_size, *cnt_size + rlen); // THIS DOES NOT WORK AT ALL!
        if (!tmp_cnt)
        {
            write(STDERR_FILENO, MEM_ALLOC_ERR_MSG, ft_strlen(MEM_ALLOC_ERR_MSG));
            free(*chunk);
            *chunk = NULL;
            return (ERROR_CODE);
        }
        *file_cnt = tmp_cnt;
        map_copy_content(*file_cnt, *chunk, rlen, *cnt_size);
        *cnt_size += rlen;
    }
    return (SUCCESS_CODE);
}
