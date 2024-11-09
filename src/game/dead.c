/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:45:02 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/09 14:20:30 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_dead_screen_multiplayer(t_game *game)
{
	int		x;
	int		y;
	int		original_color;
	int		grey_color;

	y = -1;
	while (++y < game->screen_height)
	{
		x = -1;
		while (++x < game->screen_width)
		{
			original_color = get_pixel_color_from_image(game, x, y);
			int red = (original_color >> 16) & 0xFF;
			int green = (original_color >> 8) & 0xFF;
			int blue = original_color & 0xFF;
			int grey_value = (red * 0.3) + (green * 0.59) + (blue * 0.11);
			grey_color = (grey_value << 16) | (grey_value << 8) | grey_value;
			int final_color = blend_colors(original_color, grey_color, 0.7);
			pixel_put(game, x, y, final_color);
		}
	}
}

