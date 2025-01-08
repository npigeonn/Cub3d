/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:31:59 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 16:13:07 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_server_name(t_game *game, int keycode, int len, char *field)
{
	if (keycode >= 65429 && keycode <= 65439)
		field[len] = get_keycode_pad(keycode);
	else if (keycode == 32)
		field[len] = '_';
	else if (is_key_pressed(game, KEY_SHIFT))
		field[len] = ft_toupper((char)keycode);
	else
		field[len] = (char)keycode;
	field[len + 1] = '\0';
}

static void	update_create_server_menu_text2(t_game *game, int keycode)
{
	if (game->menu->button_selected == 1 && keycode == 1)
	{
		game->client->name[0] = '\0';
		game->menu->status = SERVERS;
	}
	if (game->menu->button_selected == 2 && keycode == 1)
		handle_create_server(game);
	if (game->menu->button_selected == 5 && keycode == 1)
	{
		game->menu->status = OPTIONS_KEYBOARD;
		game->menu->last_status = SERVER_CREATE;
	}
}

void	update_create_server_menu_text(t_game *game, int mouse_x, int mouse_y,
int keycode)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.33;

	if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + 30
		&& mouse_y <= y + 30 + btn_height)
	{
		if (keycode == 1)
		{
			game->menu->text_field_selected = 1;
			game->menu->error_name = false;
		}
	}
	else
		game->menu->text_field_selected = 0;
	update_create_server_menu_text2(game, keycode);
}

void	update_create_server_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_height = game->screen_height * 0.08;
	const int	btn_h_width = game->screen_width * 0.25 * 0.45;
	const int	t_btn_width = 2 * btn_h_width + game->screen_width * 0.02;
	const int	btn_x_start = (game->screen_width - t_btn_width) / 2;
	const int	btn_y_start = game->screen_height * 0.33 + 2
		* (btn_height + game->screen_height * 0.06);

	if (mouse_x >= btn_x_start && mouse_x <= btn_x_start + btn_h_width
		&& mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
		game->menu->button_selected = 1;
	else if (mouse_x >= btn_x_start + btn_h_width + game->screen_width * 0.02
		&& mouse_x <= btn_x_start + 2 * btn_h_width + game->screen_width * 0.02
		&& mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
		game->menu->button_selected = 2;
	else
		game->menu->button_selected = 0;
	check_mouse_on_gear(game, mouse_x, mouse_y);
}
