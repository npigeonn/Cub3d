/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:13:37 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/09 14:18:28 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void draw_vertical_sprite_line(t_game *game, int x, int draw_start, int draw_end, t_image *texture, int tex_x, int sprite_height)
{
	if (x < 0 || x >= game->screen_width)
		return;
	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;

	float step = (float)texture->height / sprite_height;
	float tex_pos = (draw_start - game->screen_height / 2 + sprite_height / 2) * step;

	for (int y = draw_start; y < draw_end; y++)
	{
		int tex_y = (int)tex_pos % texture->height;
		tex_pos += step;

		if (tex_y < 0 || tex_y >= texture->height)
			continue;
		int color = *((int *)(texture->data + tex_y * texture->size_line + tex_x * (texture->bpp / 8)));
		if (color > 0)
			pixel_put(game, x, y, color);
	}
}


void	draw_sprite(t_game *game, t_image *texture, float x, float y)
{
	float sprite_x = x - game->player->x;
	float sprite_y = y - game->player->y;

	float inv_det = 1.0 / (game->player->planeX * game->player->dirY - game->player->dirX * game->player->planeY);
	float transform_x = inv_det * (game->player->dirY * sprite_x - game->player->dirX * sprite_y);
	float transform_y = inv_det * (-game->player->planeY * sprite_x + game->player->planeX * sprite_y);

	int sprite_screen_x = (int)((game->screen_width * 0.5) * (1 + transform_x / transform_y));

	int sprite_height = abs((int)(game->screen_height / transform_y));
	int draw_start_y = -sprite_height * 0.5 + game->screen_height * 0.5;
	int draw_end_y = sprite_height * 0.5 + game->screen_height * 0.5;
	draw_start_y -= (int)(game->player->height * sprite_height);
	draw_end_y -= (int)(game->player->height * sprite_height);


	int sprite_width = abs((int)(game->screen_height / transform_y));

	for (int stripe = -sprite_width * 0.5 + sprite_screen_x; stripe < sprite_width * 0.5 + sprite_screen_x; stripe++)
	{
		if (transform_y > 0 && stripe >= 0 && stripe < game->screen_width)
		{
			int tex_x = (int)((stripe - (-sprite_width * 0.5 + sprite_screen_x)) * texture->width / sprite_width);
			if (tex_x >= 0 && tex_x < texture->width)
				draw_vertical_sprite_line(game, stripe, draw_start_y, draw_end_y, texture, tex_x, sprite_height);
		}
	}
}

int	is_in_fov(t_game *game, float x, float y)
{
	float sprite_x = x - game->player->x;
	float sprite_y = y - game->player->y;
	float sprite_distance = sqrt(sprite_x * sprite_x + sprite_y * sprite_y);

	sprite_x /= sprite_distance;
	sprite_y /= sprite_distance;

	float dir_x = game->player->dirX;
	float dir_y = game->player->dirY;

	float player_distance = sqrt(dir_x * dir_x + dir_y * dir_y);
	dir_x /= player_distance;
	dir_y /= player_distance;

	float dot_product = dir_x * sprite_x + dir_y * sprite_y;
	float angle = acos(dot_product);

	float fov = 30 * (M_PI / 180);
	return (angle < fov);
}

void	draw_teleporter(t_game *game)
{
	t_teleporter	*current;

	current = game->tp;
	while(current)
	{
		if (current->floor1 == game->player->floor)
			if (is_in_fov(game, current->x1, current->y1))
			{
				float sprite_distance = sqrt((current->x1 - game->player->x) * (current->x1 - game->player->x) +
										(current->y1 - game->player->y) * (current->y1 - game->player->y));
				for (int x = 0; x < game->screen_width; x++)
				{
					if (game->wall_distances[x] > sprite_distance)
					{
						draw_sprite(game, game->textures->tp, current->x1, current->y1);
						break;
					}
				}
			}
		if (current->floor2 == game->player->floor)
			if (is_in_fov(game, current->x2, current->y2))
			{
				float sprite_distance = sqrt((current->x2 - game->player->x) * (current->x2 - game->player->x) +
										(current->y2 - game->player->y) * (current->y2 - game->player->y));
				for (int x = 0; x < game->screen_width; x++)
				{
					if (game->wall_distances[x] > sprite_distance)
					{
						draw_sprite(game, game->textures->tp, current->x2, current->y2);
						break;
					}
				}
			}
		current = current->next;
	}
}

void	draw_sprites(t_game *game)
{
	draw_teleporter(game);
}
