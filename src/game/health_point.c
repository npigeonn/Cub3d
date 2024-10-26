/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_point.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 09:30:57 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/26 00:15:57 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	life(t_game *game)
{
	int	x;
	int	y;
	
	y = game->screen_height * 0.855;
	game->time_regen += game->delta_time * 10;
	if ((int)game->time_regen > 18000000)
		game->time_regen = 100;
	if (game->time_regen > 100 && game->player->health < 0.9999)
		game->player->health += 0.00015;
	if (game->player->health > 1)
		game->player->health = 1;
	while (++y < game->screen_height * 0.88)
	{
		x = game->screen_width * 0.583;
		while (++x < game->screen_width * (.583 + .279 * game->player->health))
			pixel_put(game, x, y, 16711680);
	}

}


void	health_point_draw(t_game *game)
{
	int	x;
	int	y;
	float	alpha = 0.5; // Ajuster cette valeur entre 0 et 1 pour changer l'opacité

	y = game->screen_height * 0.85 - 1;
	while (++y <= game->screen_height * 0.885)
	{
		x = game->screen_width * 0.58 - 1;
		while (++x <= game->screen_width * 0.865)
		{
			pixel_put(game, x, y,
				blend_colors(get_pixel_color_from_image(game, x, y),
				0x000000, alpha));
			if ((game->screen_width * 0.862 <= x && x <= game->screen_width * 0.865)
				|| (game->screen_width * 0.58 <= x && x <= game->screen_width * 0.583)
				|| (game->screen_height * 0.85 <= y && y <= game->screen_height * 0.855)
				|| (game->screen_height * 0.88 <= y && y <= game->screen_height * 0.885))
				pixel_put(game, x, y, 0x000000);
		}
	}
	life(game);
}


void	draw_anim_health(t_game *game, int x, int y, t_image *im_health)
{
	static float	time_sprites = 0;

	time_sprites += game->delta_time * 10;
	if ((int)time_sprites > 18000000)
		time_sprites = 0;
	draw_sprite(game, im_health, x, y, 0, 0.2, 2, (int)time_sprites % 18);
}

void	on_life(t_game *game)
{
	t_sprite *current;

	current = game->sprites;
	if (game->map[game->player->floor][(int)game->player->y][(int)game->player->x] == 'H'
		|| game->map[game->player->floor][(int)game->player->y - 1][(int)game->player->x] == 'H'
		|| game->map[game->player->floor][(int)game->player->y + 1][(int)game->player->x] == 'H'
		|| game->map[game->player->floor][(int)game->player->y][(int)game->player->x + 1] == 'H'
		|| game->map[game->player->floor][(int)game->player->y][(int)game->player->x - 1] == 'H')
	{
		while (current)
		{
			if ((int)game->player->x <= current->x + 1 && (int)game->player->x >= current->x - 1
				&& (int)game->player->y <= current->y + 1 && (int)game->player->y >= current->y - 1
				&& current->type == SPRITE_HEALTH)
			{
				if (current->still_exist == 1 && game->player->health < 0.75)
					game->player->health += 0.25;
				else if (current->still_exist == 1)
					game->player->health = 1;
				current->still_exist = 0;
				return ;
			}
			current = current->next;
		}
	}
}
