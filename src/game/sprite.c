/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 12:09:23 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/14 14:53:04 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void draw_vertical_sprite_line(t_game *game, int x, int draw_start, int draw_end, t_image *texture, int tex_x, int sprite_height)
{
	if (x < 0 || x >= game->screen_width || draw_start >= game->screen_height || draw_end < 0)
		return;
	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;

	float step = (float)texture->height / sprite_height;
	float tex_pos = (draw_start - game->screen_height * 0.5f + sprite_height * 0.5f + game->player->height * sprite_height) * step;

	int bpp_div_8 = texture->bpp / 8;
	char *texture_data = texture->data;

	for (int y = draw_start; y < draw_end; y++)
	{
		int tex_y = (int)tex_pos % texture->height;
		tex_pos += step;
		if (tex_y >= 0 && tex_y < texture->height)
		{
			int color = *((int *)(texture_data + tex_y * texture->size_line + tex_x * bpp_div_8));
			if (color > 0)
				pixel_put(game, x, y, color);
		}
	}
}


void	draw_sprite(t_game *game, t_image *texture, float x, float y)
{
	float	sprite_x = x - game->player->x;
	float	sprite_y = y - game->player->y;

	float	inv_det = 1.0f / (game->player->planeX * game->player->dirY - game->player->dirX * game->player->planeY);
	float	transform_x = inv_det * (game->player->dirY * sprite_x - game->player->dirX * sprite_y);
	float	transform_y = inv_det * (-game->player->planeY * sprite_x + game->player->planeX * sprite_y);

	if (transform_y <= 0) return;

	int	sprite_screen_x = (int)((game->screen_width * 0.5) * (1 + transform_x / transform_y));
	int	sprite_height = abs((int)(game->screen_height / transform_y));
	int	draw_start_y = (-sprite_height * 0.5) + (game->screen_height * 0.5) - (int)(game->player->height * sprite_height);
	int	draw_end_y = (sprite_height * 0.5) + (game->screen_height * 0.5) - (int)(game->player->height * sprite_height);
	int	sprite_width = abs((int)(game->screen_height / transform_y));

	if (sprite_width <= 0 || draw_end_y <= 0 || draw_start_y >= game->screen_height) return;

	int	stripe_start = sprite_screen_x - (sprite_width * 0.5);
	int	stripe_end = sprite_screen_x + (sprite_width * 0.5);

	for (int stripe = stripe_start; stripe < stripe_end; stripe++)
	{
		if (stripe >= 0 && stripe < game->screen_width)
		{
			if (transform_y < game->wall_distances[stripe])
			{
				int	tex_x_base = (int)((stripe - stripe_start) * texture->width / sprite_width);
				int	y_start = draw_start_y < 0 ? 0 : draw_start_y;
				int	y_end = draw_end_y >= game->screen_height ? game->screen_height - 1 : draw_end_y;

				if (tex_x_base >= 0 && tex_x_base < texture->width)
				{
					for (int y_pos = y_start; y_pos <= y_end; y_pos++)
						draw_vertical_sprite_line(game, stripe, y_pos, y_pos + 1, texture, tex_x_base, sprite_height);
					game->wall_distances[stripe] = transform_y;
				}
			}
		}
	}
}

void draw_teleporter(t_game *game)
{
	t_teleporter *current = game->tp;
	
	while (current)
	{
		if (current->floor1 == game->player->floor)
			draw_sprite(game, game->textures->tp, current->x1, current->y1);
		if (current->floor2 == game->player->floor)
			draw_sprite(game, game->textures->tp, current->x2, current->y2);
		current = current->next;
	}
}

void	draw_sprites(t_game *game)
{
	draw_teleporter(game);
	// draw_enemies(game);
}
