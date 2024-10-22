/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shoot.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:17:07 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/22 11:24:23 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

bool	check_collision_with_entity(t_game *game, t_projectile *projectile)
{
	t_enemy *current = game->enemies;

	while (current)
	{
		if (current->floor != projectile->floor || current == projectile->enemy)
		{
			current = current->next;
			continue;
		}
		float dx = current->x - projectile->x;
		float dy = current->y - projectile->y;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance < 0.5f)
		{
			current->health -= projectile->damage;
			return (true);
		}
		current = current->next;
	}
	if (game->player->floor == projectile->floor && projectile->owner != game->player)
	{
		float dx = game->player->x - projectile->x;
		float dy = game->player->y - projectile->y;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance < 0.5f)
		{
			game->player->health -= projectile->damage;
			return (true);
		}
	}
	return (false);
}

void	update_projectiles(t_game *game)
{
	t_projectile *current = game->projectiles;
	t_projectile *prev = NULL;

	while (current)
	{
		current->x += cos(current->direction * (M_PI / 180.0f)) * current->speed;
		current->y += sin(current->direction * (M_PI / 180.0f)) * current->speed;

		if (!is_position_passable(game, current->x, current->y, current->floor))
		{
			if (prev)
				prev->next = current->next;
			else
				game->projectiles = current->next;
			t_projectile *temp = current;
			current = current->next;
			free(temp);
		}
		else
		{
			if (check_collision_with_entity(game, current))
			{
				if (prev)
					prev->next = current->next;
				else
					game->projectiles = current->next;
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
}

void	shoot_at_player(t_enemy *enemy, t_point player_pos, t_game *game)
{
	float dx = player_pos.x - enemy->x;
	float dy = player_pos.y - enemy->y;
	float angle_to_player = atan2(dy, dx) * (180.0f / M_PI);
	float angle_diff = fmodf(fabsf(angle_to_player - enemy->direction), 360.0f);
		
	if (angle_diff > 180.0f)
		angle_diff = 360.0f - angle_diff;
	if (angle_diff < 5.0f && enemy->shoot_delay <= 0)
	{
		t_projectile *new_projectile = malloc(sizeof(t_projectile));
		
		if (new_projectile)
		{
			new_projectile->x = enemy->x;
			new_projectile->y = enemy->y;
			new_projectile->direction = angle_to_player;
			new_projectile->speed = 0.4f;
			new_projectile->next = NULL;
			new_projectile->owner = NULL;
			new_projectile->enemy = enemy;
			new_projectile->next = game->projectiles; 
			new_projectile->floor = enemy->floor;
			new_projectile->damage = 0.05f;
			game->projectiles = new_projectile;
		}
		enemy->shoot_delay = 1;
	}
	else if (enemy->shoot_delay > 0)
		enemy->shoot_delay -= 0.6 * game->delta_time;
}
