/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crosshair.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:16:38 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/15 12:06:45 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	crosshair(t_game *game)
{
	int		x;
	int		y;
	float	alpha;
	int 	cross_len;
	int		cross_width;

	cross_len = game->screen_height * 0.02;
	cross_width = game->screen_height * 0.005;
	alpha = 0.6; // Ajuster cette valeur entre 0 et 1 pour changer l'opacité

	y = game->screen_height / 2 - cross_len - 1;
	while (++y <= game->screen_height / 2 + cross_len)
	{
		x = game->screen_width / 2 - cross_len - 1;
		while (++x <= game->screen_width / 2 + cross_len)
			if (((game->screen_width - cross_width) / 2 <= x
				&& x <= (game->screen_width + cross_width) / 2)
				|| ((game->screen_height - cross_width) / 2 <= y
				&& y <= (game->screen_height + cross_width) / 2))
				// || ((game->screen_height + cross_len) * (game->screen_height + cross_len) >= x * x + y * y))
				// && x * x + y * y <= (game->screen_height - 0.75 * cross_len) * (game->screen_height - 0.75 * cross_len)))
				pixel_put(game, x, y,
				blend_colors(get_pixel_color_from_image(game, x, y),
				9830400, alpha));
	}
}

void	hp(t_game *game)
{

	// int	x;
	// int	y;
	// int	background_color;
	// int	grey_with_alpha;
	// float	alpha = 0.8; // Ajuster cette valeur entre 0 et 1 pour changer l'opacité

	// int grey = 0x000000; // Gris
	// game->y_minimap = game->screen_height - game->screen_height / 4;
	// game->x_minimap = game->screen_width - game->screen_width / 8;
	// y = game->y_minimap;
	// while (y <= game->screen_height)
	// {
	// 	x = game->x_minimap;
	// 	while (x <= game->screen_width)
	// 	{
	// 		background_color = get_pixel_color_from_image(game, x, y);
	// 		grey_with_alpha = blend_colors(background_color, grey, alpha);
	// 		pixel_put(game, x, y, grey_with_alpha);
	// 		x++;
	// 	}
	// 	y++;
	// }
}

void	gun_draw(t_game *game)
{
	

}