/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:04:17 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/20 23:30:29 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_logic.h"

/* px and py - world-space coordinates
 * of a tile in pixels, measured form
 * the map's origin (0,0 at top-left
 * of the map);
 * tl_tile - top-left tile; x0 and y0
 * br_tile - bottom-right tile. x1 and y1
 * */
int	render_frame(t_game_data *gdata)
{
	t_point	tl_tile;
	t_point	br_tile;

	if (!gdata)
		return (0);
	update_player(gdata);
	draw_sea(gdata, &tl_tile, &br_tile);
	draw_tiles(gdata, &tl_tile, &br_tile);
	draw_dolphin(gdata);
	return (0);
}

/* Determine visible tile range. Expanded visible tile range so we always cover the
 * whole window. Then draw sea everywhere inside that range (this fills margins).
 * Sea tiled even outside map bounds (mlx allows negative coords).
 *     t_point	i; x and y
 *     t_point	p; px and py
 * */
void	draw_sea(t_game_data *gdata, t_point *tl_tile, t_point *br_tile)
{
	t_point	i;
	t_point	p;

	tl_tile->x = (gdata->cam_x / TILE_SIZE) - 1;
	tl_tile->y = (gdata->cam_y / TILE_SIZE) - 1;
	br_tile->x = ((gdata->cam_x + gdata->win_w) / TILE_SIZE) + 1;
	br_tile->y = ((gdata->cam_y + gdata->win_h) / TILE_SIZE) + 1;
	i.y = tl_tile->y;
	while (i.y <= br_tile->y)
	{
		i.x = tl_tile->x;
		while (i.x <= br_tile->x)
		{
			p.x = i.x * TILE_SIZE;	
			p.y = i.y * TILE_SIZE;
			mlx_put_image_to_window(gdata->mlx, gdata->mlx_win,
				gdata->sea.img, p.x - gdata->cam_x, p.y - gdata->cam_y);
			++i.x;
		}
		++i.y;
	}
}

/* Draw objects only where tiles exist and then draw tiles.
 * min_tile - mx0 and my0;
 * max_tile - mx1 and my1.
 * t_point	m[2] - m[0]=min_tile; m[1]=max_tile */
void	draw_tiles(t_game_data *gdata, t_point *tl_tile, t_point *br_tile)
{
	t_point	m[2];

	check_tiles(gdata, tl_tile, br_tile, m);
	draw_tiles_loops(gdata, &m[0], &m[1]);
}

void	check_tiles(t_game_data *gdata, t_point *tl_tile, t_point *br_tile, t_point *m)
{
	if (tl_tile->x < 0)
		m[0].x = 0;
	else
		m[0].x = tl_tile->x;
	if (tl_tile->y < 0)
		m[0].y = 0;
	else
		m[0].y = tl_tile->y;
	if (br_tile->x >= (int)gdata->map.width)
		m[1].x = (int)gdata->map.width - 1;
	else
		m[1].x = br_tile->x;
	if (br_tile->y >= (int)gdata->map.height)
		m[1].y = (int)gdata->map.height - 1;
	else
		m[1].y = br_tile->y;
}

void	draw_tiles_loops(t_game_data *gdata, t_point *min_tile, t_point *max_tile)
{
	t_point	p;
	t_point	i;

	i.y = min_tile->y;
	while (i.y <= max_tile->y)
	{
		i.x = min_tile->x;
		while (i.x <= max_tile->x)
		{
			p.x = i.x * TILE_SIZE;
			p.y = i.y * TILE_SIZE;
			char t = gdata->map.matrix[i.y][i.x];
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
			++i.x;
		}
		++i.y;
	}
}

/* Draw player at its pixel position */
void	draw_dolphin(t_game_data *gdata)
{
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
		gdata->player_pixel.x - gdata->cam_x,
		gdata->player_pixel.y - gdata->cam_y);
}

/* EXPLAIN THIS LOGIC */
void	update_player(t_game_data * gdata)
{
	if (!gdata->moving || gdata->dir == DIR_NONE)
		return ;

	int	target_tx;
	int	target_ty;

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

	int	target_px;
	int	target_py;

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

/* GOOD! */
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

/* GOOD! */
void	try_enter_tile(t_game_data *gdata, int nx, int ny)
{
	char	tile;

	tile = gdata->map.matrix[ny][nx];
	if (nx < 0 || ny < 0 ||
		nx >= (int)gdata->map.width || ny >= (int)gdata->map.height)
		return ;
	if (tile == MAP_WALL_SYMBOL)
	{
		ft_printf("Game over: collided with wall after %d moves\n",
			gdata->moves_count);
		game_cleanup(gdata);
	}
	if (tile == MAP_EXIT_SYMBOL)
	{
		ft_printf("You reached the exit in %d moves\n", gdata->moves_count + 1);
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
