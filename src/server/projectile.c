/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:16:55 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/06 09:45:19 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#define COLLISION_THRESHOLD 0.25f

bool	check_collision_with_entity_server(t_server *server, t_projectile *projectile, float x, float y)
{
	t_sprite *current = server->sprites;

	while (current)
	{
		if (current->type == SPRITE_ENEMY && current->health > 0 && current->floor == projectile->floor && current != projectile->enemy)
		{
			if (distance_squared(current->x, current->y, x, y) < COLLISION_THRESHOLD)
			{
				if (projectile->owner == server->player)
					server->player->stats->nb_hit++;
				current->health -= projectile->damage;
				if (current->health <= 0)
				{
					server->player->stats->nb_kills++;
					current->animation = 2;
				}
				return (true);
			}
		}
		current = current->next;
	}
	if (server->player->floor == projectile->floor && projectile->owner != server->player)
	{
		if (distance_squared(server->player->x, server->player->y, x, y) < COLLISION_THRESHOLD)
		{
			server->player->stats->nb_degats += projectile->damage;
			server->player->health -= projectile->damage;
			server->time_regen = 0;
			if (server->player->health <= 0)
				server->player->health = 0;
			damages_red_draw(server);
			return (true);
		}
	}
	return (false);
}

void	update_projectiles_server(t_server *server)
{
	t_projectile *current = server->projectiles;
	t_projectile *prev = NULL;

	while (current)
	{
		float x_old = current->x;
		float y_old = current->y;

		float dx = cos(current->direction * (M_PI / 180.0f)) * current->speed;
		float dy = sin(current->direction * (M_PI / 180.0f)) * current->speed;

		float distance = sqrt(dx * dx + dy * dy);
		bool collision = false;

		float x_ray = x_old;
		float y_ray = y_old;
		float step = 0.1f;

		for (float traveled = 0; traveled < distance; traveled += step)
		{
			x_ray += cos(current->direction * (M_PI / 180.0f)) * step;
			y_ray += sin(current->direction * (M_PI / 180.0f)) * step;

			if (!can_move(server->map, server->door, x_ray, y_ray, current->floor) || check_collision_with_entity_server(server, current, x_ray, y_ray))
			{
				collision = true;
				break;
			}
			current->x = x_ray;
			current->y = y_ray;
		}
		if (!collision)
		{
			current->x = x_old + dx;
			current->y = y_old + dy;
		}
		if (collision)
		{
			if (prev)
				prev->next = current->next;
			else
				server->projectiles = current->next;

			t_projectile *temp = current;
			current = current->next;
			free(temp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
