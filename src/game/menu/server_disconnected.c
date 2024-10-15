/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_disconnected.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:21:28 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/15 12:44:49 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_server_error_click(t_game *game, int mouse_x, int mouse_y, int keycode)
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
	if (mouse_x >= (game->screen_width - btn_width) * 0.5 && mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y + 2 * (btn_height + spacing) && mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 1;
	}
}

void	draw_server_error_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;

	const char *message = "You have been disconnected from the server.";
	draw_text(game, message, game->screen_width >> 1, game->screen_height * 0.4, 70, MENU_BUTTON_TEXT_COLOR);
	if (game->menu->button_selected == 1)
		draw_rectangle(game, x - 2, y + 2 * (btn_height + spacing) - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, x, y + 2 * (btn_height + spacing), btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Return to Menu", x + btn_width / 2, y + 2 * (btn_height + spacing) + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
}
