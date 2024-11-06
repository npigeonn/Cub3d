/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:17:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/06 13:34:17 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	handle_client_msg(t_server *server, t_game_message msg)
{
	int			i;
	t_sprite	*player;

	pthread_mutex_lock(server->game_lock);
	if (msg.type == MSG_MOVE)
	{
		player = find_player_by_pseudo(server, msg.pseudo);
		if (player)
		{
			player->x = msg.x;
			player->y = msg.y;
			player->dir_x = msg.dir_x;
			player->dir_y = msg.dir_y;
			player->floor = msg.floor;
			add_game_message_to_queue(server, msg);
		}
	}
	if (msg.type == MSG_CHAT || msg.type == MSG_PLAYER_SHOOT || msg.type == MSG_DOOR || msg.type == MSG_PLAYER_SHOOT)
		add_game_message_to_queue(server, msg);
	pthread_mutex_unlock(server->game_lock);
}

void	handle_client_receive_msg(t_server *server, int client_socket)
{
	t_game_message	msg;
	ssize_t			valread;
	int				i;

	valread = recv(client_socket, &msg, sizeof(t_game_message), 0);
	if (valread <= 0)
	{
		i = -1;
		while (++i < MAX_PLAYERS)
		{
			if (server->client_sockets[i] == client_socket)
			{
				msg.type = MSG_DISCONNECT;
				msg.player_id = i;
				add_game_message_to_queue(server, msg);
				close(client_socket);
				server->client_sockets[i] = -1;
				break ;
			}
		}
		return ;
	}
	handle_client_msg(server, msg);
}
