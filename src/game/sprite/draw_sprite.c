/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:04:21 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 12:04:21 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_player_in_front(t_sprite *enemy, t_player *player)
{
	const float	angle_to_player = atan2(player->y - enemy->y,
			player->x - enemy->x);
	const float	enemy_angle = atan2(enemy->dir_y, enemy->dir_x);
	const float	angle_difference = fmod(angle_to_player - enemy_angle
			+ M_PI, 2 * M_PI) - M_PI;

	return (fabs(angle_difference) < M_PI / 4);
}

void	draw_sprites_enemies(t_game *game, t_sprite *current)
{
	if (current->health <= 0)
	{
		current->dir = 0;
		draw_sprite(game, game->textures->enemy_death, current);
	}
	else
	{
		current->dir = atan2(current->dir_y, current->dir_x);
		if (current->state == CHASE
			&& is_player_in_front(current, game->player))
			draw_sprite(game, game->textures->enemy_fire, current);
		else
			draw_sprite(game, game->textures->enemy, current);
	}
}

void	draw_sprites2(t_game *game, t_sprite *current)
{
	if (current->type == SPRITE_ENEMY && current->floor == game->player->floor)
		draw_sprites_enemies(game, current);
	else if (current->type == SPRITE_AMMO && current->still_exist
		&& current->floor == game->player->floor)
		draw_sprite(game, game->textures->ammo, current);
	else if (current->type == SPRITE_HEALTH && current->still_exist
		&& current->floor == game->player->floor)
		draw_anim_health(game, current, game->textures->health);
	else if (current->type == SPRITE_PLAYER
		&& current->floor == game->player->floor)
	{
		current->dir = atan2(current->dir_y, current->dir_x);
		draw_sprite(game, game->textures->enemy, current);
	}
}

void	draw_sprite_tp(t_game *game, t_sprite *current)
{
	t_sprite	sprite;

	if (current->floor != game->player->floor
		&& current->floor1 != game->player->floor)
		return ;
	ft_memcpy(&sprite, current, sizeof(t_sprite));
	if (current->floor1 == game->player->floor)
	{
		if (current->floor == game->player->floor)
			draw_sprite(game, game->textures->tp, &sprite);
		sprite.x = sprite.x1;
		sprite.y = sprite.y1;
		sprite.floor = sprite.floor1;
	}
	draw_sprite(game, game->textures->tp, &sprite);
}

void	draw_sprites(t_game *game)
{
	t_sprite	*current;

	sort_sprites(game, &game->sprites);
	current = game->sprites;
	while (current)
	{
		if (game->menu->status == MULTI_PLAYER || game->menu->status == CHATING)
			pthread_mutex_lock(&game->game_lock);
		if (current->type == SPRITE_TELEPORTER)
			draw_sprite_tp(game, current);
		else if (current->type == SPRITE_EXIT
			&& current->floor == game->player->floor)
			draw_sprite(game, game->textures->exit, current);
		else
			draw_sprites2(game, current);
		current = current->next;
		if (game->menu->status == MULTI_PLAYER || game->menu->status == CHATING)
			pthread_mutex_unlock(&game->game_lock);
	}
}
