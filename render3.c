/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:52:09 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/21 04:06:42 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_logic.h"

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
	{
		if (max_cx > 0)
			gdata->cam_x = max_cx;
		else
			gdata->cam_x = 0;
	}
	if (gdata->cam_y > max_cy)
	{
		if (max_cy > 0)
			gdata->cam_y = max_cy;
		else
			gdata->cam_y = 0;
	}
}
