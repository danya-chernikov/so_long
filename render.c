/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:04:17 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/21 02:45:51 by dchernik         ###   ########.fr       */
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
/* GOOD! */
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
/* GOOD! */
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
/* GOOD! */
void	draw_tiles(t_game_data *gdata, t_point *tl_tile, t_point *br_tile)
{
	t_point	m[2];

	check_tiles(gdata, tl_tile, br_tile, m);
	draw_tiles_loops(gdata, &m[0], &m[1]);
}

/* GOOD! */
void	check_tiles(t_game_data *gdata, t_point *tl_tile, t_point *br_tile, t_point *m)
{
	if (tl_tile->x < 0)
		m[MIN_TILE].x = 0;
	else
		m[MIN_TILE].x = tl_tile->x;
	if (tl_tile->y < 0)
		m[MIN_TILE].y = 0;
	else
		m[MIN_TILE].y = tl_tile->y;
	if (br_tile->x >= (int)gdata->map.width)
		m[MAX_TILE].x = (int)gdata->map.width - 1;
	else
		m[MAX_TILE].x = br_tile->x;
	if (br_tile->y >= (int)gdata->map.height)
		m[MAX_TILE].y = (int)gdata->map.height - 1;
	else
		m[MAX_TILE].y = br_tile->y;
}

/* GOOD! */
void	draw_tiles_loops(t_game_data *gdata, t_point *min_tile, t_point *max_tile)
{
	t_point	i;

	i.y = min_tile->y;
	while (i.y <= max_tile->y)
	{
		i.x = min_tile->x;
		while (i.x <= max_tile->x)
		{
			draw_tiles_inner_loop_body(gdata, &i);
			++i.x;
		}
		++i.y;
	}
}
