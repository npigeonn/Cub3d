/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_disconnected.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:21:28 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:13:57 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_server_error_click(t_game *game, int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
		game->menu->status = SERVERS;
	game->menu->button_selected = 0;
}

void	update_server_error_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	y = game->screen_height * 0.25;

	game->menu->button_selected = 0;
	if (mouse_x >= (game->screen_width - btn_width) * 0.5
		&& mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y + 2 * (btn_height + spacing)
			&& mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 1;
	}
}

void	draw_server_error_title(t_game *game)
{
	char		*message;
	t_draw_info	info;

	if (game->menu->status == SERVER_FULL)
		message = "Server is full.";
	else
		message = "You have been disconnected from the server.";
	info = init_draw_info(70, message, game->screen_width >> 1,
			game->screen_height * 0.4);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_server_button(t_game *game, int x, int y)
{
	t_draw_info	info2;
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;

	info2 = init_draw_info(0, "", x - 2, y + 2 * (btn_height + spacing) - 2);
	info2.width = btn_width + 4;
	info2.height = btn_height + 4;
	info2.color = MENU_BUTTON_SELECTED_COLOR;
	if (game->menu->button_selected == 1)
		draw_rectangle(game, info2);
	info2 = init_draw_info(btn_height, "", x, y + 2 * (btn_height + spacing));
	info2.color = MENU_BUTTON_COLOR;
	info2.width = btn_width;
	draw_rectangle(game, info2);
}

void	draw_server_error_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;
	t_draw_info	info;

	x_fixes_cursor(game, 's');
	draw_server_error_title(game);
	draw_server_button(game, x, y);
	ft_strcpy(info.str, "Return to Menu");
	info.y = y + 2 * (btn_height + game->screen_height * 0.05)
		+ btn_height * 0.33 - 5;
	info.height = btn_height * 0.5;
	info.x = x + btn_width / 2;
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
	game->chatbox->visible = false;
	game->chatbox->is_writting = false;
}
