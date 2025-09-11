/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:50:45 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/11 15:07:24 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

#include "printf/ft_printf.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* This is the number that will be added
 * to each newly allocated memory size
 * for our map file content when there
 * is not enough memory to accommodate
 * new map raws. In  other words, it is
 * the amount by which the size of the
 * `all_file_cnt` array  will be increased
 * during each subsequent  memory reallocation */
#define MAP_INC_CHUNK_SIZE          8192
#define MAP_MIN_RAWS_NUM            3
#define MAP_MIN_COLUMNS_NUM         3
#define MAP_FILE_EXTENSION          ".ber"

/* Error messages */
/* Map error messages */
#define MAP_PATH_ERR_MSG			"You need to specify the path to the game's map\n"
#define MAP_EXT_ERR_MSG             "Map file extension is invalid\n"
#define MAP_RECT_ERR_MSG            "Map format error: map must be rectangular\n"
#define MAP_EMPTY_RAW_ERR_MSG       "Map format error: map cannot contain empty raws\n"
#define MAP_EMPTY_ERR_MSG           "Map format error: map file is empty\n"
#define MAP_MIN_RAWS_ERR_MSG        "Map format error: map has to contain at least 3 raws\n"

/* MLX library error messages */
#define MLX_INIT_ERR_MSG			"mlx_init(): Failed to initialize mlx\n"
#define MLX_WIN_CREATE_ERR_MSG		"mlx_new_window(): Cannot create a window\n"

/* System error messages ( for using with perror() ) */
#define MEM_ALLOC_ERR_MSG           "malloc()"
#define FILE_OPEN_ERR_MSG           "open()"
#define FILE_READ_ERR_MSG           "read()"

/* Exit codes
 *     ERROR         - denotes a general error (which may
 *                     also include a memory allocation error);
 *     MEM_ALLOC_ERR - specifically denotes a memory
 *                     allocation error, used when a
 *                     function can return multiple
 *                     error codes */
#define SUCCESS_CODE                1
#define ERROR_CODE                  0
#define MEM_ALLOC_ERR_CODE          -1

#define BREAK                       2

typedef struct  s_map
{
    size_t  width;
    size_t  height;
    char    **matrix;
}   t_map;

/* map_checker.c */
int     map_init(t_map *map, const char *file_path);
int     map_check_file_ext(const char *file_path);
int		map_move_raws_into_matrix(t_map *map, int fd, char **file_cnt);
int     map_read_cnt(int fd, char **file_cnt, size_t *cnt_size);
int     map_read_cnt_chunk(int fd, char **file_cnt,
            size_t *cnt_size, char **chunk);

/* map_checker2.c */
void    map_copy_content(char *dst, const char *src,
            const size_t size, const size_t offset);
void    map_calc_height(t_map *map, char *file_cnt, size_t cnt_size);
int     map_split_raws(t_map *map, char *file_cnt, size_t cnt_size);
int     map_detect_raw(t_map *map, char *file_cnt,
			size_t cnt_size, void **pack);
size_t  map_calc_raw_width(t_map *map, char *file_cnt,
			size_t cnt_size, size_t *cnt_i);

/* map_checker3.c */
void    map_copy_raw_into_matrix(t_map *map, char *file_cnt, void **pack);
void    map_matrix_free(t_map *map);
int     map_check(t_map *map);
void	map_print(t_map *map);

#endif
