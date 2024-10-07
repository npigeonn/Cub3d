/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 01:48:20 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/07 01:48:42 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../includes/cub3d.h"

void draw_multiplayer_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
		
	const int	list_width = game->screen_width * 0.65;
	const int	list_height = game->screen_height * 0.8;
	const int	list_x = (game->screen_width - list_width) * 0.02;
	const int	list_y = (game->screen_height - list_height) / 2;

	draw_rectangle(game, list_x, list_y, list_width, list_height, MENU_BUTTON_COLOR);
	draw_text(game, "Available Servers", list_x + list_width * 0.05, list_y - 25, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);

	int	server_y_offset = list_y + 40;
	for (int i = 0; i < 3; i++)
	{
		char	server_name[50];
		snprintf(server_name, 50, "Server %d", i + 1);
		draw_text(game, server_name, list_x + 20, server_y_offset, btn_height * 0.4, MENU_BUTTON_TEXT_COLOR);
		server_y_offset += 50;
	}

	const int	btn_x = game->screen_width - btn_width - 20;
	const int	btn_y_start = list_y;

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

