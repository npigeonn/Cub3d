/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleporter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:44:16 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/09 11:10:59 by ybeaucou         ###   ########.fr       */
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
	tp->x2 = 0;
	tp->y2 = 0;
	tp->floor2 = 0;
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
		else if (current->x2 == x && current->y2 == y)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	use_teleporter(t_game *game)
{
	t_teleporter	*tp;
	t_player		*p = game->player;

	p = game->player;
	if (game->message != TELEPORT)
		return ;
	tp = get_teleporter(game, (int)p->x, (int)p->y);
	if (tp)
	{
		if (p->x == tp->x1 && p->y == tp->y1 && p->floor == tp->floor1)
		{
			p->x = tp->x2;
			p->y = tp->y2;
			p->floor = tp->floor2;
		}
		else
		{
			p->x = tp->x2;
			p->y = tp->y2;
			p->floor = tp->floor2;
		}
	}	
}
