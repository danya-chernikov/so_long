/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:42:57 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/20 21:23:23 by dchernik         ###   ########.fr       */
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

#define TEXTURE_SEA_PATH		"textures/water/water_1_128x128.xpm"
#define TEXTURE_WALL_PATH		"textures/rocks/rock_1_128x128.xpm"
#define TEXTURE_MINE_PATH		"textures/mines/mine_128x128.xpm"
#define TEXTURE_EXIT_PATH		"textures/exit/skull_128x128.xpm"
#define TEXTURE_DOLP_UP_PATH	"textures/dolphins/dolphin_up_128x64.xpm"
#define TEXTURE_DOLP_RIGHT_PATH	"textures/dolphins/dolphin_right_128x64.xpm"
#define TEXTURE_DOLP_DOWN_PATH	"textures/dolphins/dolphin_down_128x64.xpm"
#define TEXTURE_DOLP_LEFT_PATH	"textures/dolphins/dolphin_left_128x64.xpm"

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
/* MLX library resources
 *     cam_x/cam_y - camera offset in
 *					 pixels (top-left
 *					 of what is drawn);
 *     win_w/win_h - pixels actually created. */
typedef struct	s_game_data
{
	void	*mlx;
	void	*mlx_win;
	int		win_w;
	int		win_h;

	t_map	map;

	t_img	sea;
	t_img	wall;
	t_img	mine;
	t_img	exit;
	t_img	dolphin_up;
	t_img	dolphin_right;
	t_img	dolphin_down;
	t_img	dolphin_left;

	t_point	player_tile;
	t_point	player_pixel;
	t_point	exit_coord;

	t_dir	dir;
	int		moving;

	int		moves_count;
	int		collected_count;
	int		total_collectibles;

	int		cam_x;
	int		cam_y;
	
}	t_game_data;

/* game_logic.c */
int		game_init(t_game_data *gdata, const char *file_path);
int		game_init_1(t_game_data *gdata, const char *map_path);
void	game_init_2(t_game_data *gdata);
void	get_window_size(t_game_data *gdata);
void	camera_init(t_game_data *gdata);

/* game_logic2.c */
int		game_cleanup(t_game_data *data);
int		load_image(t_game_data *gdata, t_img *out, const char *path);
int		load_images(t_game_data *gdata);
void	find_player(t_game_data *gdata);
void	find_exit(t_game_data *gdata);

/* input.c */
void	start_moving(t_game_data *gdata, t_dir d);
int		key_down(int keycode, void *param);
int		key_up(int keycode, void *param);

/* render.c */
int		render_frame(t_game_data *gdata);
void	update_player(t_game_data *gdata);
void	clamp_camera(t_game_data *gdata);
void	try_enter_tile(t_game_data *gdata, int nx, int ny);

#endif
