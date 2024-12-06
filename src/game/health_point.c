/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_point.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 09:30:57 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/27 16:58:50 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	life(t_game *game)
{
	int	x;
	int	y;

	y = game->screen_height * 0.855;
	if (game->menu->status != MULTI_PLAYER && game->menu->status != CHATING)
	{
		game->time_regen += game->delta_time * 10;
		if ((int)game->time_regen > 18000000)
			game->time_regen = 150;
		if (game->time_regen > 150 && game->player->health < 0.9999)
			game->player->health += 0.00015;
		if (game->player->health > 1)
			game->player->health = 1;
	}
	while (++y < game->screen_height * 0.88)
	{
		x = game->screen_width * 0.583;
		while (++x < game->screen_width * (.583 + .279 * game->player->health))
			pixel_put(game, x, y, 16711680);
	}
}

void	health_point_draw(t_game *game)
{
	int			x;
	int			y;
	const float	alpha = 0.5;

	y = game->screen_height * 0.85 - 1;
	while (++y <= game->screen_height * 0.885)
	{
		x = game->screen_width * 0.58 - 1;
		while (++x <= game->screen_width * 0.865)
		{
			pixel_put(game, x, y,
				blend_colors(get_pixel_color_from_image(game, x, y),
					0x000000, alpha));
			if ((game->screen_width * 0.862 <= x
					&& x <= game->screen_width * 0.865)
				|| (game->screen_width * 0.58 <= x
					&& x <= game->screen_width * 0.583)
				|| (game->screen_height * 0.85 <= y
					&& y <= game->screen_height * 0.855)
				|| (game->screen_height * 0.88 <= y
					&& y <= game->screen_height * 0.885))
				pixel_put(game, x, y, 0x000000);
		}
	}
	life(game);
}

void	draw_anim_health(t_game *game, t_sprite *sprite, t_image *im_health)
{
	static float	time_sprites = 0;

	time_sprites += game->delta_time * 10;
	if ((int)time_sprites > 18000000)
		time_sprites = 0;
	sprite->selected_anim = (int)time_sprites % 18;
	draw_sprite(game, im_health, sprite);
}

void	on_life2(t_game *game, t_sprite *current)
{
	t_player	*p;

	p = game->player;
	pthread_mutex_lock(&game->mutex_music);
	p->life_up = 1;
	pthread_mutex_unlock(&game->mutex_music);
	current->still_exist = 0;
	if (p->health < 0.75)
		p->health += 0.25;
	else
		p->health = 1;
	return ;
}

void	on_life(t_game *game)
{
	t_sprite	*current;
	t_player	*p;

	p = game->player;
	current = game->sprites;
	if (game->map[p->floor][(int)p->y][(int)p->x] == 'H'
		|| game->map[p->floor][(int)p->y - 1][(int)p->x] == 'H'
		|| game->map[p->floor][(int)p->y + 1][(int)p->x] == 'H'
		|| game->map[p->floor][(int)p->y][(int)p->x + 1] == 'H'
		|| game->map[p->floor][(int)p->y][(int)p->x - 1] == 'H')
	{
		while (current)
		{
			if ((int)p->x <= current->x + 1 && (int)p->x >= current->x - 1
				&& (int)p->y <= current->y + 1 && (int)p->y >= current->y - 1
				&& current->type == SPRITE_HEALTH && current->still_exist)
				on_life2(game, current);
			current = current->next;
		}
	}
}
