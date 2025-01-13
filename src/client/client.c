/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:13:44 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/13 11:18:22 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	manage_position(t_game *game, t_game_message msg)
{
	game->player->x = msg.x;
	game->player->y = msg.y;
	game->player->health = msg.health;
	game->player->height = msg.height;
	game->player->floor = msg.floor;
	if (game->player->health < 0)
		game->player->health = 0;
}

static int	manage_connection(t_game *game)
{
	t_game_message	connect_msg;
	ssize_t			recv_size;

	recv_size = recv(game->client->sock, &connect_msg,
			sizeof(t_game_message), 0);
	if (recv_size <= 0 || connect_msg.type == MSG_FULL)
	{
		if (recv_size <= 0)
			game->menu->status = SERVER_DISCONNECTED;
		else
			game->menu->status = SERVER_FULL;
		close(game->client->sock);
		return (0);
	}
	game->client->player_id = connect_msg.player_id;
	manage_position(game, connect_msg);
	if (!receive_map_from_server(game, game->client->sock))
	{
		game->menu->status = SERVER_DISCONNECTED;
		close(game->client->sock);
		return (0);
	}
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

void	reset_enemies(t_game *game)
{
	t_sprite	*current;
	t_sprite	*previous;
	t_sprite	*next;

	previous = NULL;
	current = game->sprites;
	while (current)
	{
		if (current->type == SPRITE_ENEMY)
		{
			next = current->next;
			gc_free(game->mem, current);
			if (previous)
				previous->next = next;
			else
				game->sprites = next;
			current = next;
		}
		else
		{
			previous = current;
			current = current->next;
		}
	}
}

int	join_server(t_game *game)
{
	struct sockaddr_in	serv_addr;
	char				pseudo[MAX_PSEUDO_LENGTH];

	ft_strcpy(pseudo, game->client->pseudo);
	reset_enemies(game);
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
	pthread_mutex_init(&game->game_lock, NULL);
	pthread_create(&game->client->thread, NULL, receive_updates, game);
	return (1);
}
