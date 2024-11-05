/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:16:55 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/05 11:21:10 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#define COLLISION_THRESHOLD 0.25f

bool	check_collision_with_entity_server(t_server *server, t_projectile *projectile)
{
	t_sprite	*current = server->sprites;

	while (current)
	{
		if (current->type == SPRITE_ENEMY && current->health > 0 && current->floor == projectile->floor && current != projectile->enemy)
		{
			if (distance_squared(current->x, current->y, projectile->x, projectile->y) < COLLISION_THRESHOLD)
			{
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
		if (distance_squared(server->player->x, server->player->y, projectile->x, projectile->y) < COLLISION_THRESHOLD)
		{
			server->player->stats->nb_degats += projectile->damage;
			server->player->health -= projectile->damage;
			server->time_regen = 0;
			if (server->player->health <= 0) {
				server->player->health = 0;
			}
			damages_red_draw(game);
			return (true);
		}
	}
	return (false);
}

void update_projectiles_server(t_server *server)
{
	t_projectile *current = server->projectiles;
	t_projectile *prev = NULL;

	while (current)
	{
		float x_old = current->x;
		float y_old = current->y;

		float dx = cos(current->direction * (M_PI / 180.0f)) * current->speed;
		float dy = sin(current->direction * (M_PI / 180.0f)) * current->speed;

		float total_distance = sqrt(dx * dx + dy * dy);
		float step_distance = fmax(0.1f, total_distance / 100.0f);
		int steps = (int)(total_distance / step_distance) + 1;
		bool collision = false;
		
		for (int i = 1; i <= steps; i++)
		{
			float t = (float)i / steps;
			float x_check = x_old + dx * t;
			float y_check = y_old + dy * t;

			if (!can_move(server->map, server->door, x_check, y_check, current->floor) || check_collision_with_entity_server(server, current))
			{
				collision = true;
				break ;
			}
			current->x = x_check;
			current->y = y_check;
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
