/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:12:31 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 12:12:31 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	init_spritecast(t_game *game, t_sprite *sprite, t_image *texture)
{
	t_spritecast	*sp;
	t_player		*p;

	sp = sprite->spritecast;
	pthread_mutex_lock(&game->mutex);
	p = game->player;
	sp->r_x = sprite->x - p->x;
	sp->r_y = sprite->y - p->y;
	pthread_mutex_unlock(&game->mutex);
	sp->inv_det = 1.0f / (p->plane_x * p->dir_y - p->dir_x * p->plane_y);
	sp->transform_x = sp->inv_det * (p->dir_y * sp->r_x - p->dir_x * sp->r_y);
	sp->transform_y = sp->inv_det
		* (-p->plane_y * sp->r_x + p->plane_x * sp->r_y);
	sp->player_angle = atan2(p->dir_y, p->dir_x);
	sp->relative_angle = sprite->dir - sp->player_angle;
	if (sp->relative_angle > M_PI)
		sp->relative_angle -= 2 * M_PI;
	if (sp->relative_angle < -M_PI)
		sp->relative_angle += 2 * M_PI;
	sp->texture = texture;
}

int	get_spritecast_info(t_game *game, t_sprite *sp, int *sprite_index)
{
	t_spritecast	*spc;

	spc = sp->spritecast;
	if (*sprite_index >= spc->texture->nb_sprite || *sprite_index < 0)
		*sprite_index = 0;
	spc->screen_x = (int)(game->cen_x
			* (1 + spc->transform_x / spc->transform_y));
	spc->height = abs((int)(game->screen_height / spc->transform_y))
		* sp->scale;
	spc->width = abs((int)(game->screen_height / spc->transform_y))
		* sp->scale;
	spc->draw_start_y = (-spc->height / 2) + game->cen_y
		- (int)(game->player->height * spc->height)
		+ (int)(sp->z_offset * spc->height);
	spc->draw_end_y = (spc->height / 2) + game->cen_y
		- (int)(game->player->height * spc->height)
		+ (int)(sp->z_offset * spc->height);
	if (spc->width <= 0 || spc->draw_end_y <= 0
		|| spc->draw_start_y >= game->screen_height)
		return (0);
	spc->stripe_start = spc->screen_x - (spc->width / 2);
	spc->stripe_end = spc->screen_x + (spc->width / 2);
	return (1);
}

void	get_spritecast_tex_x(t_game *game, t_sprite *sprite, int stripe,
int sprite_index)
{
	t_spritecast	*sp;

	sp = sprite->spritecast;
	sp->tex_x_base = (int)((stripe - sp->stripe_start)
			* sp->texture->sprite_width / sp->width);
	sp->tex_x = sp->tex_x_base + sprite_index * sp->texture->sprite_width;
}

void	draw_player_pseudo(t_game *game, t_sprite *sprite)
{
	t_spritecast	*sp;
	float			distance;
	t_draw_info		info;
	int				height;

	sp = sprite->spritecast;
	if (sprite->type == SPRITE_PLAYER)
	{
		distance = sqrtf(sp->r_x * sp->r_x + sp->r_y * sp->r_y);
		height = game->screen_height * 0.1 / distance;
		info = init_draw_info(height, sprite->pseudo,
				sp->screen_x, sp->draw_start_y + 20);
		info.color = 0xFFFFF;
		draw_text(game, info);
	}
}

void	draw_sprite(t_game *game, t_image *texture, t_sprite *sprite)
{
	const int		sprite_order[8] = {5, 6, 7, 3, 2, 1, 0, 4};
	t_spritecast	*sp;
	int				sprite_index;
	int				stripe;

	sp = sprite->spritecast;
	init_spritecast(game, sprite, texture);
	sprite_index = (int)((sp->relative_angle + M_PI) / (2 * M_PI)
			* texture->nb_sprite) % texture->nb_sprite;
	sprite_index = sprite_order[sprite_index];
	if (sp->transform_y <= 0 || !get_spritecast_info(game, sprite,
			&sprite_index))
		return ;
	stripe = sp->stripe_start - 1;
	while (++stripe < sp->stripe_end)
	{
		if (stripe >= 0 && stripe < game->screen_width
			&& sp->transform_y < game->wall_distances[stripe])
		{
			get_spritecast_tex_x(game, sprite, stripe, sprite_index);
			if (sp->tex_x_base >= 0 && sp->tex_x < texture->width)
				draw_vertical_sprite_band(game, sprite, stripe);
		}
	}
	draw_player_pseudo(game, sprite);
}
