/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:50:38 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/11 20:12:04 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libx/mlx.h"
#include <stdlib.h>

int	main(int argc, char *argv[])
{
	void	*mlx;
	void	*mlx_win;
	t_map	map;

	if (argc != 2)
	{
		write(STDERR_FILENO, MAP_PATH_ERR_MSG, ft_strlen(MAP_PATH_ERR_MSG));
		exit(EXIT_FAILURE);
	}
	if (!map_init(&map, argv[1]))
	{
		return (EXIT_FAILURE);
	}
	if (!map_check(&map))
		return (EXIT_FAILURE);

	map_print(&map);

	mlx = mlx_init();
	if (!mlx)
	{
		write(STDERR_FILENO, MLX_INIT_ERR_MSG, ft_strlen(MLX_INIT_ERR_MSG));
		exit(EXIT_FAILURE);
	}
	mlx_win = mlx_new_window(mlx, WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, GAME_NAME);
	if (!mlx_win)
	{
		write(STDERR_FILENO, MLX_WIN_CREATE_ERR_MSG,
			ft_strlen(MLX_WIN_CREATE_ERR_MSG));
		exit(EXIT_FAILURE);
	}
	mlx_loop(mlx);
    map_matrix_free(&map);
	return (EXIT_SUCCESS);
}
