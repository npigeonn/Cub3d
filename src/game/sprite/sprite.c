/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:51:45 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/12/10 12:17:19 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	get_info_sprite_band(t_game *game, t_sprite *sprite, int x)
{
	t_spritecast	*sp;

	sp = sprite->spritecast;
	sp->step = (float)sp->texture->sprite_height / sp->height;
	sp->tex_pos = (sp->draw_start_y - game->cen_y + sp->height * 0.5f
			+ game->player->height * sp->height) * sp->step;
	sp->bpp = sp->texture->bpp / 8;
	sp->texture_data = sp->texture->data;
	sp->anim_frame = sprite->selected_anim % sp->texture->sprite_height;
}

void	draw_vertical_sprite_band_loop(t_game *game, t_sprite *sprite,
int x, int y)
{
	int				tex_y;
	int				color;
	t_spritecast	*sp;

	sp = sprite->spritecast;
	tex_y = (int)(sp->tex_pos) % sp->texture->sprite_height;
	sp->tex_pos += sp->step;
	if (tex_y < 0)
		tex_y = 0;
	else if (tex_y >= sp->texture->sprite_height)
		tex_y = sp->texture->sprite_height - 1;
	color = *((int *)(sp->texture_data + (tex_y + sp->texture->sprite_height
					* sp->anim_frame) * sp->texture->size_line
				+ sp->tex_x * sp->bpp));
	if (sprite->type == SPRITE_PLAYER && color >= 0)
		color = blend_colors(color, 0x51789, 0.5);
	if (color >= 0)
		pixel_put(game, x, y, color);
}

void	draw_vertical_sprite_band(t_game *game, t_sprite *sprite, int x)
{
	t_spritecast	*sp;
	int				y;
	int				tex_y;
	int				color;

	sp = sprite->spritecast;
	if (x < 0 || x >= game->screen_width
		|| sp->draw_start_y >= game->screen_height || sp->draw_end_y < 0)
		return ;
	if (sprite->selected_anim < 0)
		sprite->selected_anim = 0;
	if (sp->draw_start_y < 0)
		sp->draw_start_y = 0;
	if (sp->draw_end_y >= game->screen_height)
		sp->draw_end_y = game->screen_height - 1;
	get_info_sprite_band(game, sprite, x);
	y = sp->draw_start_y - 1;
	while (++y < sp->draw_end_y)
		draw_vertical_sprite_band_loop(game, sprite, x, y);
}

float	calculate_distance(t_game *game, t_sprite *sprite)
{
	float	sprite_x;
	float	sprite_y;

	if (sprite->type == SPRITE_TELEPORTER && sprite->floor1 == game->player->y)
	{
		sprite_x = sprite->x1;
		sprite_y = sprite->y1;
	}
	else
	{
		sprite_x = sprite->x;
		sprite_y = sprite->y;
	}
	return (sqrtf((sprite_x - game->player->x) * (sprite_x - game->player->x)
			+ (sprite_y - game->player->y) * (sprite_y - game->player->y)));
}
