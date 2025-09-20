/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:50:45 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/20 13:21:18 by dchernik         ###   ########.fr       */
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

/* Minimum number of rows and columns
 * a map can consist of */
#define MAP_MIN_RAWS_NUM            3
#define MAP_MIN_COLUMNS_NUM         3

/* MAP_OBJECTS_NUM – the number of different
 * objects from which a map can be composed */
#define MAP_FILE_EXTENSION          ".ber"
#define MAP_OBJECTS_NUM				5
#define MAP_SEA_SYMBOL				'0'
#define MAP_WALL_SYMBOL				'1'
#define MAP_COLLECT_SYMBOL			'C'
#define MAP_PLAYER_POS_SYMBOL		'P'
#define MAP_EXIT_SYMBOL				'E'
#define VISITED						'V'

/* Error messages */
/* Map error messages */
#define MAP_PATH_ERR_MSG			"Error\nYou need to specify the path to the game map\n"
#define MAP_EXT_ERR_MSG             "Error\nMap file extension is invalid\n"
#define MAP_RECT_ERR_MSG            "Error\nMap must be rectangular\n"
#define MAP_EMPTY_RAW_ERR_MSG       "Error\nMap cannot contain empty rows\n"
#define MAP_EMPTY_ERR_MSG           "Error\nMap file is empty\n"
#define MAP_MIN_RAWS_ERR_MSG        "Error\nMap has to contain at least 3 rows\n"
#define MAP_INVALID_SYMBOL_ERR_MSG	"Error\nMap contains an invalid symbol\n"
#define MAP_MUST_BE_CLOSED_ERR_MSG	"Error\nMap must be enclosed\n"
#define MAP_DUPLICATES_ERR_MSG		"Error\nThe map must have exactly one exit and one initial player position\n"
#define MAP_AT_LEAST_1_COL_ERR_MSG	"Error\nMap must contain at least one collectible object\n"
#define MAP_CLTS_NOT_REACH_ERR_MSG	"Error\nMap has some collectibles that are not reachable\n"
#define MAP_NO_WAY_TO_EXIT_ERR_MSG	"Error\nMap does not have a valid path to exit\n"

typedef struct s_point
{
	size_t	x;
	size_t	y;
}	t_point;

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
void	map_print(const t_map *map);
int		map_get_collect_num(const t_map *map);
int     map_check(const t_map *map);

/* map_checker4.c */
int		map_check_cnt_is_valid(const t_map *map);
int		map_check_if_closed(const t_map *map);
int		map_check_duplicates(const t_map *map);
char	**map_duplicate(t_map *map);
void	map_free_copy(char **map_copy);

/* map_checker5.c */
int		map_check_collectibles(t_map *map, t_point player, int total_collect);
int		map_check_exit(t_map *map, t_point player, t_point exit);
int		map_check_reachability(t_map *map, t_point player,
			t_point exit, int total_collect);

#endif
