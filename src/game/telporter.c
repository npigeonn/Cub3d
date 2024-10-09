/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   telporter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:44:16 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/09 10:21:01 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	add_teleporter(t_game *game, int x, int y, int floor)
{
	t_teleporter	*tp;
	
	tp = malloc(sizeof(t_teleporter));
	tp->x = x;
	tp->y = y;
	tp->floor = floor;
	tp->x1 = 0;
	tp->y1 = 0;
	tp->floor1 = 0;
	tp->next = game->tp;
	game->tp = tp;
}

void	set_output_teleporter(t_game *game, int x, int y, int floor)
{
	game->tp->x1 = x;
	game->tp->y1 = y;
	game->tp->floor1 = floor;
}

static t_teleporter	*get_teleporter(t_game *game, int x, int y)
{
	t_teleporter	*current;

	current = game->tp;
	if (!game->tp)
		return (NULL);
	while (current)
	{
		if (current->x == x && current->y == y)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	use_teleporter(t_game *game, int x, int y)
{
	t_teleporter	*tp;

	if (game->message != TELEPORT)
		return ;
	tp = get_teleporter(game, x, y);
	if (tp)
	{
		game->player->x = tp->x;
		game->player->y = tp->y;
		game->player->floor = tp->floor;
	}	
}
