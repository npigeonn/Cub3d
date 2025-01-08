/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:06:05 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/03 17:05:46 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	use_type(t_server *server, t_game_message msg)
{
	if (msg.type == MSG_CONNECT)
	{
		send_info(server, msg);
		notify_players_of_connection(server, msg.player_id, msg.pseudo);
	}
	else if (msg.type == MSG_RECONNECT)
	{
		send_info(server, msg);
		notify_players_of_reconnection(server, msg.player_id, msg.pseudo);
	}
	else if (msg.type == MSG_DISCONNECT)
		notify_players_of_disconnection(server, msg.player_id);
	else if (msg.type == MSG_MOVE || msg.type == MSG_DOOR
		|| msg.type == MSG_CHAT)
		notify_players(server, msg);
	else if (msg.type == MSG_PLAYER_HIT)
		notify_all_players(server, msg);
	else if (msg.type == MSG_PLAYER_SHOOT)
		add_projectile(server, msg);
}

void	broadcast_enemies(t_server *server)
{
	t_game_message	msg;
	t_sprite		*current;

	ft_bzero(&msg, sizeof(t_game_message));
	msg.type = MSG_BROADCAST_ENEMIES;
	current = server->sprites;
	while (current)
	{
		if (current->type == SPRITE_ENEMY)
		{
			msg.x = current->x;
			msg.y = current->y;
			msg.floor = current->floor;
			msg.dir_x = current->dir_x;
			msg.dir_y = current->dir_y;
			msg.health = current->health;
			msg.player_id = current->player_id;
			msg.animation = current->animation;
			msg.selected_anim = current->selected_anim;
			msg.state = current->state;
			send_enemies(server, msg);
		}
		current = current->next;
	}
}

static void	use_queue_server(t_server *server)
{
	t_game_message_queue	*current;

	if (server->game_queue)
	{
		current = server->game_queue;
		server->game_queue = server->game_queue->next;
		use_type(server, current->message);
		gc_free(server->mem, current);
	}
}

void	*logic_game(void *arg)
{
	t_server				*server;
	struct timeval			current_time;
	float					seconds;

	server = (t_server *)arg;
	while (1)
	{
		pthread_mutex_lock(server->game_lock);
		use_queue_server(server);
		pthread_mutex_unlock(server->game_lock);
		gettimeofday(&current_time, NULL);
		seconds = (current_time.tv_sec - server->last_time.tv_sec)
			+ (current_time.tv_usec - server->last_time.tv_usec) / 1000000.0f;
		if (seconds > 0.04)
		{
			pthread_mutex_lock(server->game_lock);
			server->delta_time = seconds;
			server->last_time = current_time;
			update_enemies_server(server);
			broadcast_enemies(server);
			update_projectiles_server(server);
			pthread_mutex_unlock(server->game_lock);
		}
	}
}
