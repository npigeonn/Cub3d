/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:20:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 16:21:51 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#define COLLISION_THRESHOLD 0.12f

void	shoot_at_player(t_sprite *enemy, t_game *game)
{
	float	dx;
	float	dy;
	float	angle_to_player;
	float	angle_diff;

	dx = game->player->x - enemy->x;
	dy = game->player->y - enemy->y;
	angle_to_player = atan2(dy, dx) * (180.0f / M_PI);
	angle_diff = fmodf(fabsf(angle_to_player - enemy->direction), 360.0f);
	if (angle_diff > 180.0f)
		angle_diff = 360.0f - angle_diff;
	if (enemy->animation > 0)
		enemy->animation -= 15 * game->delta_time;
	if (enemy->animation < 0)
		enemy->selected_anim = 0;
	if (angle_diff < 5.0f && enemy->shoot_delay <= 0)
	{
		add_projectil(enemy, game, angle_to_player);
		enemy->selected_anim = 1;
		enemy->animation = 5;
		enemy->shoot_delay = 1;
	}
	else if (enemy->shoot_delay > 0)
		enemy->shoot_delay -= 0.6 * game->delta_time;
}

static bool	check_enemy_collision(t_game *game, t_projectile *projectile,
t_sprite *current, float ray[2])
{
	if (current->type == SPRITE_ENEMY && current->health > 0
		&& current->floor == projectile->floor
		&& current != projectile->enemy)
	{
		if (distance_squared(current->x, current->y, ray[0], ray[1])
			< COLLISION_THRESHOLD)
		{
			if (projectile->owner == game->player)
				game->player->stats->nb_hit++;
			current->health -= projectile->damage;
			if (current->health <= 0)
			{
				game->player->stats->nb_kills++;
				current->animation = 2;
			}
			return (true);
		}
	}
	return (false);
}

static bool	check_player_collision(t_game *game, t_projectile *projectile,
float x, float y)
{
	if (game->player->floor == projectile->floor
		&& projectile->owner != game->player)
	{
		if (distance_squared(game->player->x, game->player->y, x, y)
			< COLLISION_THRESHOLD)
		{
			game->player->stats->nb_degats += projectile->damage;
			game->player->health -= projectile->damage;
			pthread_mutex_lock(&game->mutex_music);
			game->player->injured = 1;
			pthread_mutex_unlock(&game->mutex_music);
			game->time_regen = 0;
			if (game->player->health <= 0)
				game->player->health = 0;
			return (true);
		}
	}
	return (false);
}

bool	check_collision_with_entity(t_game *game, t_projectile *projectile,
float ray[2])
{
	t_sprite	*current;

	current = game->sprites;
	while (current)
	{
		if (check_enemy_collision(game, projectile, current, ray))
			return (true);
		current = current->next;
	}
	if (check_player_collision(game, projectile, ray[0], ray[1]))
		return (true);
	return (false);
}
