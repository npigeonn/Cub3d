/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 03:03:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/23 16:01:07 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	use_item(t_game *game)
{
	use_door_in_view(game);
	use_teleporter(game);
}

int	can_move(t_game *game, float x, float y, int floor)
{
	t_door	*door;

	if (game->map[game->player->floor][(int)(y)][(int)(x)] == '1')
		return (false);
	if (game->map[game->player->floor][(int)(y)][(int)(x)] == 'D')
	{
		door = get_door(game, (int)x, (int)y, game->player->floor);
		if (!door)
			return (true);
		if (!door->open)
			return (false);
	}
	return (true);
}

// int	can_move(t_game *game, float x, float y, int floor) //TODO:A modifier car segfault
// {
// 	t_door		*door;
// 	int			check_x;
// 	int			check_y;
// 	const float	buffer = 0.2;

// 	check_y = (int)(y - buffer) - 1;
// 	while (++check_y <= (int)(y + buffer))
// 	{
// 		check_x = (int)(x - buffer) - 1;
// 		while (++check_x <= (int)(x + buffer))
// 		{
// 			if (!game->map[floor])
// 				return (false);
// 			if (!game->map[floor][check_y])
// 				return (false);
// 			if (!game->map[floor][check_y][check_x])
// 				return (false);
// 			if (game->map[floor][check_y][check_x] == '1')
// 				return (false);
// 			if (game->map[floor][check_y][check_x] == 'D')
// 			{
// 				door = get_door(game, check_x, check_y, floor);
// 				if (!door->open)
// 					return (false);
// 			}
// 		}
// 	}
// 	return (true);  
// }

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
