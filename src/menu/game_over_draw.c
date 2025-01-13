/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_over_draw.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:53:32 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/13 09:10:20 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	apply_fade_to(t_game *game, int color)
{
	int			x;
	int			y;
	int			clr;
	int			new_color;
	const float	alpha = game->fade_progress;

	y = 0;
	while (y < game->screen_height)
	{
		x = 0;
		while (x < game->screen_width)
		{
			clr = get_pixel_color_from_image(game, x, y);
			new_color = blend_colors(clr, color, alpha);
			pixel_put(game, x, y, new_color);
			x++;
		}
		y++;
	}
	game->fade_progress += 0.3 * game->delta_time;
}

void	draw_game_over_bg(t_game *game)
{
	t_draw_info	info;

	info = init_draw_info(game->screen_height, "", 0, 0);
	info.width = game->screen_width;
	info.color = 0x850606;
	draw_rectangle(game, info);
	x_fixes_cursor(game, 's');
}

void	draw_game_over_text(t_game *game)
{
	t_draw_info	info;

	info = init_draw_info(70, "You are dead.",
			game->screen_width >> 1, game->screen_height * 0.4);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}
