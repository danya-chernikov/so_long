/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:52:07 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/21 04:32:27 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_logic.h"

void	draw_tiles_inner_loop_body(t_game_data *gdata, t_point *i)
{
	t_point	p;
	char	t;

	p.x = i->x * TILE_SIZE;
	p.y = i->y * TILE_SIZE;
	t = gdata->map.matrix[i->y][i->x];
	if (t == MAP_WALL_SYMBOL)
	{
		mlx_put_image_to_window(gdata->mlx, gdata->mlx_win,
			gdata->wall.img, p.x - gdata->cam_x, p.y - gdata->cam_y);
	}
	else if (t == MAP_COLLECT_SYMBOL)
	{
		mlx_put_image_to_window(gdata->mlx, gdata->mlx_win,
			gdata->mine.img, p.x - gdata->cam_x, p.y - gdata->cam_y);
	}
	else if (t == MAP_EXIT_SYMBOL)
	{
		mlx_put_image_to_window(gdata->mlx, gdata->mlx_win,
			gdata->exit.img, p.x - gdata->cam_x, p.y - gdata->cam_y);
	}
}

/* Draw player at its pixel position */
void	draw_dolphin(t_game_data *gdata)
{
	t_img	*dimg;

	dimg = &gdata->dolphin_down;
	if (gdata->dir == DIR_UP)
		dimg = &gdata->dolphin_up;
	else if (gdata->dir == DIR_RIGHT)
		dimg = &gdata->dolphin_right;
	else if (gdata->dir == DIR_DOWN)
		dimg = &gdata->dolphin_down;
	else if (gdata->dir == DIR_LEFT)
		dimg = &gdata->dolphin_left;
	mlx_put_image_to_window(gdata->mlx, gdata->mlx_win, dimg->img,
		gdata->player_pixel.x - gdata->cam_x,
		gdata->player_pixel.y - gdata->cam_y);
}

/* Move player pixel position towards target tile.
 * Before starting movement, check bounds: if target
 * is a wall we still need to detect collision when
 * reaching it. Then it clamps to target to avoid
 * overshoot. If reached target tile center (aligned)
 * we set the logical tile to the target tile.
 * t_point	target_tile - target_tx and target_ty;
 * t_point	target_pos  - target_px and target_py */
void	update_player(t_game_data *gdata)
{
	t_point	target_tile;
	t_point	target_pos;

	ft_bzero(&target_tile, sizeof(target_tile));
	ft_bzero(&target_pos, sizeof(target_pos));
	move_player_towards_target(gdata, &target_tile, &target_pos);
	if (((int)gdata->player_pixel.x > target_pos.x - MOVE_SPEED)
		&& ((int)gdata->player_pixel.x < target_pos.x + MOVE_SPEED))
		gdata->player_pixel.x = target_pos.x;
	if (((int)gdata->player_pixel.y > target_pos.y - MOVE_SPEED)
		&& ((int)gdata->player_pixel.y < target_pos.y + MOVE_SPEED))
		gdata->player_pixel.y = target_pos.y;
	if ((int)gdata->player_pixel.x == target_pos.x
		&& (int)gdata->player_pixel.y == target_pos.y)
	{
		gdata->player_tile.x = target_tile.x;
		gdata->player_tile.y = target_tile.y;
		++gdata->moves_count;
		ft_printf("Moves: %d\n", gdata->moves_count);
		try_enter_tile(gdata, gdata->player_tile.x, gdata->player_tile.y);
	}
	gdata->cam_x = gdata->player_pixel.x - gdata->win_w / 2 + TILE_SIZE / 2;
	gdata->cam_y = gdata->player_pixel.y - gdata->win_h / 2 + TILE_SIZE / 2;
	clamp_camera(gdata);
}

void	move_player_towards_target(t_game_data *gdata,
			t_point *target_tile, t_point *target_pos)
{
	if (!gdata->moving || gdata->dir == DIR_NONE)
		return ;
	target_tile->x = gdata->player_tile.x;
	target_tile->y = gdata->player_tile.y;
	if (gdata->dir == DIR_UP)
		target_tile->y -= 1;
	else if (gdata->dir == DIR_DOWN)
		target_tile->y += 1;
	else if (gdata->dir == DIR_LEFT)
		target_tile->x -= 1;
	else if (gdata->dir == DIR_RIGHT)
		target_tile->x += 1;
	target_pos->x = target_tile->x * TILE_SIZE;
	target_pos->y = target_tile->y * TILE_SIZE;
	if ((int)gdata->player_pixel.x < target_pos->x)
		gdata->player_pixel.x += MOVE_SPEED;
	if ((int)gdata->player_pixel.x > target_pos->x)
		gdata->player_pixel.x -= MOVE_SPEED;
	if ((int)gdata->player_pixel.y < target_pos->y)
		gdata->player_pixel.y += MOVE_SPEED;
	if ((int)gdata->player_pixel.y > target_pos->y)
		gdata->player_pixel.y -= MOVE_SPEED;
}

void	try_enter_tile(t_game_data *gdata, int nx, int ny)
{
	char	tile;

	if (nx < 0 || ny < 0
		|| nx >= (int)gdata->map.width || ny >= (int)gdata->map.height)
		return ;
	tile = gdata->map.matrix[ny][nx];
	if (tile == MAP_WALL_SYMBOL)
	{
		ft_printf("Game over: collided with wall after %d moves\n",
			gdata->moves_count);
		game_cleanup(gdata);
	}
	if (tile == MAP_EXIT_SYMBOL)
	{
		ft_printf("You reached the exit in %d moves\n", gdata->moves_count);
		game_cleanup(gdata);
	}
	if (tile == MAP_COLLECT_SYMBOL)
	{
		++gdata->collected_count;
		gdata->map.matrix[ny][nx] = '0';
		ft_printf("Collected: %d/%d\n", gdata->collected_count,
			gdata->total_collectibles);
	}
}
