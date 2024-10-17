/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ammo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:09:22 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/17 15:34:51 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	ammo_written(t_game *game)
{
	char *str;
	t_image	*im;
	
	im = malloc(sizeof(t_image));
	str = ft_itoa(game->player->ammo);
	draw_text_right(game, str, game->screen_width * 0.82,
		game->screen_height * 0.885, game->screen_height * 0.05, 16777215);
	load_texture(game, im, "assets/sprites/bullet.xpm");
	draw_image(game, game->screen_width * 0.81, game->screen_height * 0.875,
		game->screen_height * 0.06, im);
	free(str);
}
