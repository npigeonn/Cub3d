/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:05:54 by ybeaucou          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/11/06 11:55:26 by npigeon          ###   ########.fr       */
=======
/*   Updated: 2024/11/06 12:35:14 by ybeaucou         ###   ########.fr       */
>>>>>>> 042954a (add projectil)
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_connection_msg(t_game *game, char *pseudo)
{
	t_message	*new_msg;

	if (pseudo[0] == '\0')
		return ;
	new_msg = gc_malloc(game->mem, sizeof(t_message));
	ft_strlcpy(new_msg->message, pseudo, MAX_PSEUDO_LENGTH);
	ft_strlcat(new_msg->message + ft_strlen(pseudo), " join the game.",
		MAX_MESSAGE_LENGTH);
	new_msg->pseudo[0] = '\0';
	new_msg->next = game->chatbox->messages;
	new_msg->color = 0x32CD32;
	gettimeofday(&new_msg->time, NULL);
	game->chatbox->messages = new_msg;
}

static void update_enemies_client(t_game *game, t_game_message msg)
{
	t_sprite *current_enemy;
	t_sprite *incoming_enemy = msg.sprites;

	while (incoming_enemy)
	{
		current_enemy = game->sprites;
		int exists = 0;

		while (current_enemy)
		{
			if (incoming_enemy->type == SPRITE_ENEMY && current_enemy->type == SPRITE_ENEMY &&
				current_enemy->x == incoming_enemy->x && current_enemy->y == incoming_enemy->y)
			{
				current_enemy->health = incoming_enemy->health;
				current_enemy->dir_x = incoming_enemy->dir_x;
				current_enemy->dir_y = incoming_enemy->dir_y;
				current_enemy->floor = incoming_enemy->floor;
				exists = 1;
				break;
			}
			current_enemy = current_enemy->next;
		}
		if (!exists && incoming_enemy->type == SPRITE_ENEMY)
		{
			t_sprite *new_enemy = gc_malloc(game->mem, sizeof(t_sprite));
			if (new_enemy)
			{
				new_enemy->type = incoming_enemy->type;
				new_enemy->x = incoming_enemy->x;
				new_enemy->y = incoming_enemy->y;
				new_enemy->health = incoming_enemy->health;
				new_enemy->dir_x = incoming_enemy->dir_x;
				new_enemy->dir_y = incoming_enemy->dir_y;
				new_enemy->floor = incoming_enemy->floor;
				new_enemy->next = game->sprites;
				game->sprites = new_enemy;
			}
		}
		incoming_enemy = incoming_enemy->next;
	}

	current_enemy = game->sprites;
	t_sprite *previous_enemy = NULL;

	while (current_enemy)
	{
		incoming_enemy = msg.sprites;
		int found = 0;

		while (incoming_enemy)
		{
			if (current_enemy->type == SPRITE_ENEMY && 
				current_enemy->x == incoming_enemy->x && current_enemy->y == incoming_enemy->y && 
				current_enemy->floor == incoming_enemy->floor && 
				current_enemy->health == incoming_enemy->health &&
				current_enemy->dir_x == incoming_enemy->dir_x && 
				current_enemy->dir_y == incoming_enemy->dir_y) {
				found = 1;
				break;
			}
			incoming_enemy = incoming_enemy->next;
		}

		if (!found && current_enemy->type == SPRITE_ENEMY)
		{
			if (previous_enemy)
				previous_enemy->next = current_enemy->next;
			else
				game->sprites = current_enemy->next;
			gc_free(game->mem, current_enemy);
			current_enemy = (previous_enemy) ? previous_enemy->next : game->sprites;
		}
		else
		{
			previous_enemy = current_enemy;
			current_enemy = current_enemy->next;
		}
	}
}


static void	add_msg_chat(t_game *game, t_game_message msg)
{
	t_message	*new_msg;

	if (msg.pseudo[0] == '\0')
		return ;
	new_msg = gc_malloc(game->mem, sizeof(t_message));
	ft_strlcpy(new_msg->message, msg.message, MAX_MESSAGE_LENGTH);
	ft_strlcpy(new_msg->pseudo, msg.pseudo, 20);
	new_msg->next = game->chatbox->messages;
	new_msg->color = 0x000000;
	gettimeofday(&new_msg->time, NULL);
	game->chatbox->messages = new_msg;
}

static void	update_player_health(t_game *game, t_game_message msg)
{
	t_sprite *current = game->sprites;

	if (msg.player_id == game->client->player_id)
	{
		game->player->health = msg.health;
		return ;
	}
	while (current)
	{
		if (current->type == SPRITE_PLAYER && current->player_id == msg.player_id)
		{
			current->health = msg.health;
			break ;
		}
		current = current->next;
	}
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
		update_player_position(game, msg);
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
	else if (msg.type == MSG_PLAYER_HIT)
		update_player_health(game, msg);
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
