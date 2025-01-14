/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:11:28 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/14 08:53:37 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_server_info(t_game *game, t_server_info *current, int list_x,
int server_y_offset)
{
	const int	btn_height = game->screen_height * 0.1;
	const int	list_width = game->screen_width * 0.65;
	t_draw_info	info3;

	info3 = init_draw_info(btn_height * 0.4, current->name, list_x + 40,
			server_y_offset + 10);
	info3.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, info3);
	ft_strcpy(info3.str, "1/4");
	info3.y = server_y_offset + 22;
	info3.x = list_x + list_width - 80;
	info3.height = btn_height * 0.4;
	draw_text_right(game, info3);
	info3.x = list_x + 40;
	ft_strcpy(info3.str, "Ping: 30ms");
	info3.y = server_y_offset + 50;
	info3.height = btn_height * 0.3;
	draw_text_left(game, info3);
}

static void	show_list_servers_loop(t_game *game, t_server_info *current,
int list_x, int server_y_offset)
{
	int			i;

	i = 1;
	while (current && server_y_offset < game->screen_height * 0.25 + 80 + 50
		+ game->screen_height * 0.8)
	{
		show_bg_servers_btn(game, list_x, server_y_offset, i);
		draw_server_info(game, current, list_x, server_y_offset);
		server_y_offset += 90;
		current = current->next;
		i++;
	}
}

void	show_list_servers(t_game *game, int list_x, int list_y)
{
	const int		btn_height = game->screen_height * 0.1;
	const int		list_width = game->screen_width * 0.65;
	t_draw_info		info;
	t_server_info	*current;

	if (game->servers == NULL)
	{
		info = init_draw_info(btn_height * 0.5, "Searching for servers...",
				list_x + list_width * 0.5, 0);
		info.color = 0x378599;
		info.y = (game->screen_height >> 1) - 80;
		draw_text(game, info);
	}
	else
	{
		pthread_mutex_lock(&game->mutex);
		current = game->servers;
		show_list_servers_loop(game, current, list_x, list_y + 80);
		pthread_mutex_unlock(&game->mutex);
	}
}
