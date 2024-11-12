/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:13:26 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/12 11:39:17 by ybeaucou         ###   ########.fr       */
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

void	draw_wall(t_game *game, t_raycast *raycast)
{
	t_image		*texture;
		
	raycast->perp_wall_dist = (raycast->side == SIDE_EAST || raycast->side == SIDE_WEST)
		? (raycast->map_x - game->player->x + ((1 - raycast->step_x) >> 1)) / raycast->ray_dir_x
		: (raycast->map_y - game->player->y + ((1 -raycast-> step_y) >> 1)) / raycast->ray_dir_y;

	raycast->line_height = (int)(game->screen_height / raycast->perp_wall_dist);
	raycast->draw_start = (game->screen_height >> 1) - (raycast->line_height >> 1) - (int)(game->player->height * raycast->line_height);
	raycast->draw_end = raycast->draw_start + raycast->line_height - 1;

	game->wall_distances[raycast->x] = raycast->perp_wall_dist;
	if (is_colored_wall(game, raycast->side, raycast->x, raycast->draw_start, raycast->draw_end))
		return ;
	if (raycast->draw_start < 0) raycast->draw_start = 0;
	if (raycast->draw_end >= game->screen_height) raycast->draw_end = game->screen_height - 1;
	float wall_x = (raycast->side == SIDE_EAST || raycast->side == SIDE_WEST)
		? (game->player->y + raycast->perp_wall_dist * raycast->ray_dir_y)
		: (game->player->x + raycast->perp_wall_dist * raycast->ray_dir_x);
	wall_x -= (int)wall_x;
	switch (raycast->side)
	{
		case SIDE_EAST: texture = game->textures->east; break;
		case SIDE_WEST: texture = game->textures->west; break;
		case SIDE_NORTH: texture = game->textures->north; break;
		case SIDE_SOUTH: texture = game->textures->south; break;
	}
	draw_vertical_line_with_texture(game, raycast->x, raycast->draw_start, raycast->draw_end, texture, wall_x, raycast->line_height);
}


