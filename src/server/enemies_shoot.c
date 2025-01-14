/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_shoot.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:05:01 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/14 09:24:01 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	sprite->ammo--;
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
	if (fabsf(fmodf(atan2(dy, dx) * (180.0f / M_PI) - enemy->direction
				+ 360.0f, 360.0f) - 180.0f)
		<= enemy->fov * 0.5f)
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

static t_sprite	*init_get_player(t_server *server, float *dx, float *dy,
t_sprite *current)
{
	t_sprite	*copy;

	*dx = current->x - server->current_enemy->x;
	*dy = current->y - server->current_enemy->y;
	
	copy = gc_malloc(server->mem, sizeof(t_sprite));
	ft_memcpy(copy, current, sizeof(t_sprite));
	return (copy);
}

t_sprite	*get_target_player(t_server *server, float *distance, t_sprite *current_enemy)
{
	t_sprite	*current;
	t_sprite	*copy;
	float		dx;
	float		dy;

	current = server->sprites;
	while (current)
	{
		if (current->type != SPRITE_PLAYER || current->player_id < 0
			|| current->floor != current_enemy->floor)
		{
			current = current->next;
			continue ;
		}
		copy = init_get_player(server, &dx, &dy, current);
		if (sqrt(dx * dx + dy * dy) < *distance && sqrt(dx * dx + dy * dy) < 10
			&& has_line_of_sight_server(server, copy, current))
		{
			*distance = sqrt(dx * dx + dy * dy);
			return (gc_free(server->mem, copy), current);
		}
		current = current->next;
		gc_free(server->mem, copy);
	}
	return (NULL);
}
