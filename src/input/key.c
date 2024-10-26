/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:19:22 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/26 00:30:15 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	get_key_index(int keycode)
{
	const int keycode_map[11] =
	{
		KEY_W,
		KEY_S,
		KEY_A,
		KEY_D,
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_SPACE,
		KEY_SHIFT,
		KEY_RIGHT_CLICK
	};
	int i;
	
	i = -1;
	while (++i < 10)
	{
		if (keycode_map[i] == keycode)
			return (i);
	}
	return (-1);
}

void	set_key_flag(t_game *game, int keycode, int is_pressed)
{
	const int	index = get_key_index(keycode);

	if (index >= 0)
	{
		if (is_pressed)
			game->key_flags |= (1U << index);
		else
			game->key_flags &= ~(1U << index);
	}
}

int	is_key_pressed(t_game *game, int keycode)
{
	const int index = get_key_index(keycode);

	if (index < 0)
		return (0);
	return ((game->key_flags & (1U << index)) != 0);
}

bool	is_keyflag(int keycode)
{
	return (keycode == KEY_UP || keycode == KEY_DOWN || keycode == KEY_LEFT
		|| keycode == KEY_RIGHT || keycode == KEY_W || keycode == KEY_A
		|| keycode == KEY_S || keycode == KEY_D || keycode == KEY_SPACE
		|| keycode == KEY_SHIFT);
}