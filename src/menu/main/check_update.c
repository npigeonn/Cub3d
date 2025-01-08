/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:51:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:22:20 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_main_menu_click(t_game *game, int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
	{
		game->menu->status = PLAYING;
		x_fixes_cursor(game, 'h');
	}
	else if (game->menu->button_selected == 2)
	{
		game->menu->status = SERVERS;
		pthread_create(&game->discover_servers_thread, NULL,
			discover_servers_thread, game);
	}
	else if (game->menu->button_selected == 3)
		game->menu->status = STATS;
	else if (game->menu->button_selected == 4)
		handle_close(game);
	else if (game->menu->button_selected == 5)
	{
		game->menu->status = OPTIONS_KEYBOARD;
		game->menu->last_status = MAIN_MENU;
	}
	else
		return ;
	game->menu->button_selected = 0;
}

void	check_main_mouse_on_gear(t_game *game, int mouse_x, int mouse_y)
{
	const int	gear_size = game->screen_width * 0.035;
	const int	dx = mouse_x - (game->screen_width - gear_size / 2 - 17);
	const int	dy = mouse_y - (gear_size / 2 + 15);
	const int	distance_squared = dx * dx + dy * dy;

	if (distance_squared >= (gear_size / 2 - 5) * (gear_size / 2 - 5)
		&& distance_squared <= (gear_size / 2 + 5) * (gear_size / 2 + 5))
		game->menu->button_selected = 5;
	else if (distance_squared <= (gear_size / 2) * (gear_size / 2))
		game->menu->button_selected = 5;
}

void	update_main_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	y = game->screen_height * 0.22;

	game->menu->button_selected = 0;
	if (mouse_x >= (game->screen_width - btn_width) * 0.5
		&& mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y && mouse_y <= y + btn_height)
			game->menu->button_selected = 1;
		else if (mouse_y >= y + btn_height + spacing
			&& mouse_y <= y + btn_height + spacing + btn_height)
			game->menu->button_selected = 2;
		else if (mouse_y >= y + 2 * (btn_height + spacing)
			&& mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 3;
		else if (mouse_y >= y + 3 * (btn_height + spacing)
			&& mouse_y <= y + 3 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 4;
	}
	check_mouse_on_gear(game, mouse_x, mouse_y);
}
