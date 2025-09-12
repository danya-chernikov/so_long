#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "map.h"

/* mlx library resources */
typedef struct	s_game_data
{
	void	*mlx;
	void	*mlx_win;
	t_map	map;
}	t_game_data;

#endif
