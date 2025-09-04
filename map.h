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

#define MAP_FILE_EXTENSION          ".ber"
#define MAP_FILE_EXT_ERR            "Map file extension is invalid\n"
#define MAP_FORMAT_RECT_ERR         "Map format error: map must be rectangular\n"
#define MAP_FORMAT_ZERO_WIDTH_ERR   "Map format error: map cannot contain empty raws\n"
#define MAP_FORMAT_EMPTY_ERR        "Map format error: map file is empty\n"
#define MEM_ALLOC_ERR               "Memory allocation error\n"

typedef struct  s_map
{
    int     width;
    int     height;
    char    **matrix;
}   t_map;

int     map_init(t_map *map, const char *file_path);
int     map_check_file_ext(const char *file_path);
int     map_check(const t_map *map);
void    map_free(t_map *map);

void    copy_content(char *dst, const char *src,
            const size_t size, const size_t offset);

#endif
