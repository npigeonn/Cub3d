/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ammo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:09:22 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/21 00:02:11 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	ammo_written(t_game *game)
{
	char		*str;
	t_image		*im;
	t_draw_info	info;
	
	im = malloc(sizeof(t_image));
	str = ft_itoa(game->player->ammo);
	info = init_draw_info( game->screen_height * 0.05, str,  game->screen_width * 0.82, game->screen_height * 0.885);
	info.color = 16777215;
	draw_text_right(game, info);
	info = init_draw_info( game->screen_height * 0.06, "",  game->screen_width * 0.81, game->screen_height * 0.875);
	draw_image(game, game->textures->bullet, info);
	free(str);
}
