/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:20:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/15 12:28:20 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	get_enemies_id(t_game *game)
{
	int			id;
	t_sprite	*current;

	id = 0;
	current = game->sprites;
	while (current)
	{
		if (current->type == SPRITE_ENEMY)
			id++;
		current = current->next;
	}
	return (id);
}

void	add_enemies(t_game *game, int x, int y, int floor)
{
	t_sprite	*new;
	const int	id = get_enemies_id(game);

	new = gc_malloc(game->mem, sizeof(t_sprite));
	ft_bzero(new, sizeof(t_sprite));
	new->x = x + 0.5;
	new->y = y + 0.5;
	new->floor = floor;
	new->health = 1;
	new->dir_x = 0;
	new->dir_y = 1;
	new->next = game->sprites;
	new->state = PATROL;
	new->direction = 200;
	new->frame_count = 0;
	new->player_id = id;
	new->fov = 60;
	new->shoot_delay = 0;
	new->type = SPRITE_ENEMY;
	new->animation = 0;
	new->dir = 0;
	new->scale = 1;
	new->z_offset = 0;
	new->spritecast = gc_malloc(game->mem, sizeof(t_spritecast));
	game->sprites = new;
}

bool	has_line_of_sight(t_game *game, t_sprite *sprite)
{
	float dx = game->player->x - sprite->x;
	float dy = game->player->y - sprite->y;
	float distance = sqrt(dx * dx + dy * dy);

	float angle_to_player = atan2(dy, dx) * (180.0f / M_PI);
	float angle_diff = fabsf(angle_to_player - sprite->dir);

	if (angle_diff <= sprite->fov * 0.5)
	{
		float step_x = dx / distance;
		float step_y = dy / distance;
		for (float t = 0; t < distance; t += 0.1f)
		{
			sprite->x += step_x * 0.1f;
			sprite->y += step_y * 0.1f;
			if (!can_move(game->map, game->door, sprite->x, sprite->y, sprite->floor))
				return (false);
		}
		return (true);
	}
	return (false);
}

void	damages_red_draw(t_game *game, int y)
{
	int		x;
	float	alpha;
	float	max_dist_x;
	float	max_dist_y;

	max_dist_x =  game->cen_x * 0.5;
	max_dist_y = game->cen_y * 0.5;
	alpha = 0.2 * (1 - game->player->health);
	float dist_y = abs(y - game->cen_y) / max_dist_y;

	x = -1;
	while (++x <= game->screen_width)
		pixel_put(game, x, y,  blend_colors(get_pixel_color_from_image(game,
			x, y), 9830400, alpha * (1 + abs(x -  game->cen_x)
			/ max_dist_x + dist_y)));
}

#define COLLISION_THRESHOLD 0.12f

static inline float distance_squared(float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	return (dx * dx + dy * dy);
}

bool	check_collision_with_entity(t_game *game, t_projectile *projectile, float x, float y)
{
	t_sprite *current = game->sprites;

	while (current)
	{
		if (current->type == SPRITE_ENEMY && current->health > 0 && current->floor == projectile->floor && current != projectile->enemy)
		{
			if (distance_squared(current->x, current->y, x, y) < COLLISION_THRESHOLD)
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
		current = current->next;
	}
	if (game->player->floor == projectile->floor && projectile->owner != game->player)
	{
		if (distance_squared(game->player->x, game->player->y, x, y) < COLLISION_THRESHOLD)
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

void	projectile_move(t_game *game, t_projectile *projectile, bool *collision)
{
	float	x_ray;
	float	y_ray;
	float	distance;
	
	distance = sqrt((cos(projectile->direction * (M_PI / 180.0f)) * projectile->speed)
		* (cos(projectile->direction * (M_PI / 180.0f)) * projectile->speed)
		+ (sin(projectile->direction * (M_PI / 180.0f)) * projectile->speed)
		* (sin(projectile->direction * (M_PI / 180.0f)) * projectile->speed));
	x_ray = projectile->x;
	y_ray = projectile->y;
	for (float traveled = 0; traveled < distance; traveled += 0.1)
	{
		x_ray += cos(projectile->direction * (M_PI / 180.0f)) * 0.1;
		y_ray += sin(projectile->direction * (M_PI / 180.0f)) * 0.1;

		if (!can_move(game->map, game->door, x_ray, y_ray, projectile->floor)
			|| check_collision_with_entity(game, projectile, x_ray, y_ray))
		{
			*collision = true;
			break;
		}
		projectile->x = x_ray;
		projectile->y = y_ray;
	}
}

void	projectile_collision(t_game *game, t_projectile **projectile,
t_projectile **prev, int collision)
{
	t_projectile	*temp;

	if (collision)
	{
		temp = *projectile;
		if (*prev)
			(*prev)->next = (*projectile)->next;
		else
			game->projectiles = (*projectile)->next;
		*projectile = (*projectile)->next;
		gc_free(game->mem, temp);
	}
	else
	{
		*prev = *projectile;
		*projectile = (*projectile)->next;
	}
}

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

void	add_projectil(t_sprite *enemy, t_game *game, float angle_to_player)
{
	t_projectile	*new_projectile;

	new_projectile = gc_malloc(game->mem, sizeof(t_projectile));
	new_projectile->x = enemy->x;
	new_projectile->y = enemy->y;
	new_projectile->direction = angle_to_player;
	new_projectile->speed = 2000;
	new_projectile->next = NULL;
	new_projectile->owner = NULL;
	new_projectile->enemy = enemy;
	new_projectile->next = game->projectiles;
	new_projectile->floor = enemy->floor;
	new_projectile->damage = 0.09f;
	game->projectiles = new_projectile;
}

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

void	death_enemies_animation(t_game *game, t_sprite *current)
{
	if (current->type == SPRITE_ENEMY && current->animation > 0)
	{
		current->animation -= game->delta_time * 2.5;
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
}

void	init_update(t_game *game, t_sprite *current)
{
	t_spritecast	*sp;

	sp = current->spritecast;
	sp->dx = game->player->x - current->x;
	sp->dy = game->player->y - current->y;
	sp->distance_squared = sp->dx * sp->dx + sp->dy * sp->dy;
	sp->distance = sqrt(sp->distance_squared);
}

void	update_enemies_patrol_move(t_game *game, t_sprite *current)
{
	float	new_x;
	float	new_y;

	new_x = current->x + current->dir_x * 0.02;
	new_y = current->y + current->dir_y * 0.02;
	if (can_move(game->map, game->door, new_x, new_y, current->floor))
	{
		current->x = new_x;
		current->y = new_y;
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
