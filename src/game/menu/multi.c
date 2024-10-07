/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 01:48:20 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/07 09:05:26 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_multiplayer_menu(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
		
	const int	list_width = game->screen_width * 0.65;
	// const int	list_height = game->screen_height * 0.8;
	const int	list_x = (game->screen_width - list_width) * 0.1;
	// const int	list_y = (game->screen_height - list_height) * 0.35;
	// int			server_y_offset = list_y + 60;
	
	// for (int i = 0; i < 3; i++)
	// {
	// 	if (mouse_x >= list_x && mouse_x <= list_x + list_width &&
	// 		mouse_y >= server_y_offset && mouse_y <= server_y_offset + btn_height * 0.4)
	// 	{
	// 		// game->server_selected = i + 1;
	// 		game->button_selected = 0;
	// 		return;
	// 	}
	// 	server_y_offset += 60;
	// }

	const int	remaining_space = game->screen_width - (list_x + list_width);
	const int	btn_x = list_x + list_width + (remaining_space - btn_width) * 0.5;
	const int	btn_y_start = game->screen_height * 0.25;

	if (mouse_x >= btn_x && mouse_x <= btn_x + btn_width)
	{
		if (mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
			game->button_selected = 1;
		else if (mouse_y >= btn_y_start + btn_height + spacing && mouse_y <= btn_y_start + 2 * btn_height + spacing)
			game->button_selected = 2;
		else if (mouse_y >= btn_y_start + 2 * (btn_height + spacing) && mouse_y <= btn_y_start + 3 * btn_height + 2 * spacing)
			game->button_selected = 3;
		else
			game->button_selected = 0;
	}
	else
	{
		game->button_selected = 0;
		// game->server_selected = 0;
	}
}


void	draw_multiplayer_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
		
	const int	list_width = game->screen_width * 0.65;
	const int	list_height = game->screen_height * 0.8;
	const int	list_x = (game->screen_width - list_width) * 0.1;
	const int	list_y = (game->screen_height - list_height) * 0.35;

	draw_rectangle(game, list_x, list_y, list_width, list_height, MENU_BUTTON_COLOR);
	draw_text(game, "Available Servers", list_x + list_width * 0.5, list_y, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);

	int	server_y_offset = list_y + 60;
	draw_rectangle(game, list_x, server_y_offset - 10, list_width, 5, MENU_BUTTON_TEXT_COLOR);
	for (int i = 0; i < 3; i++)
	{
		char	server_name[50];
		snprintf(server_name, 50, "Server %d", i + 1);
		draw_text_left(game, server_name, list_x + 20, server_y_offset, btn_height * 0.4, MENU_BUTTON_TEXT_COLOR);
		draw_text_right(game, "1:4", list_x + list_width - 20, server_y_offset, btn_height * 0.4, MENU_BUTTON_TEXT_COLOR);
		draw_rectangle(game, list_x, server_y_offset + 50, list_width, 5, MENU_BUTTON_TEXT_COLOR);
		server_y_offset += 60;
	}

	const int	remaining_space = game->screen_width - (list_x + list_width);
	const int	btn_x = list_x + list_width + (remaining_space - btn_width) * 0.5;
	const int	btn_y_start = game->screen_height * 0.25;

	if (game->button_selected == 1)
		draw_rectangle(game, btn_x - 2, btn_y_start - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x, btn_y_start, btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Join Server", btn_x + btn_width * 0.5, btn_y_start + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);

	const int	create_btn_y = btn_y_start + btn_height + spacing;
	if (game->button_selected == 2)
		draw_rectangle(game, btn_x - 2, create_btn_y - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x, create_btn_y, btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Create Server", btn_x + btn_width * 0.5, create_btn_y + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	
	const int	back_btn_y = create_btn_y + btn_height + spacing;
	if (game->button_selected == 3)
		draw_rectangle(game, btn_x - 2, back_btn_y - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x, back_btn_y, btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Back", btn_x + btn_width * 0.5, back_btn_y + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
}

