/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:42:57 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/19 15:36:52 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "libx/mlx.h"
#include "key_codes.h"
#include "map.h"

#define GAME_NAME				"LITTLE-DOLPHIN-PARTIZAN"

/* System error messages ( for using with perror() ) */
#define MEM_ALLOC_ERR_MSG		"malloc()"
#define FILE_OPEN_ERR_MSG		"open()"
#define FILE_READ_ERR_MSG		"read()"

/* If the window created from the map
 * resources has a width and/or height
 * smaller than this, we simply leave it
 * as it is and do nothing. If the user
 * wants to increase the window size,
 * they will see black areas filling the
 * empty space. If the window created from
 * the map resources is larger than these
 * defaults, map scrolling will appear */
#define WIN_DEFAULT_WIDTH		1920
#define WIN_DEFAULT_HEIGHT		1080
#define TILE_SIZE				128
#define MOVE_SPEED				1 /* pixels per frame */

/* MLX library error messages */
#define MLX_INIT_ERR_MSG		"Error\nmlx_init(): Failed to initialize mlx\n"
#define MLX_WIN_CREATE_ERR_MSG	"Error\nmlx_new_window(): Cannot create a window\n"
#define MLX_LOAD_IMG_ERR_MSG	"Error\nFaield to load image\n"

/* Exit codes
 *     ERROR         - denotes a general error (which may
 *                     also include a memory allocation error);
 *     MEM_ALLOC_ERR - specifically denotes a memory
 *                     allocation error, used when a
 *                     function can return multiple
 *                     error codes */
#define SUCCESS_CODE			1
#define ERROR_CODE				0
#define MEM_ALLOC_ERR_CODE		-1

#define BREAK					2

typedef enum	e_dir
{
	DIR_NONE = 0,
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT
}	t_dir;

typedef struct	s_img
{
	void	*img;
	int		width;
	int		height;
}	t_img;

/* ASK WHAT WHAT PURPOSES SERVES player_tx, player_ty, player_px, player_py */
/* mlx library resources */
typedef struct	s_game_data
{
	void	*mlx;
	void	*mlx_win;
	t_map	map; /* map_w = map.width; map_h = map_height */
	int		win_w; /* pixel actually created */
	int		win_h;

	t_img	sea;
	t_img	wall;
	t_img	mine;
	t_img	exit;
	t_img	dolphin_up;
	t_img	dolphin_right;
	t_img	dolphin_down;
	t_img	dolphin_left;

	int		player_tx; /* tile x */
	int		player_ty; /* tile y */
	int		player_px; /* pixel x, top-left relative to map */
	int		player_py; /* pixel y */

	t_dir	dir;
	int		moving; /* boolean */

	int		moves_count;
	int		collected_count;
	int		total_collectibles;

	/* Camera offset in pixels (top-left of what is drawn */
	int		cam_x;
	int		cam_y;
	
}	t_game_data;

/* game_logic.c */
int		game_init(t_game_data *gdata, const char *file_path);
int		game_cleanup(t_game_data *data);
int		load_image(t_game_data *gdata, t_img *out, const char *path);
void	find_player(t_game_data *gdata);
void	game_loop(t_game_data *gdata);

/* input.c */
void	start_moving(t_game_data *gdata, t_dir d);
int		key_down(int keycode, void *param);
int		key_up(int keycode, void *param);

/* render.c */
void	clamp_camera(t_game_data *gdata);
void	try_enter_tile(t_game_data *gdata, int nx, int ny);
void	update_player(t_game_data *gdata);
void	draw_tile(t_game_data *gdata, t_img *img, int px, int py);
int		render_frame(t_game_data *gdata);

#endif
