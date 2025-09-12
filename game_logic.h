#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "map.h"

#define GAME_NAME			"LITTLE-DOLPHIN-PARTIZAN"

/* System error messages ( for using with perror() ) */
#define MEM_ALLOC_ERR_MSG	"malloc()"
#define FILE_OPEN_ERR_MSG	"open()"
#define FILE_READ_ERR_MSG	"read()"

/* Exit codes
 *     ERROR         - denotes a general error (which may
 *                     also include a memory allocation error);
 *     MEM_ALLOC_ERR - specifically denotes a memory
 *                     allocation error, used when a
 *                     function can return multiple
 *                     error codes */
#define SUCCESS_CODE		1
#define ERROR_CODE          0
#define MEM_ALLOC_ERR_CODE	-1

#define BREAK				2

/* mlx library resources */
typedef struct	s_game_data
{
	void	*mlx;
	void	*mlx_win;
	t_map	map;
}	t_game_data;

#endif
