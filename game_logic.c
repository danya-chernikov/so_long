#include "game_logic.h"
#include <stdlib.h>

int	game_init(t_game_data *gdata, const char *map_path)
{
	int	map_px_w;
	int	map_px_h;
	int	max_cx;
	int	max_cy;

	ft_memset(gdata, '\0', sizeof (gdata));
	if (!map_init(&gdata->map, map_path) || !map_check(&gdata->map))
		return (ERROR_CODE);
	gdata->total_collectibles = map_get_collect_num(&gdata->map);
	map_print(&gdata->map);
	gdata->mlx = mlx_init();
	if (!gdata->mlx)
	{
		write(STDERR_FILENO, MLX_INIT_ERR_MSG, ft_strlen(MLX_INIT_ERR_MSG));
		return (ERROR_CODE);
	}

	/* Compute desired window size in pixels */
	map_px_w = gdata->map.width * TILE_SIZE;
	map_px_h = gdata->map.height * TILE_SIZE;
	gdata->win_w = (map_px_w <= WIN_DEFAULT_WIDTH) ? map_px_w : WIN_DEFAULT_WIDTH;
	gdata->win_h = (map_px_h <= WIN_DEFAULT_HEIGHT) ? map_px_h : WIN_DEFAULT_HEIGHT;

	gdata->mlx_win = mlx_new_window(gdata->mlx, gdata->win_w,
			gdata->win_h, GAME_NAME);
	if (!gdata->mlx_win)
	{
		write(STDERR_FILENO, MLX_WIN_CREATE_ERR_MSG,
			ft_strlen(MLX_WIN_CREATE_ERR_MSG));
		return (ERROR_CODE);
	}
	
	if (!load_images(gdata))
		return (ERROR_CODE);

	find_player(gdata);

	gdata->dir = DIR_NONE;
	gdata->moving = 0;
	gdata->moves_count = 0;
	gdata->collected_count = 0;

	/* Initial camera */
	gdata->cam_x = gdata->player_px - gdata->win_w / 2 + TILE_SIZE / 2;
	gdata->cam_y = gdata->player_py - gdata->win_h / 2 + TILE_SIZE / 2;
	if (gdata->cam_x < 0)
		gdata->cam_x = 0;
	if (gdata->cam_y < 0)
		gdata->cam_y = 0;
	max_cx = gdata->map.width * TILE_SIZE - gdata->win_w;
	max_cy = gdata->map.height * TILE_SIZE - gdata->win_h;
	if (gdata->cam_x > max_cx)
		gdata->cam_x = (max_cx > 0 ? max_cx : 0);
	if (gdata->cam_y > max_cy)
		gdata->cam_y = (max_cy > 0 ? max_cy : 0);

	/* Hooks */
	mlx_hook(gdata->mlx_win, 2, 1L<<0, key_down, gdata);
	mlx_hook(gdata->mlx_win, 3, 1L<<1, key_up, gdata);
	mlx_hook(gdata->mlx_win, 17, 0, (int (*)(void *))game_cleanup, gdata);
	mlx_loop_hook(gdata->mlx, (int (*)(void *))render_frame, gdata);

	return (SUCCESS_CODE);
}

/* Note: mlx does not expose an easy way to free
 * images portably here; skipping for brevity */
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
			if (gdata->map.matrix[y][x] == 'P')
			{
				gdata->player_tx = x;
				gdata->player_ty = y;
				gdata->player_px = x * TILE_SIZE;
				gdata->player_py = y * TILE_SIZE;
				/* replace P with empty floor */
				gdata->map.matrix[y][x] = '0';
				return ;
			}
			++x;
		}
		++y;
	}
}

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
