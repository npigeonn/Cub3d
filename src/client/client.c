/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:22:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 23:55:15 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	manage_position(t_game *game, t_game_message msg)
{
	if (msg.x < 0 || msg.y < 0)
	{
		msg.type = MSG_MOVE;
		msg.x = game->player->x;
		msg.y = game->player->y;
		msg.health = game->player->health;
		msg.height = game->player->height;
		msg.floor = game->player->floor;
		send(game->client->sock, &msg, sizeof(t_game_message), 0);
	}
	else
	{
		game->player->x = msg.x;
		game->player->y = msg.y;
		game->player->health = msg.health;
		game->player->height = msg.height;
		game->player->floor = msg.floor;
	}
}

static int	manage_connection(t_game *game)
{
	t_game_message	connect_msg;
	ssize_t			recv_size;

	recv_size = recv(game->client->sock, &connect_msg,
			sizeof(t_game_message), 0);
	if (recv_size <= 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		close(game->client->sock);
		return (0);
	}
	if (connect_msg.type == MSG_FULL)
	{
		game->menu->status = SERVER_FULL;
		close(game->client->sock);
		return (0);
	}
	game->client->player_id = connect_msg.player_id;
	manage_position(game, connect_msg);
	return (1);
}

static int	manage_error(t_game *game, struct sockaddr_in serv_addr)
{
	if (inet_pton(AF_INET, game->client->ip, &serv_addr.sin_addr) <= 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		return (0);
	}
	if (connect(game->client->sock, (struct sockaddr *)&serv_addr,
			sizeof(serv_addr)) < 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		return (0);
	}
	return (1);
}

int	join_server(t_game *game)
{
	struct sockaddr_in	serv_addr;
	char				pseudo[MAX_PSEUDO_LENGTH];
	pthread_t			thread_id;

	ft_strcpy(pseudo, game->client->pseudo);
	game->client->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (game->client->sock < 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		return (0);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if (!manage_error(game, serv_addr))
		return (0);
	send(game->client->sock, pseudo, ft_strlen(pseudo) + 1, 0);
	if (!manage_connection(game))
		return (0);
	pthread_create(&thread_id, NULL, receive_updates, game);
	return (1);
}
