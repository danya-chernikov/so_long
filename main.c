/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:50:38 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/12 18:54:28 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "key_codes.h"
#include "game_logic.h"
#include "libx/mlx.h"
#include <stdlib.h>

int	close_win_esc(int keycode, t_game_data *gdata)
{
	if (keycode == KEY_ESC)
		mlx_destroy_window(gdata->mlx, gdata->mlx_win);
	map_matrix_free(&gdata->map);
	exit (EXIT_SUCCESS);
	return (0);
}

int	close_win_redcross(t_game_data *gdata)
{
	map_matrix_free(&gdata->map);
	exit (EXIT_SUCCESS);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game_data	gdata;

	if (argc != 2)
	{
		write(STDERR_FILENO, MAP_PATH_ERR_MSG, ft_strlen(MAP_PATH_ERR_MSG));
		exit(EXIT_FAILURE);
	}
	if (!map_init(&gdata.map, argv[1]) || !map_check(&gdata.map))
		return (EXIT_FAILURE);
	map_print(&gdata.map);

	gdata.mlx = mlx_init();
	if (!gdata.mlx)
	{
		write(STDERR_FILENO, MLX_INIT_ERR_MSG, ft_strlen(MLX_INIT_ERR_MSG));
		exit(EXIT_FAILURE);
	}
	gdata.mlx_win = mlx_new_window(gdata.mlx, WIN_DEFAULT_WIDTH,
		WIN_DEFAULT_HEIGHT, GAME_NAME);
	if (!gdata.mlx_win)
	{
		write(STDERR_FILENO, MLX_WIN_CREATE_ERR_MSG,
			ft_strlen(MLX_WIN_CREATE_ERR_MSG));
		exit(EXIT_FAILURE);
	}
	mlx_hook(gdata.mlx_win, 2, 1L<<0, close_win_esc, &gdata);
	mlx_hook(gdata.mlx_win, 17, 0, close_win_redcross, &gdata);
	mlx_loop(gdata.mlx);
    map_matrix_free(&gdata.map);
	return (EXIT_SUCCESS);
}
