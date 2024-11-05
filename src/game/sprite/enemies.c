/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:20:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/05 08:57:04 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_enemies(t_game *game, int x, int y, int floor)
{
	t_sprite	*new;

	new = malloc(sizeof(t_sprite));
	new->x = x + 0.5;
	new->y = y + 0.5;
	new->floor = floor;
	new->health = 1;
	new->dirX = 0;
	new->dirY = 1;
	new->next = game->sprites;
	new->state = PATROL;
	new->direction = 200;
	new->frame_count = 0;
	new->fov = 60;
	new->shoot_delay = 0;
	new->type = SPRITE_ENEMY;
	new->animation = 0;
	game->sprites = new;
}

void	draw_players(t_game *game)
{
	t_player_info	*current;

	current = game->client->players;
	while (current)
	{
		if (current->floor == game->player->floor)
			draw_sprite(game, game->textures->enemy, current->x, current->y, 150, 1, 0, 0);
		current = current->next;
	}
}

bool	has_line_of_sight(t_game *game, t_point enemy_pos, t_point player_pos, float enemy_facing_angle, float fov_angle)
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
			if (!can_move(game, enemy_pos.x, enemy_pos.y, enemy_pos.floor))
				return (false);
		}
		return (true);
	}
	return (false);
}

void	damages_red_draw(t_game *game)
{
	int		x;
	int		y;
	float	alpha;
	float	max_dist_x;
	float	max_dist_y;
	int		center_x;
	int		center_y;
	
	center_x = game->screen_width * 0.5;
	center_y = game->screen_height * 0.5;
	max_dist_x = center_x * 0.5;
	max_dist_y = center_y * 0.5;
	alpha = 0.2 * (1 - game->player->health);
	y = -1;
	while (++y <= game->screen_height)
	{
		float dist_y = abs(y - center_y) / max_dist_y;

		x = -1;
		while (++x <= game->screen_width)
			pixel_put(game, x, y,  blend_colors(get_pixel_color_from_image(game,
				x, y), 9830400, alpha * (1 + abs(x - center_x)
				/ max_dist_x + dist_y)));
	}
}

bool	check_collision_with_entity(t_game *game, t_projectile *projectile)
{
	t_sprite *current = game->sprites;

	while (current)
	{
		if (current->type != SPRITE_ENEMY || current->health <= 0)
		{
			current = current->next;
			continue;
		}
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
		current = current->next;
	}
	if (game->player->floor == projectile->floor && projectile->owner != game->player)
	{
		float dx = game->player->x - projectile->x;
		float dy = game->player->y - projectile->y;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance < 0.5f)
		{
			game->player->stats->nb_degats += projectile->damage;
			game->player->health -= projectile->damage;
			game->time_regen = 0;
			if (game->player->health <= 0)
				game->player->health = 0;
			damages_red_draw(game);
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
		float x_old = current->x;
		float y_old = current->y;

		current->x += cos(current->direction * (M_PI / 180.0f)) * current->speed;
		current->y += sin(current->direction * (M_PI / 180.0f)) * current->speed;

		float dx = current->x - x_old;
		float dy = current->y - y_old;
		float distance = sqrt(dx * dx + dy * dy);

		int steps = (int)(distance / 0.1f) + 1;
		bool collision = false;

		float x_check = x_old;
		float y_check = y_old;

		for (int i = 0; i < steps; i++)
		{
			x_check = x_old + dx * ((float)i / steps);
			y_check = y_old + dy * ((float)i / steps);
			current->x = x_check;
			current->y = y_check;
			if (!can_move(game, current->x, current->y, current->floor) || check_collision_with_entity(game, current))
			{
				collision = true;
				break;
			}
		}
		if (!collision)
		{
			current->x = x_check;
			current->y = y_check;
		}

		if (collision)
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

void	shoot_at_player(t_sprite *enemy, t_point player_pos, t_game *game)
{
	float dx = player_pos.x - enemy->x;
	float dy = player_pos.y - enemy->y;
	float angle_to_player = atan2(dy, dx) * (180.0f / M_PI);
	float angle_diff = fmodf(fabsf(angle_to_player - enemy->direction), 360.0f);
		
	if (angle_diff > 180.0f)
		angle_diff = 360.0f - angle_diff;
	if (enemy->animation > 0)
		enemy->animation -= 15 * game->delta_time;
	if (enemy->animation < 0)
		enemy->selected_anim = 0;
	if (angle_diff < 5.0f && enemy->shoot_delay <= 0)
	{
		t_projectile *new_projectile = malloc(sizeof(t_projectile));
		enemy->selected_anim = 1;
		enemy->animation = 5;
		
		if (new_projectile)
		{
			new_projectile->x = enemy->x;
			new_projectile->y = enemy->y;
			new_projectile->direction = angle_to_player;
			new_projectile->speed = 25;
			new_projectile->next = NULL;
			new_projectile->owner = NULL;
			new_projectile->enemy = enemy;
			new_projectile->next = game->projectiles; 
			new_projectile->floor = enemy->floor;
			new_projectile->damage = 0.09f;
			game->projectiles = new_projectile;
		}
		enemy->shoot_delay = 1;
	}
	else if (enemy->shoot_delay > 0)
		enemy->shoot_delay -= 0.6 * game->delta_time;
}

void	update_enemies(t_game *game)
{
	t_sprite *current = game->sprites;
	t_point player_pos = {game->player->x, game->player->y, game->player->floor};

	while (current)
	{
		if (current->type != SPRITE_ENEMY || current->health <= 0)
		{
			if (current->type == SPRITE_ENEMY && current->animation > 0)
			{
				current->animation -=  game->delta_time * 2.5;
				if (current->animation <= 0)
					current->selected_anim = 4;
				else if (current->animation <= 0.5)
					current->selected_anim = 3;
				else if (current->animation <= 1)
					current->selected_anim = 2;
				else if (current->animation <= 1.5)
					current->selected_anim = 1;
				else if (current->animation <= 2)
					current->selected_anim = 0;
			}
			current = current->next;
			continue ;
		}
		t_point enemy_pos = {current->x, current->y, current->floor};
		
		float dx = player_pos.x - current->x;
		float dy = player_pos.y - current->y;
		float distance_squared = dx * dx + dy * dy;
		float distance = sqrt(distance_squared);

		if (current->state == PATROL)
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
			
			float angle_in_radians = current->direction * (M_PI / 180);
			current->dirX = cos(angle_in_radians);
			current->dirY = sin(angle_in_radians);
			
			float new_x = current->x + current->dirX * 0.02;
			float new_y = current->y + current->dirY * 0.02;
			if (can_move(game, new_x, new_y, current->floor))
			{
				current->x = new_x;
				current->y = new_y;
			}
			else
			{
				current->direction = rand() % 360;
				current->frame_count = 0;
			}
			if (current->floor == game->player->floor && distance < 5 && has_line_of_sight(game, enemy_pos, player_pos, current->direction, current->fov))
			{
				current->state = CHASE;
				current->selected_anim = 0;
			}
		}
		else if (current->state == CHASE)
		{
			float dx = game->player->x - current->x;
			float dy = game->player->y - current->y;
			current->direction = atan2(dy, dx) * (180 / M_PI);
			float angle_in_radians = current->direction * (M_PI / 180);
			current->dirX = cos(angle_in_radians);
			current->dirY = sin(angle_in_radians);
			
			if (distance < 7 && current->floor == game->player->floor)
				shoot_at_player(current, player_pos, game);
			else 
				current->state = PATROL;
		}
		current->frame_count++;
		current = current->next;
	}
}
