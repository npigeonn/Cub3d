/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:17:48 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/08 10:18:24 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
	if (x > game->screen_width || x < 0)
		return;
	for (int y = start; y < end; y++)
	{
		if (y >= 0 && y < game->screen_height && *((int *)(game->images->base->data + y * game->images->base->size_line + x * (int)(game->images->base->bpp * 0.125))) == 0)
			pixel_put(game, x, y, color);
	}
}

void	draw_ceilling(t_game *game)
{
	int horizon = (game->screen_height * 0.5) + game->player->height;
	if (horizon < 0)
		horizon = 0;
	else if (horizon > game->screen_height)
		horizon = game->screen_height;
	for (int y = horizon; y > 0; y--)
	{
		int color = 0x896542;
		int shadow_factor = (y + horizon) * 255 / (game->screen_height + horizon);
		shadow_factor = fmin(1.0f, shadow_factor);
		int shadowed_color = ((int)((color & 0xFF0000) * shadow_factor) & 0xFF0000) |
						((int)((color & 0x00FF00) * shadow_factor) & 0x00FF00) |
						((int)((color & 0x0000FF) * shadow_factor) & 0x0000FF);
	   for (int x = 0; x < game->screen_width; x++)
		{
			if (y >= 0 && y < game->screen_height && *(int *)(game->images->base->data + y * game->images->base->size_line + x * (int)(game->images->base->bpp * 0.125)) == 0)
				pixel_put(game, x, y, shadowed_color);
		}
	}
}

void	draw_floor(t_game *game)
{
	int horizon = (game->screen_height * 0.5) + game->player->height;
	if (horizon < 0)
		horizon = 0;
	else if (horizon > game->screen_height)
		horizon = game->screen_height;
	for (int y = horizon; y < game->screen_height; y++)
	{
		int color = 0x08c41b;
		int shadow_factor = (y - horizon) * 255 / (game->screen_height - horizon);
		shadow_factor = fmin(1.0f, shadow_factor);
		int shadowed_color = ((int)((color & 0xFF0000) * shadow_factor) & 0xFF0000) |
						((int)((color & 0x00FF00) * shadow_factor) & 0x00FF00) |
						((int)((color & 0x0000FF) * shadow_factor) & 0x0000FF);
	   for (int x = 0; x < game->screen_width; x++)
		{
			if (y >= 0 && y < game->screen_height && *(int *)(game->images->base->data + y * game->images->base->size_line + x * (int)(game->images->base->bpp * 0.125)) == 0)
				pixel_put(game, x, y, shadowed_color);
		}
	}
}

void	draw_vertical_line_with_texture(t_game *game, int x, int draw_start, int draw_end, t_image *texture, float wall_x, int line_height)
{
	if (x < 0 || x >= game->screen_width)
		return;
	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;
	float step = (float)texture->height / line_height; 
	float tex_pos = (draw_start - game->screen_height / 2 + line_height / 2 + game->player->height * line_height) * step;

	for (int y = draw_start; y < draw_end; y++)
	{
		if (y < 0 || y >= game->screen_height)
			return;

		int tex_y = (int)tex_pos % texture->height;
		tex_pos += step;

		int texture_width = texture->width;
		int tex_x = (int)(wall_x) % texture_width;
		if (tex_y < 0 || tex_y >= texture->height || tex_x < 0 || tex_x >= texture_width)
			continue;
		int color = *((int *)(texture->data + tex_y * texture->size_line + tex_x * (texture->bpp / 8)));
		pixel_put(game, x, y, color);
	}
}

void	draw_wall(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side)
{
	t_image *texture;

	float perp_wall_dist = (side == SIDE_EAST) || (side == SIDE_WEST)
		? (map_x - game->player->x + (1 - step_x) * 0.5) / ray_dir_x
		: (map_y - game->player->y + (1 - step_y) * 0.5) / ray_dir_y;
		
	int line_height = (int)(game->screen_height / perp_wall_dist);
	int draw_start = -line_height * 0.5 + game->screen_height * 0.5;
	int draw_end = line_height * 0.5 + game->screen_height * 0.5;
	draw_start -= (int)(game->player->height * line_height);
	draw_end -= (int)(game->player->height * line_height);

	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;

	float wall_x;
	if (side == SIDE_EAST || side == SIDE_WEST) {
		wall_x = game->player->y + perp_wall_dist * ray_dir_y; 
	} else {
		wall_x = game->player->x + perp_wall_dist * ray_dir_x;
	}
	wall_x -= floor(wall_x);
	if (side == SIDE_EAST)
		texture = game->textures->zekrom;
	else if (side == SIDE_WEST)
		texture = game->textures->crefadet;
	else if (side == SIDE_NORTH)
		texture = game->textures->artikodin;
	else
		texture = game->textures->mewtwo;
	int texture_width = texture->width;
	wall_x *= texture_width;

	draw_vertical_line_with_texture(game, x, draw_start, draw_end, texture, wall_x, line_height);
}
