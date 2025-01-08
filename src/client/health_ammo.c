/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_ammo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:42:01 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 13:13:14 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_player_health(t_game *game, t_game_message msg)
{
	t_sprite	*current;

	current = game->sprites;
	if (msg.player_id == game->client->player_id)
	{
		game->player->health = msg.health;
		game->time_regen = 0;
		return ;
	}
	while (current)
	{
		if (current->type == SPRITE_PLAYER
			&& current->player_id == msg.player_id)
		{
			current->health = msg.health;
			break ;
		}
		current = current->next;
	}
}

void	destroy_sprite_health(t_game *game, t_sprite *current,
t_game_message msg)
{
	t_player	*player;

	player = find_player_by_id(game->sprites, msg.player_id);
	if (player)
	{
		if (current->type == SPRITE_AMMO)
			player->ammo = msg.ammo;
		else
			player->health = msg.health;
	}
}

void	destroy_sprite(t_game *game, t_game_message msg)
{
	t_sprite	*current;
	t_sprite	*prev;

	current = game->sprites;
	while (current)
	{
		if (current->x == msg.x && current->y == msg.y
			&& current->floor == msg.floor && (current->type == SPRITE_AMMO
				|| current->type == SPRITE_HEALTH) && current->still_exist)
		{
			current->still_exist = 0;
			if (msg.player_id == game->client->player_id)
			{
				if (current->type == SPRITE_AMMO)
					game->player->ammo = msg.ammo;
				else
					game->player->health = msg.health;
			}
			else
				destroy_sprite_health(game, current, msg);
		}
		current = current->next;
	}
}
