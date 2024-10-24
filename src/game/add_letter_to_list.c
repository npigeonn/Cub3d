/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_letter_to_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:38:19 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/24 11:40:05 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_ammo(t_game *game, int x, int y, int floor)
{
	t_sprite *new_ammo;

	new_ammo = malloc(sizeof(t_sprite));
	new_ammo->x = x;
	new_ammo->y = y;
	new_ammo->floor = floor;
	new_ammo->still_exist = 1;
	new_ammo->next = game->sprites;
	new_ammo->type = SPRITE_AMMO;
	game->sprites = new_ammo;
}

void	add_health(t_game *game, int x, int y, int floor)
{
	t_sprite *new_health;

	new_health = malloc(sizeof(t_sprite));
	new_health->x = x;
	new_health->y = y;
	new_health->floor = floor;
	new_health->still_exist = 1;
	new_health->next = game->sprites;
	new_health->type = SPRITE_HEALTH;
	game->sprites = new_health;
}

void	add_exit(t_game *game, int x, int y, int floor)
{
	t_sprite *new_exit;

	new_exit = malloc(sizeof(t_sprite));
	new_exit->x = x;
	new_exit->y = y;
	new_exit->floor = floor;
	new_exit->next = game->sprites;
	new_exit->type = SPRITE_EXIT;
	game->sprites = new_exit;
}

void	door_ennemi_ammo_health_mngmt(t_game *game)
{
	int	i;
	int	j;
	int	k;
	
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			k = -1;
			while (game->map[i][j][++k])
			{
				if (game->map[i][j][k] == 'D')
					add_door(game, k, j, i);
				else if (game->map[i][j][k] == 'M')
					add_ammo(game, k + 0.5, j + 0.5, i);
				else if (game->map[i][j][k] == 'B')
					add_enemies(game, k, j, i);
				else if (game->map[i][j][k] == 'H')
					add_health(game, k, j + 0.5, i + 0.5);
				else if (game->map[i][j][k] == 'e')
					add_exit(game, k, j + 0.5, i + 0.5);
			}
		}
	}
}
