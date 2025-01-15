/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:05:18 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/15 08:44:21 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_buton_option_click(t_game *game, int mouse_x, int mouse_y,
int info[4])
{
	const int	key_x1 = (info[0] - info[0] * 0.5) * 0.5 + info[0] * 0.24;
	const int	key_x2 = key_x1 + info[0] * 0.2;
	const int	key_height = game->screen_height * 0.03 + 10;

	if (mouse_x >= info[1] && mouse_x <= info[1] + info[0]
		&& mouse_y >= info[2] && mouse_y <= info[2] + key_height)
	{
		if (mouse_x >= key_x1 - 5 && mouse_x <= key_x1 + info[0] * 0.15)
			game->menu->text_field_selected = info[4] * 2 + 1;
		else if (mouse_x >= key_x2 - 5 && mouse_x <= key_x2 + info[0] * 0.15)
			game->menu->text_field_selected = info[4] * 2 + 2;
	}
}

void	update_option_menu_click(t_game *game, int mouse_x, int mouse_y,
int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->status == OPTIONS_KEYBOARD)
		update_option_menu_click_keyboard(game, mouse_x, mouse_y);
	if (game->menu->status == OPTIONS_MOUSE)
		update_mouse_options_interaction(game, mouse_x, mouse_y);
	if (game->menu->button_selected == 1)
		game->menu->status = OPTIONS_KEYBOARD;
	else if (game->menu->button_selected == 2)
		game->menu->status = OPTIONS_MOUSE;
	else if (game->menu->button_selected == 4)
		game->menu->status = game->menu->last_status;
	else
		return ;
	game->menu->button_selected = 0;
}

void	update_option_menu_button2(t_game *game, int mouse_x, int mouse_y,
int panel_y)
{
	const float	x = (game->screen_width - (game->screen_width * 0.9) * 0.25)
		* 0.5;
	const float	y = panel_y + (game->screen_height * 0.8) + 20;

	if (mouse_x >= x && mouse_x <= x + (game->screen_width * 0.9) * 0.25
		&& mouse_y >= y && mouse_y <= y + game->screen_height * 0.1)
		game->menu->button_selected = 4;
	if (game->menu->status == OPTIONS_MOUSE)
		update_option_menu_button_mouse(game, mouse_x, mouse_y);
}

void	update_option_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const float	panel_x = (game->screen_width - (game->screen_width * 0.9))
		* 0.5;
	const float	panel_y = (game->screen_height - (game->screen_height * 0.8))
		* 0.7 - game->screen_height * 0.1 - 20;
	const float	button_width = (game->screen_width * 0.9) / 2;
	const float	button_height = (game->screen_height * 0.8) * 0.095;

	game->menu->button_selected = 0;
	if (mouse_x >= panel_x && mouse_x <= panel_x + (game->screen_width * 0.9)
		&& mouse_y >= panel_y && mouse_y <= panel_y + button_height)
	{
		if (mouse_x <= panel_x + button_width)
			game->menu->button_selected = 1;
		else if (mouse_x > panel_x + button_width
			&& mouse_x <= panel_x + 2 * button_width)
			game->menu->button_selected = 2;
	}
	update_option_menu_button2(game, mouse_x, mouse_y, panel_y);
}
