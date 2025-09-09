/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:50:38 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/09 16:50:40 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

#include "libx/mlx.h"

#include <stdlib.h>

int	main(int argc, char *argv[])
{
	void	*mlx;
	void	*mlx_win;
    t_map   map;

	if (argc != 2)
	{
		ft_printf("%s", "You need to specify the path to the game's map\n");
		exit(EXIT_FAILURE);
	}

    if (!map_init(&map, argv[1]))
    {
        return (EXIT_FAILURE);
    }
	map_check(&map);

	mlx = mlx_init();
	if (mlx == NULL)
	{
		ft_printf("%s", "mlx_init(): Failed to initialize mlx\n");
		exit(EXIT_FAILURE);
	}

	mlx_win = mlx_new_window(mlx, 1280, 720, "Just a window");
	if (mlx_win == NULL)
	{
		ft_printf("%s", "mlx_new_window(): Cannot create a window\n");
		exit(EXIT_FAILURE);
	}
	mlx_loop(mlx);

	return (EXIT_SUCCESS);
}
