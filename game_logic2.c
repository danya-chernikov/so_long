#include "game_logic.h"
#include <stdlib.h>

/* Note: mlx does not expose an easy way to free
 * images portably here; skipping for brevity */
/* GOOD! */
int	game_cleanup(t_game_data *gdata)
{
	if (!gdata)
		exit(EXIT_SUCCESS);
	if (gdata->mlx_win)
		mlx_destroy_window(gdata->mlx, gdata->mlx_win);
	map_matrix_free(&gdata->map);
	exit(EXIT_SUCCESS);
	return (SUCCESS_CODE);
}

/* GOOD! */
int	load_image(t_game_data *gdata, t_img *out, const char *path)
{
	out->img = mlx_xpm_file_to_image(gdata->mlx, (char *)path, &out->width, &out->height);
	if (!out->img)
	{
		write(STDERR_FILENO, MLX_LOAD_IMG_ERR_MSG, ft_strlen(MLX_LOAD_IMG_ERR_MSG));
		game_cleanup(gdata);
		return (ERROR_CODE);
	}
	return (SUCCESS_CODE);
}

/* GOOD! */
int	load_images(t_game_data *gdata)
{
	if (!load_image(gdata, &gdata->sea, TEXTURE_SEA_PATH))
		return (ERROR_CODE);
	if (!load_image(gdata, &gdata->wall, TEXTURE_WALL_PATH))
		return (ERROR_CODE);
	if (!load_image(gdata, &gdata->mine, TEXTURE_MINE_PATH))
		return (ERROR_CODE);
	if (!load_image(gdata, &gdata->exit, TEXTURE_EXIT_PATH))
		return (ERROR_CODE);
	if (!load_image(gdata, &gdata->dolphin_up, TEXTURE_DOLP_UP_PATH))
		return (ERROR_CODE);
	if (!load_image(gdata, &gdata->dolphin_right, TEXTURE_DOLP_RIGHT_PATH))
		return (ERROR_CODE);
	if (!load_image(gdata, &gdata->dolphin_down, TEXTURE_DOLP_DOWN_PATH))
		return (ERROR_CODE);
	if (!load_image(gdata, &gdata->dolphin_left, TEXTURE_DOLP_LEFT_PATH))
		return (ERROR_CODE);
	return (SUCCESS_CODE);
}

/* GOOD! */
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

/* GOOD! */
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
