/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:04:17 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/17 17:17:47 by dchernik         ###   ########.fr       */
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

	target_tx = gdata->player_tx;
	target_ty = gdata->player_ty;
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
	if (gdata->player_px < target_px)
		gdata->player_px += MOVE_SPEED;
	if (gdata->player_px > target_px)
		gdata->player_px -= MOVE_SPEED;
	if (gdata->player_py < target_py)
		gdata->player_py += MOVE_SPEED;
	if (gdata->player_py > target_py)
		gdata->player_py -= MOVE_SPEED;

	/* Clamp to target to avoid overshoot */
	if ((gdata->player_px > target_px - MOVE_SPEED) &&
		(gdata->player_px < target_px + MOVE_SPEED))
		gdata->player_px = target_px;
	if ((gdata->player_py > target_py - MOVE_SPEED) &&
		(gdata->player_py < target_py + MOVE_SPEED))
		gdata->player_py = target_py;

	/* If reached target tile center (aligned) */
	if (gdata->player_px == target_px && gdata->player_py == target_py)
	{
		/* Update tile coords */
		gdata->player_tx = target_tx;
		gdata->player_ty = target_ty;
		++gdata->moves_count;
		ft_printf("Moves: %d\n", gdata->moves_count);
		try_enter_tile(gdata, gdata->player_tx, gdata->player_ty);
	}

	/* Update camera to follow player */
	gdata->cam_x = gdata->player_px - gdata->win_w / 2 + TILE_SIZE / 2;
	gdata->cam_y = gdata->player_py - gdata->win_h / 2 + TILE_SIZE / 2;
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

	/* Determine visible tile range */
	int	x0 = gdata->cam_x / TILE_SIZE;
	int	y0 = gdata->cam_y / TILE_SIZE;
	int	x1 = (gdata->cam_x + gdata->win_w) / TILE_SIZE;
	int	y1 = (gdata->cam_y + gdata->win_h) / TILE_SIZE;
	if (x0 < 0)
		x0 = 0;
	if (y0 < 0)
		y0 = 0;
	if (x1 >= (int)gdata->map.width)
		x1 = gdata->map.width - 1;
	if (y1 >= (int)gdata->map.height)
		y1 = gdata->map.height - 1;

	/* Draw tiles */
	y = y0;	
	while (y <= y1)
	{
		x = x0;
		while (x <= x1)
		{
			px = x * TILE_SIZE;
			py = y * TILE_SIZE;
			/* Sea first */
			draw_tile(gdata, &gdata->sea, px, py);
			char t = gdata->map.matrix[y][x];
			if (t == '1')
				draw_tile(gdata, &gdata->wall, px, py);
			else if (t == 'C')
				draw_tile(gdata, &gdata->mine, px, py);
			else if (t == 'E')
			{
				/* Draw exit as sea for now - user may add special XPM */
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

	draw_tile(gdata, dimg, gdata->player_px, gdata->player_py);

	return (0);
}
