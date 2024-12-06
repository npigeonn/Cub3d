/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:16:55 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/29 17:37:43 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#define COLLISION 0.25f

static inline float	distance_squared(float x1, float y1, float x2, float y2)
{
	const float	dx = x2 - x1;
	const float	dy = y2 - y1;

	return (dx * dx + dy * dy);
}

void	send_heal_server(t_server *server, t_sprite *current)
{
	t_game_message	msg;

	ft_bzero(&msg, sizeof(t_game_message));
	msg.type = MSG_PLAYER_HIT;
	msg.health = current->health;
	msg.player_id = current->player_id;
	add_game_message_to_queue(server, msg);
}

bool	check_collision_with_entity_server(t_server *server,
t_projectile *projectile, float x, float y)
{
	t_sprite	*current;

	current = server->sprites;
	while (current)
	{
		if ((current->type == SPRITE_ENEMY || current->type == SPRITE_PLAYER)
			&& current->health > 0 && current->floor == projectile->floor
			&& current != projectile->enemy)
		{
			if (distance_squared(current->x, current->y, x, y) < COLLISION)
			{
				current->health -= projectile->damage;
				if (current->type == SPRITE_PLAYER)
					send_heal_server(server, current);
				if (current->health <= 0)
					current->animation = 2;
				return (true);
			}
		}
		current = current->next;
	}
	return (false);
}

static void	move_projectile(t_projectile *current, t_server *server, float step,
int square)
{
	float	traveled;
	float	ray[2];

	ray[0] = current->x;
	ray[1] = current->y;
	traveled = 0;
	while (traveled < square)
	{
		ray[0] += cos(current->direction * (M_PI / 180.0f)) * step;
		ray[1] += sin(current->direction * (M_PI / 180.0f)) * step;
		if (!can_move(server->map, server->door, ray, current->floor)
			|| check_collision_with_entity_server(server, current,
				ray[0], ray[1]))
			break ;
		current->x = ray[0];
		current->y = ray[1];
		traveled += step;
	}
}

static void	handle_collision(t_server *server, t_projectile *current,
t_projectile *prev)
{
	if (prev)
		prev->next = current->next;
	else
		server->projectiles = current->next;
	gc_free(server->mem, current);
}

void	update_projectiles_server(t_server *server)
{
	t_projectile	*current;
	t_projectile	*prev;
	float			dx;
	float			dy;
	bool			collision;

	current = server->projectiles;
	prev = NULL;
	while (current)
	{
		dx = cos(current->direction * (M_PI / 180.0f)) * current->speed;
		dy = sin(current->direction * (M_PI / 180.0f)) * current->speed;
		collision = false;
		move_projectile(current, server, 0.1f, sqrt(dx * dx + dy * dy));
		if (collision)
			handle_collision(server, current, prev);
		else
		{
			current->x += dx;
			current->y += dy;
			prev = current;
			current = current->next;
		}
	}
}
