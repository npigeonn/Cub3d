/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_letter_to_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:38:19 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/06 08:33:58 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_ammo(t_game *game, int x, int y, int floor)
{
	t_sprite *new_ammo;

	new_ammo = gc_malloc(game->mem, sizeof(t_sprite));
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

	new_health = gc_malloc(game->mem, sizeof(t_sprite));
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

	new_exit = gc_malloc(game->mem, sizeof(t_sprite));
	new_exit->x = x;
	new_exit->y = y;
	new_exit->floor = floor;
	new_exit->next = game->sprites;
	new_exit->type = SPRITE_EXIT;
	game->sprites = new_exit;
}

void	door_ennemi_ammo_health_mngmt(t_game *game)
{
	int	i[3];
	
	i[0] = -1;
	while (game->map[++i[0]])
	{
		i[1] = -1;
		while (game->map[i[0]][++i[1]])
		{
			i[2] = -1;
			while (game->map[i[0]][i[1]][++i[2]])
			{
				if (game->map[i[0]][i[1]][i[2]] == 'D')
					add_door(game, i[2], i[1], i);
				else if (game->map[i[0]][i[1]][i[2]] == 'M')
					add_ammo(game, i[2] + 0.5, i[1] + 0.5, i);
				else if (game->map[i[0]][i[1]][i[2]] == 'B')
					add_enemies(game, i[2], i[1], i);
				else if (game->map[i[0]][i[1]][i[2]] == 'H')
					add_health(game, i[2], i[1] + 0.5, i);
				else if (game->map[i[0]][i[1]][i[2]] == 'e')
					add_exit(game, i[2], i[1] + 0.5, i);
				// 	add_health(game, i[2], i[1] + 0.5, i + 0.5);
				// else if (game->map[i[0]][i[1]][i[2]] == 'e')
				// 	add_exit(game, i[2], i[1] + 0.5, i + 0.5);
			}
		}
	}
}
