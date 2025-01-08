/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:09:02 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 18:01:50 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_sound_header(t_game *game)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	panel_x = (game->screen_width - panel_width) * 0.5;
	const float	panel_y = (game->screen_height - panel_height) * 0.5;
	t_draw_info	header_info;

	header_info = init_draw_info(
			game->screen_height * 0.06, "Sound Settings",
			panel_x + panel_width * 0.5, panel_y + 50 + panel_height * 0.1);
	header_info.color = 0xFFFFFF;
	draw_text(game, header_info);
}

void	draw_options_sound(t_game *game)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	slider_x = (panel_width - panel_width * 0.3)
		* 0.5 + (game->screen_width - panel_width) * 0.5;
	float		slider_y;
	int			i;

	draw_sound_header(game);
	i = -1;
	slider_y = (game->screen_height - panel_height) * 0.5 + panel_height * 0.4;
	while (++i < 3)
	{
		draw_sound_slider(game, i, slider_x, slider_y);
		slider_y += panel_height * 0.15;
	}
}
