/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleporter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:44:16 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/09 10:45:03 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_teleporter(t_game *game, int x, int y, int floor)
{
	t_teleporter	*tp;
	
	tp = malloc(sizeof(t_teleporter));
	tp->x1 = x;
	tp->y1 = y;
	tp->floor1 = floor;
	tp->x1 = 0;
	tp->y1 = 0;
	tp->floor1 = 0;
	tp->next = game->tp;
	game->tp = tp;
}

void	set_output_teleporter(t_game *game, int x, int y, int floor)
{
	game->tp->x2 = x;
	game->tp->y2 = y;
	game->tp->floor2 = floor;
}

static t_teleporter	*get_teleporter(t_game *game, int x, int y)
{
	t_teleporter	*current;

	current = game->tp;
	if (!game->tp)
		return (NULL);
	while (current)
	{
		if (current->x1 == x && current->y1 == y)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	use_teleporter(t_game *game)
{
	t_teleporter	*tp;

	if (game->message != TELEPORT)
		return ;
	tp = get_teleporter(game, (int)game->player->x, (int)game->player->y);
	if (tp)
	{
		game->player->x = tp->x2;
		game->player->y = tp->y2;
		game->player->floor = tp->floor2;
	}	
}
