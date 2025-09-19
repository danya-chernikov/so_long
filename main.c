/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:50:38 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/19 14:39:05 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_logic.h"
#include <stdlib.h>

/* To start the game, press any
 * valid key to move your dolphin
 * in the desired direction
 * (W–up, D–right, S–down, A–left) */
int	main(int argc, char *argv[])
{
	t_game_data	gdata;

	if (argc != 2)
	{
		write(STDERR_FILENO, MAP_PATH_ERR_MSG, ft_strlen(MAP_PATH_ERR_MSG));
		exit(EXIT_FAILURE);
	}
	ft_bzero(&gdata, sizeof (t_game_data));
	if (!game_init(&gdata, argv[1]))
	{
		exit(EXIT_FAILURE);
	}
	mlx_loop(gdata.mlx);
	game_cleanup(&gdata);
	return (EXIT_SUCCESS);
}
