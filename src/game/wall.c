/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:13:26 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/15 09:01:37 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_vertical_line_color(t_game *game, int x, int draw_start, int draw_end, int color)
{
	if (x < 0 || x >= game->screen_width || draw_start >= game->screen_height || draw_end < 0)
		return;
	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;
	for (int y = draw_start; y <= draw_end; y++)
		secure_pixel_put(game, x, y, color);
}

void	draw_vertical_line_with_texture(t_game *game, int x, int draw_start, int draw_end, t_image *texture, float wall_x, int line_height)
{
	if (x < 0 || x >= game->screen_width || draw_start >= game->screen_height || draw_end < 0)
		return;
		
	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;

	int bpp = texture->bpp >> 3;
	char *texture_data = texture->data;

	float step = (float)texture->height / line_height; 
	float tex_pos = (draw_start - (game->screen_height >> 1) + (line_height >> 1) + game->player->height * line_height) * step;
	int tex_x = (int)(wall_x * texture->width) % texture->width;
	for (int y = draw_start; y <= draw_end; y++)
	{
		int tex_y = (int)tex_pos % texture->height;
		tex_pos += step;
		int color = *((int *)(texture_data + tex_y * texture->size_line + tex_x * bpp));
		secure_pixel_put(game, x, y, color);
	}
}

bool	is_colored_wall(t_game *game, int side, int x, int draw_start, int draw_end)
{
	if (side == SIDE_EAST && game->textures->color_ea > 0)
		draw_vertical_line_color(game, x, draw_start, draw_end, game->textures->color_ea);
	else if (side == SIDE_WEST && game->textures->color_we > 0)
		draw_vertical_line_color(game, x, draw_start, draw_end, game->textures->color_we);
	else if (side == SIDE_NORTH && game->textures->color_no > 0)
		draw_vertical_line_color(game, x, draw_start, draw_end, game->textures->color_no);
	else if (side == SIDE_SOUTH && game->textures->color_so > 0)
		draw_vertical_line_color(game, x, draw_start, draw_end, game->textures->color_so);
	else 
		return (false);
	return (true);
}

void	draw_wall(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side)
{
	t_image *texture;
		
	float perp_wall_dist = (side == SIDE_EAST || side == SIDE_WEST)
		? (map_x - game->player->x + ((1 - step_x) >> 1)) / ray_dir_x
		: (map_y - game->player->y + ((1 - step_y) >> 1)) / ray_dir_y;

	int line_height = (int)(game->screen_height / perp_wall_dist);
	int draw_start = (game->screen_height >> 1) - (line_height >> 1) - (int)(game->player->height * line_height);
	int draw_end = draw_start + line_height - 1;

	game->wall_distances[x] = perp_wall_dist;
	if (is_colored_wall(game, side, x, draw_start, draw_end))
		return ;
	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;
	float wall_x = (side == SIDE_EAST || side == SIDE_WEST)
		? (game->player->y + perp_wall_dist * ray_dir_y)
		: (game->player->x + perp_wall_dist * ray_dir_x);
	wall_x -= (int)wall_x;
	switch (side)
	{
		case SIDE_EAST: texture = game->textures->east; break;
		case SIDE_WEST: texture = game->textures->west; break;
		case SIDE_NORTH: texture = game->textures->north; break;
		case SIDE_SOUTH: texture = game->textures->sud; break;
	}
	draw_vertical_line_with_texture(game, x, draw_start, draw_end, texture, wall_x, line_height);
}


