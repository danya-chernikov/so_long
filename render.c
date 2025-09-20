/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:04:17 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/20 13:29:30 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_logic.h"

/* ASK ABOUT THIS */
void	clamp_camera(t_game_data *gdata)
{
	int	max_cx;
	int	max_cy;

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
}

void	try_enter_tile(t_game_data *gdata, int nx, int ny)
{
	char	tile;

	tile = gdata->map.matrix[ny][nx];
	if (nx < 0 || ny < 0 ||
		nx >= (int)gdata->map.width || ny >= (int)gdata->map.height)
		return ;
	if (tile == '1')
	{
		ft_printf("Game over: collided with wall after %d moves\n", gdata->moves_count);
		game_cleanup(gdata);
	}
	if (tile == 'E')
	{
		ft_printf("You reached the exit in %d moves\n", gdata->moves_count + 1);
		game_cleanup(gdata);
	}
	if (tile == 'C')
	{
		++gdata->collected_count;
		gdata->map.matrix[ny][nx] = '0';
		ft_printf("Collected: %d/%d\n", gdata->collected_count, gdata->total_collectibles);
	}
}

/* EXPLAIN THIS LOGIC */
void	update_player(t_game_data * gdata)
{
	int	target_tx;
	int	target_ty;
	int	target_px;
	int	target_py;

	if (!gdata->moving || gdata->dir == DIR_NONE)
		return ;

	target_tx = gdata->player_tile.x;
	target_ty = gdata->player_tile.y;
	if (gdata->dir == DIR_UP)
		target_ty -= 1;
	else if (gdata->dir == DIR_DOWN)
		target_ty += 1;
	else if (gdata->dir == DIR_LEFT)
		target_tx -= 1;
	else if (gdata->dir == DIR_RIGHT)
		target_tx += 1;

	/* Move player pixel position towards target tile */
	target_px = target_tx * TILE_SIZE;
	target_py = target_ty * TILE_SIZE;

	/* Before starting movement, check bounds: if target
	 * is a wall we still need to detect collision when
	 * reaching it */
	/* Move step */
	if ((int)gdata->player_pixel.x < target_px)
		gdata->player_pixel.x += MOVE_SPEED;
	if ((int)gdata->player_pixel.x > target_px)
		gdata->player_pixel.x -= MOVE_SPEED;
	if ((int)gdata->player_pixel.y < target_py)
		gdata->player_pixel.y += MOVE_SPEED;
	if ((int)gdata->player_pixel.y > target_py)
		gdata->player_pixel.y -= MOVE_SPEED;

	/* Clamp to target to avoid overshoot */
	if (((int)gdata->player_pixel.x > target_px - MOVE_SPEED) &&
		((int)gdata->player_pixel.x < target_px + MOVE_SPEED))
		gdata->player_pixel.x = target_px;
	if (((int)gdata->player_pixel.y > target_py - MOVE_SPEED) &&
		((int)gdata->player_pixel.y < target_py + MOVE_SPEED))
		gdata->player_pixel.y = target_py;

	/* If reached target tile center (aligned) */
	if ((int)gdata->player_pixel.x == target_px && (int)gdata->player_pixel.y == target_py)
	{
		/* Update tile coords */
		gdata->player_tile.x = target_tx;
		gdata->player_tile.y = target_ty;
		++gdata->moves_count;
		ft_printf("Moves: %d\n", gdata->moves_count);
		try_enter_tile(gdata, gdata->player_tile.x, gdata->player_tile.y);
	}

	/* Update camera to follow player */
	gdata->cam_x = gdata->player_pixel.x - gdata->win_w / 2 + TILE_SIZE / 2;
	gdata->cam_y = gdata->player_pixel.y - gdata->win_h / 2 + TILE_SIZE / 2;
	clamp_camera(gdata);
}

/* Draw at px,py relative to camera */
void	draw_tile(t_game_data *gdata, t_img *img, int px, int py)
{
	mlx_put_image_to_window(gdata->mlx, gdata->mlx_win, img->img,
		px - gdata->cam_x, py - gdata->cam_y);
    //mlx_put_image_to_window(g->mlx, g->win, img->img, px - g->cam_x, py - g->cam_y);
}

int	render_frame(t_game_data *gdata)
{
	int	x;
	int	y;
	int	px;
	int	py;

	if (!gdata)
		return (0);
	update_player(gdata);

	/* Determine visible tile range.
	 * Expanded visible tile range so
	 * we always cover the whole window */
	int	x0 = (gdata->cam_x / TILE_SIZE) - 1;
	int	y0 = (gdata->cam_y / TILE_SIZE) - 1;
	int	x1 = ((gdata->cam_x + gdata->win_w) / TILE_SIZE) + 1;
	int	y1 = ((gdata->cam_y + gdata->win_h) / TILE_SIZE) + 1;

	/* Draw sea everywhere inside that range (this fills margins) */
	y = y0;
	while (y <= y1)
	{
		x = x0;
		while (x <= x1)
		{
			px = x * TILE_SIZE;	
			py = y * TILE_SIZE;
			/* Sea tiled even outside map bounds
			 * (mlx allows negative coords) */
			mlx_put_image_to_window(gdata->mlx, gdata->mlx_win,
				gdata->sea.img, px - gdata->cam_x, py - gdata->cam_y);
			++x;
		}
		++y;
	}

	/* Draw objects only where tiles exist */
	int	mx0 = x0 < 0 ? 0 : x0;
	int	my0 = y0 < 0 ? 0 : y0;
	int	mx1 = x1 >= (int)gdata->map.width ? (int)gdata->map.width - 1 : x1;
	int	my1 = y1 >= (int)gdata->map.height ? (int)gdata->map.height - 1 : y1;

	/* Draw tiles */
	y = my0;	
	while (y <= my1)
	{
		x = mx0;
		while (x <= mx1)
		{
			px = x * TILE_SIZE;
			py = y * TILE_SIZE;
			char t = gdata->map.matrix[y][x];
			if (t == MAP_WALL_SYMBOL)
			{
				mlx_put_image_to_window(gdata->mlx, gdata->mlx_win,
					gdata->wall.img, px - gdata->cam_x, py - gdata->cam_y);
			}
			else if (t == MAP_COLLECT_SYMBOL)
			{
				mlx_put_image_to_window(gdata->mlx, gdata->mlx_win,
					gdata->mine.img, px - gdata->cam_x, py - gdata->cam_y);
			}
			else if (t == MAP_EXIT_SYMBOL)
			{
				mlx_put_image_to_window(gdata->mlx, gdata->mlx_win,
					gdata->exit.img, px - gdata->cam_x, py - gdata->cam_y);
			}
			++x;
		}
		++y;
	}

	/* Draw player at its pixel position */
	t_img *dimg = &gdata->dolphin_down;
	if (gdata->dir == DIR_UP)
		dimg = &gdata->dolphin_up;
	else if (gdata->dir == DIR_RIGHT)
		dimg = &gdata->dolphin_right;
	else if (gdata->dir == DIR_DOWN)
		dimg = &gdata->dolphin_down;
	else if (gdata->dir == DIR_LEFT)
		dimg = &gdata->dolphin_left;
	
	mlx_put_image_to_window(gdata->mlx, gdata->mlx_win, dimg->img,
		gdata->player_pixel.x - gdata->cam_x, gdata->player_pixel.y - gdata->cam_y);

	return (0);
}
