/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 14:56:34 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/12 19:05:41 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_H
# define GRAPHICS_H

#include "key_codes.h"

/* If the window created from the map
 * resources has a width and/or height
 * smaller than this, we simply leave it
 * as it is and do nothing. If the user
 * wants to increase the window size,
 * they will see black areas filling the
 * empty space. If the window created from
 * the map resources is larger than these
 * defaults, map scrolling will appear */
#define WIN_DEFAULT_WIDTH			1280
#define WIN_DEFAULT_HEIGHT			720

/* MLX library error messages */
#define MLX_INIT_ERR_MSG			"Error\nmlx_init(): Failed to initialize mlx\n"
#define MLX_WIN_CREATE_ERR_MSG		"Error\nmlx_new_window(): Cannot create a window\n"

/* colors.c */
int	create_trgb(int t, int r, int g, int b);
int	get_t(int trgb);
int	get_r(int trgb);
int	get_g(int trgb);
int	get_b(int trgb);

#endif
