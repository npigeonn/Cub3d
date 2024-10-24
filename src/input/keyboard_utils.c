/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 03:03:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/24 15:51:35 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	use_item(t_game *game)
{
	use_door_in_view(game);
	if (game->menu->message == TELEPORT)
	{
		game->player->x_tel = (int)game->player->x;
		game->player->y_tel = (int)game->player->y;
		game->player->f_tel = (int)game->player->floor;		
		game->player->being_tpted = 1;
		// animation_teleportation(game);
		// use_teleporter(game);
	}
}

int	tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

int	can_move(t_game *game, float x, float y, int floor)
{
	t_door		*door;
	int			check_x;
	int			check_y;
	const float	buffer = 0.13;

	check_y = (int)(y - buffer) - 1;
	while (++check_y <= (int)(y + buffer))
	{
		check_x = (int)(x - buffer) - 1;
		while (++check_x <= (int)(x + buffer))
		{
			if (check_x < 0 || check_y < 0 || check_y >= tablen(game->map[floor]) || check_x >= (int)ft_strlen(game->map[floor][check_y]))
				return (false);
			if (game->map[floor][check_y][check_x] == '1')
				return (false);
			if (game->map[floor][check_y][check_x] == 'D')
			{
				door = get_door(game, check_x, check_y, floor);
				if (!door->open)
					return (false);
			}
		}
	}
	return (true);  
}

void	chatting_mode(t_game *game)
{
	if (game->menu->status == MULTI_PLAYER)
	{
		game->menu->status = CHATING;
		mlx_mouse_show(game->mlx, game->win);
	}
	else
	{
		game->menu->status = MULTI_PLAYER;
		mlx_mouse_hide(game->mlx, game->win);
	}
	game->chatbox->visible = !game->chatbox->visible;
}
