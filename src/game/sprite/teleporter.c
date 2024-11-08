/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleporter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:44:16 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/08 10:11:36 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_teleporter(t_game *game, int x, int y, int floor)
{
	t_sprite	*tp;

	tp = gc_malloc(game->mem, sizeof(t_sprite));
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
			continue ;
		}
		if ((int)current->x == x && (int)current->y == y)
			return (current);
		else if ((int)current->x1 == x && (int)current->y1 == y)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	animation_teleportation(t_game *game)
{
	int			x;
	int			y;
	int			color;
	int			new_color;
	const float	alpha = game->fade_progress;

	y = 0;
	while (y < game->screen_height)
	{
		x = 0;
		while (x < game->screen_width)
		{
			color = get_pixel_color_from_image(game, x, y);
			new_color = blend_colors(color, 13107400, alpha);
			pixel_put(game, x, y, new_color);
			x++;
		}
		y++;
	}
	game->fade_progress += 0.01;
	if (game->fade_progress > 1.2)
		use_teleporter(game);
}

void	use_teleporter(t_game *game)
{
	t_sprite	*tp;
	t_player	*p;

	p = game->player;
	tp = get_teleporter(game, game->player->x_tel, game->player->y_tel);
	if (tp)
	{
		if (game->player->x_tel == (int)tp->x1
			&& game->player->y_tel == (int)tp->y1
			&& game->player->f_tel == tp->floor1)
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
	game->player->being_tpted = 0;
	game->fade_progress = 0;
}
