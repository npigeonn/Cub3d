/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:19:22 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/06 08:49:21 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	get_key_index(t_game *game, int keycode)
{
	const int keycode_map[17] =
	{
		game->player->key->up,
		game->player->key->up2,
		game->player->key->down,
		game->player->key->down2,
		game->player->key->left,
		game->player->key->left2,
		game->player->key->right,
		game->player->key->right2,
		game->player->key->use,
		game->player->key->use2,
		game->player->key->escape,
		game->player->key->escape2,
		game->player->key->pause,
		game->player->key->pause2,
		game->player->key->chat,
		game->player->key->chat2
	};
	int i;
	
	i = -1;
	while (++i < 16)
	{
		if (keycode_map[i] == keycode)
			return (i);
	}
	return (-1);
}

void	set_key_flag(t_game *game, int keycode, int is_pressed)
{
	const int	index = get_key_index(game, keycode);

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
	const int index = get_key_index(game, keycode);

	if (index < 0)
		return (0);
	return ((game->key_flags & (1U << index)) != 0);
}

bool	is_keyflag(t_game *game, int keycode)
{
	return (keycode == game->player->key->up || keycode == game->player->key->up2
	|| keycode == game->player->key->down || keycode == game->player->key->down2
	|| keycode == game->player->key->left || keycode == game->player->key->left2
	|| keycode == game->player->key->right || keycode == game->player->key->right2
	|| keycode == game->player->key->use || keycode == game->player->key->use2
	|| keycode == game->player->key->escape || keycode == game->player->key->escape2
	|| keycode == game->player->key->pause || keycode == game->player->key->pause2
	|| keycode == game->player->key->chat || keycode == game->player->key->chat2);
}