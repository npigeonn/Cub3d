/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:41:14 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/13 13:23:38 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_multiplayer_click(t_game *game, int keycode)
{
	if (keycode != 1)
		return ;
	pthread_mutex_lock(&game->client->mutex);
	if (game->menu->button_selected == 3)
		game->menu->status = MAIN_MENU;
	else if (game->menu->button_selected == 2)
	{
		game->menu->status = SERVER_CREATE;
		pthread_mutex_unlock(&game->client->mutex);
		pthread_join(game->discover_servers_thread, NULL);
		return ;
	}
	else if (game->menu->button_selected == 1)
		game->menu->status = JOIN_SERVER;
	else if (game->menu->button_selected == 5)
	{
		game->menu->status = OPTIONS_KEYBOARD;
		game->menu->last_status = SERVERS;
	}
	else if (game->menu->server_selected != 0)
		get_ip_server(game);
	game->menu->button_selected = 0;
	pthread_mutex_unlock(&game->client->mutex);
}

void	check_button_hover_multi(t_game *game, int mouse_x, int mouse_y)
{
	const int	list_width = game->screen_width * 0.65 - 20;
	const int	remaining_space = game->screen_width - ((game->screen_width
				- list_width) * 0.1 + 10 + list_width);
	const int	btn_y_start = game->screen_height * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	btn_x = (game->screen_width - list_width) * 0.1 + 10
		+ list_width + (remaining_space - game->screen_width * 0.25) * 0.5;

	if (mouse_x >= btn_x && mouse_x <= btn_x + game->screen_width * 0.25)
	{
		if (mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
			game->menu->button_selected = 1;
		else if (mouse_y >= btn_y_start + btn_height + game->screen_height
			* 0.05 && mouse_y <= btn_y_start + 2 * btn_height
			+ game->screen_height * 0.05)
			game->menu->button_selected = 2;
		else if (mouse_y >= btn_y_start + 2 * (btn_height + game->screen_height
				* 0.05) && mouse_y <= btn_y_start + 3 * btn_height + 2
			* game->screen_height * 0.05)
			game->menu->button_selected = 3;
	}
	check_mouse_on_gear(game, mouse_x, mouse_y);
}

void	update_multiplayer_menu(t_game *game, int mouse_x, int mouse_y)
{
	const int		list_width = game->screen_width * 0.65 - 20;
	const int		list_x = (game->screen_width - list_width) * 0.1 + 10;
	int				server_y_offset;
	t_server_info	*current;
	int				i;

	i = 1;
	server_y_offset = (game->screen_height - game->screen_height * 0.8)
		* 0.35 + 80 + 50;
	game->menu->button_selected = 0;
	game->menu->server_selected = 0;
	current = game->servers;
	while (current)
	{
		if (mouse_x >= list_x && mouse_x <= list_x + list_width && mouse_y
			>= server_y_offset && mouse_y <= server_y_offset + 80)
		{
			game->menu->server_selected = i;
			return ;
		}
		server_y_offset += 90;
		current = current->next;
		i++;
	}
	check_button_hover_multi(game, mouse_x, mouse_y);
}
