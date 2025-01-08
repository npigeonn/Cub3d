/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:56:43 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 22:19:23 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_options_bg(t_game *game)
{
	const float	rect_width = game->screen_width * 0.9;
	const float	rect_height = game->screen_height * 0.8;
	const float	rect_x = (game->screen_width - rect_width) * 0.5;
	const float	rect_y = (game->screen_height - rect_height)
		* 0.7 - game->screen_height * 0.1 - 20;
	t_draw_info	rect_info;

	rect_info = init_draw_info(rect_height, "", rect_x, rect_y);
	rect_info.color = MENU_BUTTON_COLOR;
	rect_info.width = rect_width;
	rect_info.radius = 25;
	draw_rounded_rectangle(game, rect_info);
}

void	draw_section_separator(t_game *game, float x, float y, float height)
{
	t_draw_info	line_info;

	line_info = init_draw_info(2, "", x, y);
	line_info.width = 2;
	line_info.height = height;
	line_info.color = 0xFFFFFF;
	draw_rectangle(game, line_info);
}

void	draw_options_all_buttons(t_game *game, int start_y, int button_height)
{
	const float	section_width = game->screen_width * 0.9 / 3;
	const float	rect_x = (game->screen_width - game->screen_width * 0.9) * 0.5;
	int			i;

	i = -1;
	while (++i < 3)
	{
		draw_options_button(game, rect_x + section_width * i,
			start_y - button_height * 0.5, i);
		if (i < 3 - 1)
			draw_section_separator(game, rect_x + section_width * (i + 1),
				23, game->screen_height * 0.8 * 0.095);
	}
}

void	draw_options_menu(t_game *game)
{
	const float	rect_x = (game->screen_width - game->screen_width * 0.9) * 0.5;
	const float	rect_y = (game->screen_height - game->screen_height * 0.8)
		* 0.7 - game->screen_height * 0.1 - 20;
	const float	button_height = game->screen_height * 0.05;
	const float	start_y = rect_y + game->screen_height * 0.8 * 0.03;
	t_draw_info	line_info;

	draw_options_bg(game);
	line_info = init_draw_info(2, "", rect_x, start_y + button_height);
	line_info.width = game->screen_width * 0.9;
	line_info.color = 0xFFFFFF;
	draw_rectangle(game, line_info);
	draw_options_all_buttons(game, start_y, button_height);
	if (game->menu->status == OPTIONS_KEYBOARD)
		draw_options_keyboard(game);
	else if (game->menu->status == OPTIONS_MOUSE)
		draw_options_mouse(game);
	else if (game->menu->status == OPTIONS_SOUND)
		draw_options_sound(game);
	draw_back_option_buttons(game);
}
