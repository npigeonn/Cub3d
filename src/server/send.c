/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:06:59 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/29 17:36:58 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	send_all_players(t_server *server, int id)
{
	t_sprite		*current;
	t_game_message	connect_msg;

	current = server->sprites;
	ft_bzero(&connect_msg, sizeof(t_game_message));
	connect_msg.type = MSG_GET_PLAYER;
	while (current)
	{
		if (current->type == SPRITE_PLAYER && current->player_id >= 0
			&& current->player_id != id)
		{
			connect_msg.player_id = current->player_id;
			connect_msg.x = current->x;
			connect_msg.y = current->y;
			connect_msg.dir_x = current->dir_x;
			connect_msg.dir_y = current->dir_y;
			connect_msg.floor = current->floor;
			ft_strcpy(connect_msg.pseudo, current->pseudo);
			send(server->client_sockets[id], &connect_msg,
				sizeof(t_game_message), 0);
		}
		current = current->next;
	}
}

void	send_reconnected_message(t_server *server, t_sprite *player,
char *pseudo)
{
	t_game_message	connect_msg;

	connect_msg.type = MSG_RECONNECT;
	connect_msg.player_id = player->player_id;
	connect_msg.x = player->x;
	connect_msg.y = player->y;
	connect_msg.dir_x = player->dir_x;
	connect_msg.dir_y = player->dir_y;
	connect_msg.floor = player->floor;
	connect_msg.health = player->health;
	ft_strcpy(connect_msg.pseudo, pseudo);
	add_game_message_to_queue(server, connect_msg);
}
