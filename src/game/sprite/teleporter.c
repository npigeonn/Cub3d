/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleporter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:44:16 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/22 13:21:00 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_teleporter(t_game *game, int x, int y, int floor)
{
	t_sprite	*tp;
	
	tp = malloc(sizeof(t_sprite));
	tp->x = x + 0.5;
	tp->y = y + 0.5;
	tp->floor = floor;
	tp->x1 = 0;
	tp->y1 = 0;
	tp->floor1 = 0;
	tp->next = game->sprites;
	tp->type = SPRITE_TELEPORTER;
	game->sprites = tp;
}

void	set_output_teleporter(t_game *game, int x, int y, int floor)
{
	game->sprites->x1 = x + 0.5;
	game->sprites->y1 = y + 0.5;
	game->sprites->floor1 = floor;
}

static t_sprite	*get_teleporter(t_game *game, int x, int y)
{
	t_sprite	*current;

	current = game->sprites;
	while (current)
	{
		if (current->type != SPRITE_TELEPORTER)
		{
			current = current->next;
			continue;
		}
		if ((int)current->x == x && (int)current->y == y)
			return (current);
		else if ((int)current->x1 == x && (int)current->y1 == y)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	use_teleporter(t_game *game)
{
	t_sprite	*tp;
	t_player	*p = game->player;

	p = game->player;
	if (game->menu->message != TELEPORT)
		return ;
	tp = get_teleporter(game, (int)p->x, (int)p->y);
	if (tp)
	{
		if ((int)p->x == (int)tp->x1 && (int)p->y == (int)tp->y1 && p->floor == tp->floor1)
		{
			p->x = tp->x;
			p->y = tp->y;
			p->floor = tp->floor;
		}
		else
		{
			p->x = tp->x1;
			p->y = tp->y1;
			p->floor = tp->floor1;
		}
	}	
}
