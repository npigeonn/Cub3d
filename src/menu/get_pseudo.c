/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pseudo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:28:40 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/27 16:57:32 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	handle_pseudo_input(t_game *game, int keycode)
{
	char	*field;
	int		len;

	field = game->client->pseudo;
	len = ft_strlen(field);
	if (!field)
		return ;
	if ((keycode >= 32 && keycode <= 126) || (keycode >= 65429 && keycode <= 65439))
	{
		if (len < 15)
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
	}
	else if (keycode == 65288 && len > 0)
		field[len - 1] = '\0';
	else if (keycode == 65293)
	{
		if (game->menu->text_field_selected == 1)
		{
			if (game->client->pseudo[0] == '\0')
				game->menu->error_name = true;
			else
				game->menu->status = MAIN_MENU;
		}
	}
}

void	update_get_pseudo_click(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.08;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.40;

	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
	{
		if (game->client->pseudo[0] == '\0')
			game->menu->error_name = true;
		else
			game->menu->status = MAIN_MENU;
	}
	if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + 30 && mouse_y <= y + 30 + btn_height)
		game->menu->text_field_selected = 1;
	else
		game->menu->text_field_selected = 0;
	game->menu->button_selected = 0;
}

void	update_get_pseudo_button(t_game *game, int mouse_x, int mouse_y)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.40;

	game->menu->button_selected = 0;
	if (mouse_x >= (game->screen_width - btn_width) * 0.5 && mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y + 200 && mouse_y <= y + 200 + btn_height)
			game->menu->button_selected = 1;
	}
}

void	draw_get_pseudo_menu(t_game *game)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.40;
	t_draw_info info3;
	t_draw_info info;

	if (game->menu->text_field_selected == 1)
	{
		info3 = init_draw_info(btn_height + 8, "", x - 4, y + 30 - 4);
		info3.color = MENU_BUTTON_SELECTED_COLOR;
		info3.width = btn_width + 8;
		info3.radius = 10;
		draw_rounded_rectangle(game, info3);
	}
	if (game->menu->error_name)
	{
		info3 = init_draw_info(btn_height + 8, "", x - 4, y + 30 - 4);
		info3.color = 0xFF0000;
		info3.width = btn_width + 8;
		info3.radius = 10;
		draw_rounded_rectangle(game, info3);
	}
	info = init_draw_info(btn_height * 0.9, "Enter your pseudo", game->screen_width >> 1, y - 90);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
	draw_text_field(game, x, y + 30, btn_width, btn_height, game->client->pseudo);
	if (game->menu->button_selected == 1)
	{
		info3 = init_draw_info(btn_height + 8, "", x - 4, y + 200 - 4);
		info3.color = MENU_BUTTON_SELECTED_COLOR;
		info3.width = btn_width + 8;
		info3.radius = 10;
		draw_rounded_rectangle(game, info3);
	}
	info = init_draw_info(btn_height * 0.5, "Continue", x + btn_width * 0.5, y + 200 + btn_height * 0.33 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	info3 = init_draw_info(0, "", x, y + 200);
	info3.color = MENU_BUTTON_COLOR;
	info3.height = btn_height;
	info3.width = btn_width;
	draw_rectangle(game, info3);
	draw_text(game, info);
}
