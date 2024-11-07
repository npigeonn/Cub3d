/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:05:54 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/07 12:57:47 by ybeaucou         ###   ########.fr       */
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

static void	update_enemy_data(t_sprite *dest, t_sprite *src)
{
	dest->health = src->health;
	dest->dir_x = src->dir_x;
	dest->dir_y = src->dir_y;
	dest->floor = src->floor;
}

static t_sprite	*find_enemy(t_sprite *list, t_sprite *target)
{
	while (list)
	{
		if (list->type == SPRITE_ENEMY && list->x == target->x && list->y == target->y && list->floor == target->floor)
			return (list);
		list = list->next;
	}
	return (NULL);
}

typedef struct	s_serialized_sprite
{
	int							type;
	float						x;
	float						y;
	float						direction;
	float						health;
	int							animation;
	int							floor;
	int							selected_anim;
	struct s_serialized_sprite	*next;
}	t_serialized_sprite;

void	update_enemies_client(t_game *game, t_game_message msg)
{
	t_serialized_sprite *incoming_enemy = msg.sprites;

	pthread_mutex_lock(&game->game_lock);
	while (incoming_enemy) {
		if (incoming_enemy->type == SPRITE_ENEMY) {
			t_sprite *existing_enemy = find_enemy(game->sprites, incoming_enemy);
			if (existing_enemy)
				update_enemy_data(existing_enemy, incoming_enemy);
			else
			{
				t_sprite *new_enemy = gc_malloc(game->mem, sizeof(t_sprite));
				if (new_enemy)
				{
					new_enemy->type = incoming_enemy->type;
					new_enemy->x = incoming_enemy->x;
					new_enemy->y = incoming_enemy->y;
					new_enemy->direction = incoming_enemy->direction;
					new_enemy->health = incoming_enemy->health;
					new_enemy->next = game->sprites;
					game->sprites = new_enemy;
				}
			}
		}
		incoming_enemy = incoming_enemy->next;
	}
	pthread_mutex_unlock(&game->game_lock);
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
