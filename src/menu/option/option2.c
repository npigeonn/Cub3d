/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:42:53 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/15 08:20:56 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_options_button2(t_game *game, const float text_x, int i)
{
	t_draw_info	info;
	const char	*labels[2] = {"Keyboard", "Mouse"};

	info = init_draw_info(game->screen_height * 0.8 * 0.095,
			(char *)labels[i], text_x, 30);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, info);
}

void	draw_options_button(t_game *game, float x, float y, int i)
{
	t_draw_info	info;
	const char	*labels[2] = {"Keyboard", "Mouse"};
	const float	text_width = get_text_width(game, (char *)labels[i],
			(int)(game->screen_height * 0.8 * 0.095));
	const float	button_width = (game->screen_width * 0.9) / 2;
	const float	text_x = x + (button_width - text_width) * 0.26;

	info = init_draw_info(game->screen_height * 0.8 * 0.095 - 3,
			(char *)labels[i], x - 157, y + 3);
	info.width = (int)button_width;
	if (game->menu->status == (OPTIONS_KEYBOARD + i))
		info.color = MENU_BUTTON_SELECTED_COLOR;
	else if (game->menu->button_selected == (i + 1))
		info.color = MENU_BUTTON_COLOR & 0x666666;
	else
		info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, info);
	draw_options_button2(game, text_x, i);
}
