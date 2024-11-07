/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:06:05 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/07 12:56:32 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	send_file_to_client(int client_socket, const char *filename)
{
	FILE			*file = fopen(filename, "rb");
	char			buffer[1024];
	size_t			bytes_read;
	t_game_message	msg;

	if (!file)
		return ;
	send(client_socket, filename, strlen(filename) + 1, 0);
	ft_memset(&msg, 0, sizeof(t_game_message));
	msg.type = MSG_FILE_SIZE;
	fseek(file, 0, SEEK_END);
	msg.file_size = htonl(ftell(file));
	fseek(file, 0, SEEK_SET);
	printf("Sending file %s of size %u\n", filename, msg.file_size);
	send(client_socket, &msg, sizeof(t_game_message), 0);
	bytes_read = fread(buffer, 1, sizeof(buffer), file);
	while (bytes_read  > 0)
	{
		send(client_socket, buffer, bytes_read, 0);
		bytes_read = fread(buffer, 1, sizeof(buffer), file);
	}
	fclose(file);
}

void	send_info(t_server *server, t_game_message msg)
{
	send(server->client_sockets[msg.player_id], &msg, sizeof(t_game_message), 0);
	send_file_to_client(server->client_sockets[msg.player_id], server->av[1]);
	send_all_players(server, msg.player_id);
}

void	add_projectile(t_server *server, t_game_message msg)
{
	t_projectile	*new_projectile;
	t_sprite		*sprite;

	sprite = find_player_by_id(server->sprites, msg.player_id);
	new_projectile = gc_malloc(server->mem, sizeof(t_projectile));
	new_projectile->x = msg.x;
	new_projectile->y = msg.y;
	new_projectile->direction = atan2(msg.dir_y, msg.dir_x) * (180.0f / M_PI);
	new_projectile->speed = 2000;
	new_projectile->next = NULL;
	new_projectile->owner = NULL;
	new_projectile->enemy = sprite;
	new_projectile->next = server->projectiles; 
	new_projectile->floor = msg.floor;
	new_projectile->damage = 0.2;
	server->projectiles = new_projectile;
}

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
	else if (msg.type == MSG_MOVE)
		notify_players_of_move(server, msg);
	else if (msg.type == MSG_DOOR)
		notify_players_of_door(server, msg);
	else if (msg.type == MSG_CHAT)
		notify_players_of_chat(server, msg);
	else if (msg.type == MSG_PLAYER_HIT)
		notify_players_of_hit(server, msg);
	else if (msg.type == MSG_PLAYER_SHOOT)
		add_projectile(server, msg);
}

long	get_current_time()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
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

void	broadcast_enemies(t_server *server)
{
	t_game_message msg;
	t_sprite *current;
	t_serialized_sprite *serialized_sprites = NULL;
	t_serialized_sprite *serialized_last = NULL;
	
	ft_memset(&msg, 0, sizeof(t_game_message));
	msg.type = MSG_BROADCAST_ENEMIES;
	msg.player_id = -1;
	current = server->sprites;
	while (current)
	{
		if (current->type == SPRITE_ENEMY)
		{
			t_serialized_sprite *new_sprite = gc_malloc(server->mem, sizeof(t_serialized_sprite));
			new_sprite->type = current->type;
			new_sprite->x = current->x;
			new_sprite->y = current->y;
			new_sprite->direction = current->direction;
			new_sprite->health = current->health;
			new_sprite->animation = current->animation;
			new_sprite->floor = current->floor;
			new_sprite->selected_anim = current->selected_anim;
			if (serialized_sprites == NULL)
			{
				serialized_sprites = new_sprite;
				serialized_last = new_sprite;
			}
			else
			{
				serialized_last->next = new_sprite;
				serialized_last = new_sprite;
			}
		}
		current = current->next;
	}
	
	msg.sprites = serialized_sprites;
	current = server->sprites;
	while (current)
	{
		if (current->type == SPRITE_PLAYER && current->player_id >= 0)
			send(server->client_sockets[current->player_id], &msg, sizeof(t_game_message), 0);
		current = current->next;
	}
	while (serialized_sprites)
	{
		t_serialized_sprite *temp = serialized_sprites;
		serialized_sprites = serialized_sprites->next;
		gc_free(server->mem, temp);
	}
}

bool	has_line_of_sight_server(t_server *server, t_point enemy_pos, t_point player_pos, float enemy_facing_angle, float fov_angle)
{
	float dx = player_pos.x - enemy_pos.x;
	float dy = player_pos.y - enemy_pos.y;
	float distance = sqrt(dx * dx + dy * dy);

	float angle_to_player = atan2(dy, dx) * (180.0f / M_PI);
	float angle_diff = fabsf(angle_to_player - enemy_facing_angle);

	if (angle_diff <= fov_angle * 0.5)
	{
		float step_x = dx / distance;
		float step_y = dy / distance;
		for (float t = 0; t < distance; t += 0.1f)
		{
			enemy_pos.x += step_x * 0.1f;
			enemy_pos.y += step_y * 0.1f;
			if (!can_move(server->map, server->door, enemy_pos.x, enemy_pos.y, enemy_pos.floor))
				return (false);
		}
		return (true);
	}
	return (false);
}

void	shoot_at_player_server(t_sprite *enemy, t_point player_pos, t_server *server)
{
	float dx = player_pos.x - enemy->x;
	float dy = player_pos.y - enemy->y;
	float angle_to_player = atan2(dy, dx) * (180.0f / M_PI);
	float angle_diff = fmodf(fabsf(angle_to_player - enemy->direction), 360.0f);
		
	if (angle_diff > 180.0f)
		angle_diff = 360.0f - angle_diff;
	if (enemy->animation > 0)
		enemy->animation -= 15 * server->delta_time;
	if (enemy->animation < 0)
		enemy->selected_anim = 0;
	if (angle_diff < 5.0f && enemy->shoot_delay <= 0)
	{
		t_projectile *new_projectile = gc_malloc(server->mem, sizeof(t_projectile));
		enemy->selected_anim = 1;
		enemy->animation = 5;
		
		if (new_projectile)
		{
			new_projectile->x = enemy->x;
			new_projectile->y = enemy->y;
			new_projectile->direction = angle_to_player;
			new_projectile->speed = 2000;
			new_projectile->next = NULL;
			new_projectile->owner = NULL;
			new_projectile->enemy = enemy;
			new_projectile->next = server->projectiles; 
			new_projectile->floor = enemy->floor;
			new_projectile->damage = 0.09f;
			server->projectiles = new_projectile;
		}
		enemy->shoot_delay = 1;
	}
	else if (enemy->shoot_delay > 0)
		enemy->shoot_delay -= 0.6 * server->delta_time;
}

void	update_enemies_server(t_server *server)
{
	t_sprite	*current_enemy = server->sprites;

	while (current_enemy)
	{
		if (current_enemy->type != SPRITE_ENEMY || current_enemy->health <= 0)
		{
			if (current_enemy->type == SPRITE_ENEMY && current_enemy->animation > 0)
			{
				current_enemy->animation -= server->delta_time * 2.5;
				current_enemy->selected_anim = (int)(4 - current_enemy->animation);
				if (current_enemy->selected_anim < 0)
					current_enemy->selected_anim = 0;
			}
			current_enemy = current_enemy->next;
			continue;
		}
		t_sprite *target_player = NULL;
		float min_distance = INFINITY;

		t_sprite *current = server->sprites;
		while (current)
		{
			if (current->type != SPRITE_PLAYER || current->player_id < 0 || current->floor != current_enemy->floor)
			{
				current = current->next;
				continue;
			}
			float dx = current->x - current_enemy->x;
			float dy = current->y - current_enemy->y;
			float distance = sqrt(dx * dx + dy * dy);

			if (distance < min_distance && distance < 10 && has_line_of_sight_server(server, (t_point){current_enemy->x, current_enemy->y, current_enemy->floor}, (t_point){current->x, current->y, current->floor}, current_enemy->direction, current_enemy->fov))
			{
				min_distance = distance;
				target_player = current;
			}
			current = current->next;
		}
		if (!target_player)
		{
			if (current_enemy->state != PATROL)
			{
				current_enemy->state = PATROL;
				current_enemy->frame_count = 0;
			}
			if (current_enemy->frame_count % 220 == 0)
				current_enemy->direction = rand() % 360;
			
			current_enemy->animation += server->delta_time;
			current_enemy->selected_anim = (int)(current_enemy->animation * 2) % 4;
			float angle_in_radians = current_enemy->direction * (M_PI / 180);
			current_enemy->dir_x = cos(angle_in_radians);
			current_enemy->dir_y = sin(angle_in_radians);
			float new_x = current_enemy->x + current_enemy->dir_x * 0.02;
			float new_y = current_enemy->y + current_enemy->dir_y * 0.02;
			if (can_move(server->map, server->door, new_x, new_y, current_enemy->floor))
			{
				current_enemy->x = new_x;
				current_enemy->y = new_y;
			}
			else
			{
				current_enemy->direction = rand() % 360;
				current_enemy->frame_count = 0;
			}
		}
		else
		{
			current_enemy->state = CHASE;
			float dx = target_player->x - current_enemy->x;
			float dy = target_player->y - current_enemy->y;
			current_enemy->direction = atan2(dy, dx) * (180 / M_PI);
			float angle_in_radians = current_enemy->direction * (M_PI / 180);
			current_enemy->dir_x = cos(angle_in_radians);
			current_enemy->dir_y = sin(angle_in_radians);
			if (min_distance < 7)
				shoot_at_player_server(current_enemy, (t_point){target_player->x, target_player->y, target_player->floor}, server);
		}
		current_enemy->frame_count++;
		current_enemy = current_enemy->next;
	}
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
			gc_free(server->mem, current);
		}
		pthread_mutex_unlock(server->game_lock);
		struct timeval current_time;
		gettimeofday(&current_time, NULL);

		float seconds = (current_time.tv_sec - server->last_time.tv_sec) +
						(current_time.tv_usec - server->last_time.tv_usec) / 1000000.0f;
		if (seconds > 0.1)
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
