/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:52:01 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/21 04:53:39 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_logic.h"
#include <stdlib.h>

int	game_init(t_game_data *gdata, const char *map_path)
{
	if (!game_init_1(gdata, map_path))
		return (ERROR_CODE);
	game_init_2(gdata);
	return (SUCCESS_CODE);
}

int	game_init_1(t_game_data *gdata, const char *map_path)
{
	ft_memset(gdata, '\0', sizeof (gdata));
	if (!map_init(&gdata->map, map_path) || !map_check(&gdata->map))
		return (ERROR_CODE);
	gdata->total_collectibles = map_get_collect_num(&gdata->map);
	gdata->mlx = mlx_init();
	if (!gdata->mlx)
	{
		write(STDERR_FILENO, MLX_INIT_ERR_MSG, ft_strlen(MLX_INIT_ERR_MSG));
		return (ERROR_CODE);
	}
	get_window_size(gdata);
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
	if (!map_check_reachability(gdata))
		return (ERROR_CODE);
	return (SUCCESS_CODE);
}

void	game_init_2(t_game_data *gdata)
{
	gdata->dir = DIR_NONE;
	gdata->moving = 0;
	gdata->moves_count = 0;
	gdata->collected_count = 0;
	camera_init(gdata);
	mlx_hook(gdata->mlx_win, 2, 1L << 0, key_down, gdata);
	mlx_hook(gdata->mlx_win, 3, 1L << 1, key_up, gdata);
	mlx_hook(gdata->mlx_win, 17, 0, (int (*)(void *))game_cleanup, gdata);
	mlx_loop_hook(gdata->mlx, (int (*)(void *))render_frame, gdata);
}

/* Computes desired window size in pixels */
void	get_window_size(t_game_data *gdata)
{
	int	map_px_w;
	int	map_px_h;

	map_px_w = gdata->map.width * TILE_SIZE;
	map_px_h = gdata->map.height * TILE_SIZE;
	if (map_px_w <= WIN_DEFAULT_WIDTH)
		gdata->win_w = map_px_w;
	else
		gdata->win_w = WIN_DEFAULT_WIDTH;
	if (map_px_h <= WIN_DEFAULT_HEIGHT)
		gdata->win_h = map_px_h;
	else
		gdata->win_h = WIN_DEFAULT_HEIGHT;
}

/* Initializes the camera. Ensures the
 * camera is centered on the player and
 * the camera does not scroll outside
 * the map boundaries */
void	camera_init(t_game_data *gdata)
{
	int	max_cx;
	int	max_cy;

	gdata->cam_x = gdata->player_pixel.x - gdata->win_w / 2 + TILE_SIZE / 2;
	gdata->cam_y = gdata->player_pixel.y - gdata->win_h / 2 + TILE_SIZE / 2;
	if (gdata->cam_x < 0)
		gdata->cam_x = 0;
	if (gdata->cam_y < 0)
		gdata->cam_y = 0;
	max_cx = gdata->map.width * TILE_SIZE - gdata->win_w;
	max_cy = gdata->map.height * TILE_SIZE - gdata->win_h;
	if (gdata->cam_x > max_cx)
	{
		if (max_cx > 0)
			gdata->cam_x = max_cx;
		else
			gdata->cam_x = 0;
	}
	if (gdata->cam_y > max_cy)
	{
		if (max_cy > 0)
			gdata->cam_y = max_cy;
		else
			gdata->cam_y = 0;
	}
}
