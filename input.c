/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:04:15 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/20 13:11:28 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_logic.h"

/* If already moving, only change direction
 * (but keep continuous movement */
void	start_moving(t_game_data *gdata, t_dir d)
{
	gdata->dir = d;
	gdata->moving = 1;
}

int	key_down(int keycode, void *param)
{
	t_game_data	*gdata;
	
	gdata = (t_game_data *)param;
	if (!gdata)
		return (0);
	if (keycode == KEY_ESC)
		game_cleanup(gdata);
	else if (keycode == KEY_W)
		start_moving(gdata, DIR_UP);
	else if (keycode == KEY_D)
		start_moving(gdata, DIR_RIGHT);
	else if (keycode == KEY_S)
		start_moving(gdata, DIR_DOWN);
	else if (keycode == KEY_A)
		start_moving(gdata, DIR_LEFT);
	return (0);
}

/* We do not stop movement on key release */
int	key_up(int keycode, void *param)
{
	(void)keycode;
	(void)param;
	
	return (0);
}
