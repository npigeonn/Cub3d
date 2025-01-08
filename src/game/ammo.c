/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ammo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:09:22 by npigeon           #+#    #+#             */
/*   Updated: 2025/01/08 14:54:33 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	ammo_written(t_game *game)
{
	char		*str;
	t_draw_info	info;

	str = gc_itoa(game->mem, game->player->ammo);
	info = init_draw_info(game->screen_height * 0.05, str,
			game->screen_width * 0.82, game->screen_height * 0.885);
	info.color = 16777215;
	draw_text_right(game, info);
	info = init_draw_info(game->screen_height * 0.06, "",
			game->screen_width * 0.81, game->screen_height * 0.875);
	draw_image(game, game->textures->bullet, info);
	gc_free(game->mem, str);
}

int	big_if_ammo(t_game *game)
{
	if (game->map[game->player->floor]
		[(int)game->player->y][(int)game->player->x] == 'M'
		|| game->map[game->player->floor]
		[(int)game->player->y - 1][(int)game->player->x] == 'M'
		|| game->map[game->player->floor]
		[(int)game->player->y + 1][(int)game->player->x] == 'M'
		|| game->map[game->player->floor]
		[(int)game->player->y][(int)game->player->x + 1] == 'M'
		|| game->map[game->player->floor]
		[(int)game->player->y][(int)game->player->x - 1] == 'M')
		return (1);
	else
		return (0);
}

void	on_ammo(t_game *game)
{
	t_sprite	*current;

	current = game->sprites;
	if (big_if_ammo(game))
	{
		while (current)
		{
			if ((int)game->player->x <= current->x + 1
				&& (int)game->player->x >= current->x - 1
				&& (int)game->player->y <= current->y + 1
				&& (int)game->player->y >= current->y - 1
				&& current->type == SPRITE_AMMO && current->still_exist == 1)
			{
				current->still_exist = 0;
				game->player->ammo += 25;
				pthread_mutex_lock(&game->mutex_music);
				game->player->picking_up_ammo = 1;
				pthread_mutex_unlock(&game->mutex_music);
				return ;
			}
			current = current->next;
		}
	}
}
