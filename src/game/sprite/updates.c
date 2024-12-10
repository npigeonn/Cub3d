/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   updates.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:30:22 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 11:30:22 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_projectiles(t_game *game)
{
	t_projectile	*current;
	t_projectile	*prev;
	float			x_old;
	float			y_old;
	bool			collision;

	current = game->projectiles;
	prev = NULL;
	while (current)
	{
		x_old = current->x;
		y_old = current->y;
		collision = false;
		projectile_move(game, current, &collision);
		if (!collision)
		{
			current->x = x_old + cos(current->direction
					* (M_PI / 180.0f)) * current->speed;
			current->y = y_old + sin(current->direction
					* (M_PI / 180.0f)) * current->speed;
		}
		projectile_collision(game, &current, &prev, collision);
	}
}

void	update_enemies_patrol_move(t_game *game, t_sprite *current)
{
	float	ray[2];

	ray[0] = current->x + current->dir_x * 0.02;
	ray[1] = current->y + current->dir_y * 0.02;
	if (can_move(game->map, game->door, ray, current->floor))
	{
		current->x = ray[0];
		current->y = ray[1];
	}
	else
	{
		current->direction = rand() % 360;
		current->frame_count = 0;
	}
	if (current->floor == game->player->floor && current->spritecast->distance
		< 5 && has_line_of_sight(game, current))
	{
		current->state = CHASE;
		current->selected_anim = 0;
	}
}

void	update_enemies_patrol(t_game *game, t_sprite *current)
{
	if (current->frame_count % 220 == 0)
		current->direction = rand() % 360;
	current->animation += game->delta_time;
	if (current->animation >= 2)
		current->animation = 0.5;
	if (current->animation >= 0.5 && current->animation < 1)
		current->selected_anim = 1;
	else if (current->animation >= 1 && current->animation < 1.5)
		current->selected_anim = 2;
	else if (current->animation >= 1.5 && current->animation < 2)
		current->selected_anim = 3;
	current->spritecast->angle_in_radians = current->direction * (M_PI / 180);
	current->dir_x = cos(current->spritecast->angle_in_radians);
	current->dir_y = sin(current->spritecast->angle_in_radians);
	update_enemies_patrol_move(game, current);
}

void	update_enemies_chase(t_game *game, t_sprite *current)
{
	float	dx;
	float	dy;
	float	angle_in_radians;

	dx = game->player->x - current->x;
	dy = game->player->y - current->y;
	angle_in_radians = current->direction * (M_PI / 180);
	current->direction = atan2(dy, dx) * (180 / M_PI);
	current->dir_x = cos(angle_in_radians);
	current->dir_y = sin(angle_in_radians);
	if (current->spritecast->distance < 7
		&& current->floor == game->player->floor)
		shoot_at_player(current, game);
	else
		current->state = PATROL;
}

void	update_enemies(t_game *game)
{
	t_sprite	*current;

	current = game->sprites;
	while (current)
	{
		if (current->type != SPRITE_ENEMY || current->health <= 0)
		{
			death_enemies_animation(game, current);
			current = current->next;
			continue ;
		}
		init_update(game, current);
		if (current->state == PATROL)
			update_enemies_patrol(game, current);
		else if (current->state == CHASE)
			update_enemies_chase(game, current);
		current->frame_count++;
		current = current->next;
	}
}
