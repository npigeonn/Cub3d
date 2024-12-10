/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_ennemies.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:21:03 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 11:21:03 by npigeon          ###   ########.fr       */
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

void	init_update(t_game *game, t_sprite *current)
{
	t_spritecast	*sp;

	sp = current->spritecast;
	sp->dx = game->player->x - current->x;
	sp->dy = game->player->y - current->y;
	sp->distance_squared = sp->dx * sp->dx + sp->dy * sp->dy;
	sp->distance = sqrt(sp->distance_squared);
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
