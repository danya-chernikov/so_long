#include "game_logic.h"
#include <stdlib.h>

void	find_player(t_game_data *gdata)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (y < gdata->map.height)
	{
		x = 0;	
		while (x < gdata->map.width)
		{
			if (gdata->map.matrix[y][x] == MAP_PLAYER_POS_SYMBOL)
			{
				gdata->player_tile.x = x;
				gdata->player_tile.y = y;
				gdata->player_pixel.x = x * TILE_SIZE;
				gdata->player_pixel.y = y * TILE_SIZE;
				gdata->map.matrix[y][x] = MAP_SEA_SYMBOL;
				return ;
			}
			++x;
		}
		++y;
	}
}

void	find_exit(t_game_data *gdata)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (y < gdata->map.height)
	{
		x = 0;	
		while (x < gdata->map.width)
		{
			if (gdata->map.matrix[y][x] == MAP_EXIT_SYMBOL)
			{
				gdata->exit_coord.x = x;
				gdata->exit_coord.y = y;
			}
			++x;
		}
		++y;
	}
}
