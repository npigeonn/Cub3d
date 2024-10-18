/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notification.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:48:35 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 16:03:54 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	notify_players_of_connection(t_server *server, int player_id,
char *pseudo)
{
	t_game_message	connect_msg;
	int				i;
	t_player_info	*player;

	player = find_player_by_pseudo(server, pseudo);
	if (!player)
		return ;
	connect_msg.type = MSG_CONNECT;
	connect_msg.player_id = player_id;
	connect_msg.x = player->x;
	connect_msg.y = player->y;
	connect_msg.floor = player->floor;
	connect_msg.height = player->height;
	connect_msg.dir_x = player->dir_x;
	connect_msg.dir_y = player->dir_y;
	connect_msg.health = player->health;
	ft_strlcpy(connect_msg.pseudo, pseudo, MAX_PSEUDO_LENGTH);
	i = -1;
	while (++i < MAX_PLAYERS)
		if (server->client_sockets[i] > 0 && i != player_id)
			send(server->client_sockets[i], &connect_msg,
				sizeof(t_game_message), 0);
}

void	notify_players_of_reconnection(t_server *server, int player_id,
char *pseudo)
{
	t_game_message	connect_msg;
	int				i;
	t_player_info	*player;

	player = find_player_by_pseudo(server, pseudo);
	if (!player)
		return ;
	connect_msg.type = MSG_CONNECT;
	connect_msg.player_id = player_id;
	connect_msg.x = player->x;
	connect_msg.y = player->y;
	connect_msg.floor = player->floor;
	connect_msg.height = player->height;
	connect_msg.dir_x = player->dir_x;
	connect_msg.dir_y = player->dir_y;
	connect_msg.health = player->health;
	ft_strlcpy(connect_msg.pseudo, pseudo, MAX_PSEUDO_LENGTH);
	i = -1;
	while (++i < MAX_PLAYERS)
		if (server->client_sockets[i] > 0 && i != player_id)
			send(server->client_sockets[i], &connect_msg,
				sizeof(t_game_message), 0);
}

void	notify_players_of_disconnection(t_server *server, int id)
{
	t_player_info	*player;
	t_game_message	disconnect_msg;
	int				i;

	player = find_player_by_id(server->players, id);
	if (!player)
		return ;
	disconnect_msg.player_id = player->player_id;
	disconnect_msg.type = MSG_DISCONNECT;
	ft_strlcpy(disconnect_msg.pseudo, player->pseudo, MAX_PSEUDO_LENGTH);
	player->player_id = -1;
	i = -1;
	while (++i < MAX_PLAYERS)
		if (server->client_sockets[i] > 0 && i != player->player_id)
			send(server->client_sockets[i], &disconnect_msg,
				sizeof(t_game_message), 0);
}

void	notify_players_of_move(t_server *server, t_game_message msg)
{
	int				i;

	msg.type = MSG_MOVE;
	update_player_node(server, msg.pseudo, msg);
	i = -1;
	while (++i < MAX_PLAYERS)
		if (server->client_sockets[i] > 0 && i != msg.player_id)
			send(server->client_sockets[i], &msg, sizeof(t_game_message), 0);
}

void	notify_players_of_door(t_server *server, t_game_message msg)
{
	int	i;

	i = -1;
	while (++i < MAX_PLAYERS)
		if (server->client_sockets[i] > 0 && i != msg.player_id)
			send(server->client_sockets[i], &msg,
				sizeof(t_game_message), 0);
}

void	notify_players_of_chat(t_server *server, t_game_message msg)
{
	int	i;

	i = -1;
	while (++i < MAX_PLAYERS)
		if (server->client_sockets[i] > 0 && i != msg.player_id)
			send(server->client_sockets[i], &msg, sizeof(t_game_message), 0);
}
