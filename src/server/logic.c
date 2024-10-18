/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:06:05 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 18:11:39 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	use_type(t_server *server, t_game_message msg)
{
	if (msg.type == MSG_CONNECT)
	{
		send(server->client_sockets[msg.player_id], &msg,
			sizeof(t_game_message), 0);
		send_all_players(server, msg.player_id);
		notify_players_of_connection(server, msg.player_id, msg.pseudo);
	}
	else if (msg.type == MSG_RECONNECT)
	{
		send(server->client_sockets[msg.player_id], &msg,
			sizeof(t_game_message), 0);
		send_all_players(server, msg.player_id);
		notify_players_of_reconnection(server, msg.player_id, msg.pseudo);
	}
	else if (msg.type == MSG_DISCONNECT)
		notify_players_of_disconnection(server, msg.player_id);
	else if (msg.type == MSG_MOVE)
		notify_players_of_move(server, msg);
	else if (msg.type == MSG_DOOR)
		notify_players_of_door(server, msg);
	else if (msg.type == MSG_CHAT)
		notify_players_of_chat(server, msg);
}

void	*logic_game(void *arg)
{
	t_server				*server;
	t_game_message_queue	*current;

	server = (t_server *)arg;
	while (1)
	{
		pthread_mutex_lock(server->game_lock);
		if (server->game_queue)
		{
			current = server->game_queue;
			server->game_queue = server->game_queue->next;
			use_type(server, current->message);
			free(current);
		}
		pthread_mutex_unlock(server->game_lock);
		usleep(1000);
	}
}
