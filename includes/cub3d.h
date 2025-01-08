/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:05:51 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/03 16:15:04 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include "mlx.h"
# include <stdlib.h>
# include <stdio.h>
# include <stdlib.h>

# include "sprite.h"
# include "raycaster.h"
# include "game.h"
# include "server.h"
# include "chat.h"
# include "client.h"
# include "input.h"
# include "stats.h"
# include "multithread.h"
# include "menu.h"

# include <X11/X.h>
# include <X11/keysym.h>
# include <X11/extensions/Xfixes.h>
# include "mlx_int.h"

# define MOVEMENT_SPEED 0.05f
# define ROTATION_SPEED 0.001f

# define MENU_BUTTON_COLOR 0x002B36 
# define MENU_BUTTON_SELECTED_COLOR 0x2F4F4F
# define MENU_BUTTON_TEXT_COLOR 0xf2e9e9

void	destroy_mlx_img(t_block_info *params);
void	destroy_mlx_display(t_block_info *params);

#endif