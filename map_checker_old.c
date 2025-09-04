#include "map.h"
#include <stdio.h>


int map_init(t_map *map, const char *file_path)
{
    size_t  i;
    int     fd;
    char    *read_raw;

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

    // Reading the map file line by line
    map->height = 0;
    read_raw = get_next_line(fd);
    if (!read_raw)
    {
        map->width = ft_strlen(read_raw);
        if (map->width == 0)
        {
            write(2, MAP_FORMAT_ZERO_WIDTH_ERR, ft_strlen(MAP_FORMAT_ZERO_WIDTH_ERR));
            return (0);
        }
        ++map->height;
    }
    else
    {
        write(2, MAP_FORMAT_EMPTY_ERR, ft_strlen(MAP_FORMAT_EMPTY_ERR);
        return (0);
    }

    while (1)
    {
        read_raw = get_next_line(fd);
        if (!read_raw)
            break;
        if (ft_strlen(read_raw) != map->width)
        {
            write(2, MAP_FORMAT_RECT_ERR, ft_strlen(MAP_FORMAT_RECT_ERR));
            free(read_raw);
            return (0);
        }
        free(read_raw);
        ++map->height;
    }

    // Allocating memory for the map
    map->matrix = (char **)malloc(map->height * sizeof (char *));
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
    }
    close(fd);
    return (1);
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
}
