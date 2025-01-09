/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:59:37 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/09 13:08:08 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	move_enemy(t_sprite *current_enemy)
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
		server->current_enemy = current_enemy;
		target_player = get_target_player(server, &min_distance);
		if (!target_player)
			move_enemy_patrol(current_enemy, server);
		else
			move_enemy_chase(server, current_enemy, target_player,
				min_distance);
		current_enemy->frame_count++;
		current_enemy = current_enemy->next;
	}
}
