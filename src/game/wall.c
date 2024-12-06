/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:13:26 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/27 15:46:24 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_vertical_line_color(t_game *game, t_raycast *raycast, int color)
{
	int	draw_start;
	int	draw_end;
	int	x;
	int	y;

	x = raycast->x;
	draw_end = raycast->draw_end;
	draw_start = raycast->draw_start;
	if (x < 0 || x >= game->screen_width || draw_start >= game->screen_height
		|| draw_end < 0)
		return ;
	if (draw_start < 0)
		draw_start = 0;
	if (draw_end >= game->screen_height)
		draw_end = game->screen_height - 1;
	y = draw_start - 1;
	while (++y <= draw_end)
		secure_pixel_put(game, x, y, color);
}

static int	get_texture_x(t_image *texture, float wall_x)
{
	int		tex_x;

	tex_x = (int)(wall_x * texture->width) % texture->width;
	if (tex_x < 0)
		tex_x += texture->width;
	return (tex_x);
}

static void	draw_texture_line(t_game *game, t_raycast *r, t_image *texture,
float step)
{
	int		bpp;
	float	tex_pos;
	int		y;
	int		tex_x;
	int		color;

	bpp = texture->bpp >> 3;
	tex_x = get_texture_x(texture, r->wall_x);
	tex_pos = (r->draw_start - (game->screen_height >> 1)
			+ (r->line_height >> 1) + game->player->height
			* r->line_height) * step;
	y = r->draw_start;
	while (y <= r->draw_end)
	{
		color = *((int *)(texture->data
					+ ((int)tex_pos % texture->height) * texture->size_line
					+ tex_x * bpp));
		secure_pixel_put(game, r->x, y, color);
		tex_pos += step;
		y++;
	}
}

void	draw_vertical_line_with_texture(t_game *game, t_raycast *r,
t_image *texture, float wall_x)
{
	float	step;

	if (r->x < 0 || r->x >= game->screen_width
		|| r->draw_start >= game->screen_height || r->draw_end < 0)
		return ;
	if (r->draw_start < 0)
		r->draw_start = 0;
	if (r->draw_end >= game->screen_height)
		r->draw_end = game->screen_height - 1;
	step = (float)texture->height / r->line_height;
	r->wall_x = wall_x;
	draw_texture_line(game, r, texture, step);
}

bool	is_colored_wall(t_game *game, t_raycast *raycast)
{
	int	side;

	side = raycast->side;
	if (side == SIDE_EAST && game->textures->color_ea > 0)
		draw_vertical_line_color(game, raycast, game->textures->color_ea);
	else if (side == SIDE_WEST && game->textures->color_we > 0)
		draw_vertical_line_color(game, raycast, game->textures->color_we);
	else if (side == SIDE_NORTH && game->textures->color_no > 0)
		draw_vertical_line_color(game, raycast, game->textures->color_no);
	else if (side == SIDE_SOUTH && game->textures->color_so > 0)
		draw_vertical_line_color(game, raycast, game->textures->color_so);
	else
		return (false);
	return (true);
}

static void	calculate_wall_distance(t_game *game, t_raycast *raycast)
{
	if (raycast->side == SIDE_EAST || raycast->side == SIDE_WEST)
		raycast->perp_wall_dist = (raycast->map_x - game->player->x
				+ ((1 - raycast->step_x) >> 1)) / raycast->ray_dir_x;
	else
		raycast->perp_wall_dist = (raycast->map_y - game->player->y
				+ ((1 - raycast->step_y) >> 1)) / raycast->ray_dir_y;
	raycast->line_height = (int)(game->screen_height / raycast->perp_wall_dist);
	raycast->draw_start = (game->screen_height >> 1)
		- (raycast->line_height >> 1) - (int)(game->player->height
			* raycast->line_height);
	raycast->draw_end = raycast->draw_start + raycast->line_height - 1;
	if (raycast->draw_start < 0)
		raycast->draw_start = 0;
	if (raycast->draw_end >= game->screen_height)
		raycast->draw_end = game->screen_height - 1;
	game->wall_distances[raycast->x] = raycast->perp_wall_dist;
}

static float	calculate_wall_x(t_game *game, t_raycast *raycast)
{
	float	wall_x;

	if (raycast->side == SIDE_EAST || raycast->side == SIDE_WEST)
		wall_x = game->player->y + raycast->perp_wall_dist * raycast->ray_dir_y;
	else
		wall_x = game->player->x + raycast->perp_wall_dist * raycast->ray_dir_x;
	return (wall_x - (int)wall_x);
}

static t_image	*select_texture(t_game *game, t_raycast *raycast)
{
	if (raycast->side == SIDE_EAST)
		return (game->textures->east);
	if (raycast->side == SIDE_WEST)
		return (game->textures->west);
	if (raycast->side == SIDE_NORTH)
		return (game->textures->north);
	return (game->textures->south);
}

void	draw_wall(t_game *game, t_raycast *raycast)
{
	t_image	*texture;
	float	wall_x;

	calculate_wall_distance(game, raycast);
	if (is_colored_wall(game, raycast))
		return ;
	wall_x = calculate_wall_x(game, raycast);
	texture = select_texture(game, raycast);
	draw_vertical_line_with_texture(game, raycast, texture, wall_x);
}
