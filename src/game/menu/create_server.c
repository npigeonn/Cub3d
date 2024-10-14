/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_server.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:49:50 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/14 14:38:35 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	handle_text_input(char *field, int keycode)
{
	int len = strlen(field);

	if (keycode >= 32 && keycode <= 126)
	{
		if (len < 15 - 1)
		{
			field[len] = (char)keycode;
			field[len + 1] = '\0';
		}
	}
	else if (keycode == 65288 && len > 0)
		field[len - 1] = '\0';
}

void	update_create_server_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.1;
	const int spacing = game->screen_height * 0.05;
	const int x_pos = (game->screen_width - btn_width) * 0.5;
	const int y_pos = game->screen_height * 0.25;

	if (mouse_x >= x_pos && mouse_x <= x_pos + btn_width && mouse_y >= y_pos + 30 && mouse_y <= y_pos + 30 + btn_height)
		game->menu->button_selected = 1;
	else if (mouse_x >= x_pos && mouse_x <= x_pos + btn_width && mouse_y >= y_pos + btn_height + spacing + 30 && mouse_y <= y_pos + btn_height + spacing + 30 + btn_height)
		game->menu->button_selected = 2;
	else if (mouse_x >= x_pos && mouse_x <= x_pos + btn_width && mouse_y >= y_pos + 2 * (btn_height + spacing) && mouse_y <= y_pos + 2 * (btn_height + spacing) + btn_height)
		game->menu->button_selected = 3;
}

void	draw_text_field(t_game *game, int x, int y, int width, int height, char *text)
{
	draw_rectangle(game, x, y, width, height, MENU_BUTTON_COLOR);
	if (text)
		draw_text_left(game, text, x + 10, y + height / 2 - 10, height * 0.5, MENU_BUTTON_TEXT_COLOR);
}

void	draw_create_server_menu(t_game *game)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.1;
	const int spacing = game->screen_height * 0.05;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.25;

	draw_text(game, "Create Server", game->screen_width >> 1, y - 120, btn_height * 0.7, MENU_BUTTON_TEXT_COLOR);
	if (game->menu->button_selected == 1)
		draw_rectangle(game, x - 2, y + 30 - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_text(game, "Server Name", game->screen_width >> 1, y - 20, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_text_field(game, x, y + 30, btn_width, btn_height, game->server->name);
	if (game->menu->button_selected == 2)
		draw_rectangle(game, x - 2, y + btn_height + spacing + 30 - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_text(game, "Server IP", game->screen_width >> 1, y + btn_height + spacing - 20, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_text_field(game, x, y + btn_height + spacing + 30, btn_width, btn_height, "127.0.0.1");
	if (game->menu->button_selected == 3)
		draw_rectangle(game, x - 2, y + 2 * (btn_height + spacing) - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, x, y + 2 * (btn_height + spacing), btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Create", game->screen_width >> 1, y + 2 * (btn_height + spacing) + btn_height / 3 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
}

