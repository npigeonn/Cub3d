/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:06:05 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/29 18:36:12 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	send_file_to_client(int client_socket, const char *filename)
{
	FILE	*file;
	char	buffer[1024];
	size_t	bytes_read;
	long	file_size;

	file = fopen(filename, "rb");
	if (!file)
		return ;
	send(client_socket, filename, 256, 0);
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	send(client_socket, &file_size, sizeof(long), 0);
	bytes_read = fread(buffer, 1, sizeof(buffer), file);
	while (bytes_read > 0)
	{
		send(client_socket, buffer, bytes_read, 0);
		bytes_read = fread(buffer, 1, sizeof(buffer), file);
	}
	fclose(file);
}

void	send_info(t_server *server, t_game_message msg)
{
	send(server->client_sockets[msg.player_id],
		&msg, sizeof(t_game_message), 0);
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

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	send_enemies(t_server *server, t_game_message msg)
{
	int	i;

	i = -1;
	while (++i < MAX_PLAYERS)
		if (server->client_sockets[i] > 0)
			send(server->client_sockets[i], &msg, sizeof(t_game_message), 0);
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

bool	has_line_of_sight_server(t_server *server, t_sprite *enemy,
t_sprite *player)
{
	const float	dx = player->x - enemy->x;
	const float	dy = player->y - enemy->y;
	const float	distance = sqrt(dx * dx + dy * dy);
	float		t;
	float		ray[2];

	t = 0;
	if (fabsf(atan2(dy, dx) * (180.0f / M_PI) - enemy->direction)
		<= enemy->fov * 0.5)
	{
		while (t < distance)
		{
			enemy->x += dx / distance * 0.1f;
			enemy->y += dy / distance * 0.1f;
			ray[0] = enemy->x;
			ray[1] = enemy->y;
			if (!can_move(server->map, server->door, ray, enemy->floor))
				return (false);
			t += 0.1f;
		}
		return (true);
	}
	return (false);
}

void	add_new_projectiles(t_server *server, t_sprite *enemy,
float angle_to_player)
{
	t_projectile	*new_projectile;

	new_projectile = gc_malloc(server->mem, sizeof(t_projectile));
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

void	shoot_at_player_server(t_sprite *enemy, t_sprite *player_pos,
t_server *server)
{
	const float	dx = player_pos->x - enemy->x;
	const float	dy = player_pos->y - enemy->y;
	const float	angle_to_player = atan2(dy, dx) * (180.0f / M_PI);
	float		angle_diff;

	angle_diff = fmodf(fabsf(angle_to_player - enemy->direction), 360.0f);
	if (angle_diff > 180.0f)
		angle_diff = 360.0f - angle_diff;
	if (enemy->animation > 0)
		enemy->animation -= 15 * server->delta_time;
	if (enemy->animation < 0)
		enemy->selected_anim = 0;
	if (angle_diff < 5.0f && enemy->shoot_delay <= 0)
	{
		enemy->selected_anim = 1;
		enemy->animation = 5;
		enemy->shoot_delay = 1;
		add_new_projectiles(server, enemy, angle_to_player);
	}
	else if (enemy->shoot_delay > 0)
		enemy->shoot_delay -= 0.6 * server->delta_time;
}

t_sprite	*get_target_player(t_server *server, t_sprite *current_enemy,
float *min_distance)
{
	t_sprite	*current;
	float		dx;
	float		dy;
	const float	distance = sqrt(dx * dx + dy * dy);

	current = server->sprites;
	while (current)
	{
		if (current->type != SPRITE_PLAYER || current->player_id < 0
			|| current->floor != current_enemy->floor)
		{
			current = current->next;
			continue ;
		}
		dx = current->x - current_enemy->x;
		dy = current->y - current_enemy->y;
		if (distance < *min_distance && distance < 10
			&& has_line_of_sight_server(server, current_enemy, current))
		{
			*min_distance = distance;
			return (current);
		}
		current = current->next;
	}
	return (NULL);
}

void	move_enemy(t_sprite *current_enemy)
{
	current_enemy->direction = rand() % 360;
	current_enemy->frame_count = 0;
}

void	move_enemy_patrol(t_sprite *current_enemy, t_server *server)
{
	float	new_x;
	float	new_y;
	float	ray[2];

	if (current_enemy->state != PATROL)
		current_enemy->state = PATROL;
	if (current_enemy->frame_count % 220 == 0)
		current_enemy->direction = rand() % 360;
	current_enemy->animation += server->delta_time;
	current_enemy->selected_anim = (int)(current_enemy->animation * 2) % 4;
	current_enemy->dir_x = cos(current_enemy->direction * (M_PI / 180));
	current_enemy->dir_y = sin(current_enemy->direction * (M_PI / 180));
	new_x = current_enemy->x + current_enemy->dir_x * 0.02;
	new_y = current_enemy->y + current_enemy->dir_y * 0.02;
	ray[0] = new_x;
	ray[1] = new_y;
	if (can_move(server->map, server->door, ray, current_enemy->floor))
	{
		current_enemy->x = new_x;
		current_enemy->y = new_y;
	}
	else
		move_enemy(current_enemy);
}

void	move_enemy_chase(t_server *server, t_sprite *current_enemy,
t_sprite *target_player, int min_distance)
{
	const float	dx = target_player->x - current_enemy->x;
	const float	dy = target_player->y - current_enemy->y;
	float		angle_in_radians;

	current_enemy->state = CHASE;
	current_enemy->direction = atan2(dy, dx) * (180 / M_PI);
	angle_in_radians = current_enemy->direction * (M_PI / 180);
	current_enemy->dir_x = cos(angle_in_radians);
	current_enemy->dir_y = sin(angle_in_radians);
	if (min_distance < 7)
		shoot_at_player_server(current_enemy, target_player, server);
}

void	update_enemies_anim(t_sprite *current_enemy, t_server *server)
{
	if (current_enemy->type == SPRITE_ENEMY && current_enemy->animation > 0)
	{
		current_enemy->animation -= server->delta_time * 2.5;
		current_enemy->selected_anim = (int)(4 - current_enemy->animation);
		if (current_enemy->selected_anim < 0)
			current_enemy->selected_anim = 0;
	}
}

void	update_enemies_server(t_server *server)
{
	t_sprite	*current_enemy;
	t_sprite	*target_player;
	float		min_distance;

	current_enemy = server->sprites;
	while (current_enemy)
	{
		if (current_enemy->type != SPRITE_ENEMY || current_enemy->health <= 0)
		{
			update_enemies_anim(current_enemy, server);
			current_enemy = current_enemy->next;
			continue ;
		}
		min_distance = INFINITY;
		target_player = get_target_player(server, current_enemy, &min_distance);
		if (!target_player)
			move_enemy_patrol(current_enemy, server);
		else
			move_enemy_chase(server, current_enemy, target_player,
				min_distance);
		current_enemy->frame_count++;
		current_enemy = current_enemy->next;
	}
}

void	use_queue_server(t_server *server)
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
