/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:17:21 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/03 03:26:45 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_connection_msg(t_game *game, char *pseudo)
{
	t_message	*new_msg;

	if (pseudo[0] == '\0')
		return ;
	new_msg = malloc(sizeof(t_message));
	ft_strlcpy(new_msg->message, pseudo, MAX_PSEUDO_LENGTH);
	ft_strlcat(new_msg->message + ft_strlen(pseudo), " join the game.",
		MAX_MESSAGE_LENGTH);
	new_msg->pseudo[0] = '\0';
	new_msg->next = game->chatbox->messages;
	new_msg->color = 0x32CD32;
	gettimeofday(&new_msg->time, NULL);
	game->chatbox->messages = new_msg;
}

static void	update_enemies_client(t_game *game, t_game_message msg)
{
	t_sprite	*current_enemy;
	t_sprite	*incoming_enemy;

	current_enemy = game->sprites;
	incoming_enemy = msg.sprites;
	while (current_enemy && incoming_enemy)
	{
		if (current_enemy->type == SPRITE_ENEMY)
		{
			current_enemy->x = incoming_enemy->x;
			current_enemy->y = incoming_enemy->y;
			current_enemy->health = incoming_enemy->health;
			current_enemy->dirX = incoming_enemy->dirX;
			current_enemy->dirY = incoming_enemy->dirY;
			current_enemy->floor = incoming_enemy->floor;
		}
		current_enemy = current_enemy->next;
		incoming_enemy = incoming_enemy->next;
	}
}

static void	add_msg_chat(t_game *game, t_game_message msg)
{
	t_message	*new_msg;

	if (msg.pseudo[0] == '\0')
		return ;
	new_msg = malloc(sizeof(t_message));
	ft_strlcpy(new_msg->message, msg.message, MAX_MESSAGE_LENGTH);
	ft_strlcpy(new_msg->pseudo, msg.pseudo, 20);
	new_msg->next = game->chatbox->messages;
	new_msg->color = 0x000000;
	gettimeofday(&new_msg->time, NULL);
	game->chatbox->messages = new_msg;
}

static int	gestion_message(t_game *game, t_game_message msg)
{
	if (msg.type == MSG_FULL)
	{
		game->menu->status = SERVER_DISCONNECTED;
		close(game->client->sock);
		return (0);
	}
	else if (msg.type == MSG_MOVE)
		update_player_position(game->client, msg);
	else if (msg.type == MSG_RECONNECT || msg.type == MSG_CONNECT
		|| msg.type == MSG_GET_PLAYER)
		add_player(game, msg);
	else if (msg.type == MSG_DISCONNECT)
		remove_player(game, msg.player_id);
	else if (msg.type == MSG_DOOR)
		update_door(game, msg);
	else if (msg.type == MSG_CHAT)
		add_msg_chat(game, msg);
	else if (msg.type == MSG_BROADCAST_ENEMIES)
		update_enemies_client(game, msg);
	return (1);
}

void	*receive_updates(void *args)
{
	t_game			*game;
	int				sock;
	t_game_message	msg;
	ssize_t			valread;

	game = (t_game *)args;
	sock = game->client->sock;
	while (1)
	{
		valread = recv(sock, &msg, sizeof(t_game_message), 0);
		if (valread <= 0)
		{
			game->menu->status = SERVER_DISCONNECTED;
			close(sock);
			break ;
		}
		if (!gestion_message(game, msg))
			break ;
	}
}
