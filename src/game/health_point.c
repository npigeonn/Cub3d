/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_point.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 09:30:57 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/17 14:48:46 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	life(t_game *game)
{
	int	x;
	int	y;
	
	game->y_hp = game->screen_height * 0.855;
	game->x_hp = game->screen_width * 0.583;
	y = game->y_hp;

	while (++y < game->screen_height * 0.88)
	{
		x = game->x_hp;
		while (++x < game->screen_width * (.583 + .279 * game->player->health))
			pixel_put(game, x, y, 16711680);
	}

}


void	health_point_draw(t_game *game)
{

	int	x;
	int	y;
	float	alpha = 0.5; // Ajuster cette valeur entre 0 et 1 pour changer l'opacité

	game->y_hp = game->screen_height * 0.85;
	game->x_hp = game->screen_width * 0.58;
	y = game->y_hp - 1;
	while (++y <= game->screen_height * 0.885)
	{
		x = game->x_hp - 1;
		while (++x <= game->screen_width * 0.865)
		{
			pixel_put(game, x, y,
				blend_colors(get_pixel_color_from_image(game, x, y),
				0x000000, alpha));
			if ((game->screen_width * 0.862 <= x && x <= game->screen_width * 0.865)
				|| (game->screen_width * 0.58 <= x && x <= game->screen_width * 0.583)
				|| (game->screen_height * 0.85 <= y && y <= game->screen_height * 0.855)
				|| (game->screen_height * 0.88 <= y && y <= game->screen_height * 0.885))
				pixel_put(game, x, y, 0x000000);
		}
	}
	life(game);
}
