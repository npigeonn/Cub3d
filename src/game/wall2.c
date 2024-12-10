/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:49:26 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 11:49:26 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

void	draw_wall(t_game *game, t_raycast *raycast)
{
	t_image	*texture;
	float	wall_x;

	calculate_wall_distance(game, raycast);
	if (is_colored_wall(game, raycast))
		return ;
	wall_x = calculate_wall_xx(game, raycast);
	texture = select_texture(game, raycast);
	draw_vertical_line_with_texture(game, raycast, texture, wall_x);
}
