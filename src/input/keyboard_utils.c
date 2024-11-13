/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 03:03:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/13 10:13:50 by npigeon          ###   ########.fr       */
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
		game->player->telep_signal = 1;
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

int	can_move(char ***map, t_door *doors, float x, float y, int floor)
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
			if (check_x < 0 || check_y < 0 || check_y >= tablen(map[floor])
				|| check_x >= (int)ft_strlen(map[floor][check_y]))
				return (false);
			if (map[floor][check_y][check_x] == '1')
				return (false);
			if (map[floor][check_y][check_x] == 'D')
			{
				door = get_door(doors, check_x, check_y, floor);
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
		x_fixes_cursor(game, 's');
	}
	else
	{
		game->menu->status = MULTI_PLAYER;
		x_fixes_cursor(game, 'h');
	}
	game->chatbox->visible = !game->chatbox->visible;
}
