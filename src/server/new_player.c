/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_player.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 21:43:40 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/07 13:06:27 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	new_player_add(t_server *server, int i, int socket, char *pseudo)
{
	t_game_message	msg;

	server->client_sockets[i] = socket;
	add_player_node(server, i, pseudo);
	ft_bzero(&msg, sizeof(t_game_message));
	msg.type = MSG_CONNECT;
	msg.player_id = i;
	msg.x = server->x;
	msg.y = server->y;
	msg.floor = server->floor;
	msg.health = 1;
	msg.height = 0;
	// msg.ammo = 30;
	ft_strcpy(msg.pseudo, pseudo);
	add_game_message_to_queue(server, msg);
}

static void	new_player_full(int new_socket)
{
	t_game_message	msg;

	ft_bzero(&msg, sizeof(t_game_message));
	msg.type = MSG_FULL;
	send(new_socket, &msg, sizeof(t_game_message), 0);
	close(new_socket);
}

void	new_player(t_server *server, int new_socket, char *pseudo)
{
	int	i;

	pthread_mutex_lock(server->game_lock);
	i = -1;
	while (++i < MAX_PLAYERS)
	{
		if (server->client_sockets[i] == -1)
		{
			new_player_add(server, i, new_socket, pseudo);
			pthread_mutex_unlock(server->game_lock);
			return ;
		}
	}
	if (i >= MAX_PLAYERS)
		new_player_full(new_socket);
	pthread_mutex_unlock(server->game_lock);
}

void	set_player_id(t_server *server, t_sprite *player)
{
	int	i;

	i = -1;
	while (++i < MAX_PLAYERS)
	{
		if (player->type != SPRITE_PLAYER)
		{
			i--;
			continue ;
		}
		if (server->client_sockets[i] == -1)
		{
			player->player_id = i;
			break ;
		}
	}
}

static int	reconnect_player(t_server *server, t_sprite *player,
int socket)
{
	struct epoll_event	event;

	if (player->player_id < 0)
		set_player_id(server, player);
	server->client_sockets[player->player_id] = socket;
	event.events = EPOLLIN;
	event.data.fd = socket;
	if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, socket, &event) < 0)
	{
		close(socket);
		pthread_mutex_unlock(server->game_lock);
		return (1);
	}
	send_reconnected_message(server, player, player->pseudo);
	pthread_mutex_unlock(server->game_lock);
	return (1);
}

char	*existing_player(t_server *server, int new_socket)
{
	char			pseudo[MAX_PSEUDO_LENGTH];
	t_sprite		*player;

	pseudo[0] = '\0';
	if (new_socket < 0)
		return (pseudo);
	recv(new_socket, pseudo, sizeof(pseudo), 0);
	pthread_mutex_lock(server->game_lock);
	player = find_player_by_pseudo(server, pseudo);
	if (player)
	{
		if (player->player_id >= 0
			&& server->client_sockets[player->player_id] >= 0)
		{
			close(new_socket);
			pthread_mutex_unlock(server->game_lock);
			return (pseudo);
		}
		else if (reconnect_player(server, player, new_socket))
			return (NULL);
	}
	pthread_mutex_unlock(server->game_lock);
	return (gc_strdup(server->mem, pseudo));
}
