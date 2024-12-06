/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:45:02 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/27 15:18:57 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_dead_screen_multiplayer(t_game *game)
{
	int		x;
	int		y;
	int		original_color;
	int		grey_color;
	int		grey_value;

	y = -1;
	while (++y < game->screen_height)
	{
		x = -1;
		while (++x < game->screen_width)
		{
			original_color = get_pixel_color_from_image(game, x, y);
			grey_value = (((original_color >> 16) & 0xFF) * 0.3)
				+ (((original_color >> 8) & 0xFF) * 0.59)
				+ ((original_color & 0xFF) * 0.11);
			grey_color = (grey_value << 16) | (grey_value << 8) | grey_value;
			pixel_put(game, x, y,
				blend_colors(original_color, grey_color, 0.7));
		}
	}
}
