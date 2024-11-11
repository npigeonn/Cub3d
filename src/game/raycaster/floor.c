/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 01:01:10 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/11 18:01:14 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	get_texture_color(t_floorcast *f, int x, t_image *t)
{
	const float	f_x = f->floor_x + x * f->floor_step_x;
	const float	f_y = f->floor_y + x * f->floor_step_y;
	const int	t_x = (int)(f_x * f->f_tex_width) % f->f_tex_width;
	const int	t_y = (int)(f_y * f->f_tex_height) % f->f_tex_height;

	return (*((int *)(f->f_tex_data + t_y * t->size_line + t_x * f->f_bpp)));
}

static int	get_color(t_game *game, t_floorcast *floorcast, int x, int y)
{
	if (game->textures->color_f < 0 && y > floorcast->screen_mid)
		return (get_texture_color(floorcast, x, game->textures->floor));
	else if (y > floorcast->screen_mid)
		return (game->textures->color_f);
	if (game->textures->color_c < 0 && y < floorcast->screen_mid)
		return (get_texture_color(floorcast, x, game->textures->ceil));
	else if (y < floorcast->screen_mid)
		return (game->textures->color_c);
}

static void	init_calc_floorcast(t_game *game, t_floorcast *f)
{
	f->ray_dir_x0 = game->player->dir_x - game->player->planeX;
	f->ray_dir_x1 = game->player->dir_x + game->player->planeX;
	f->ray_dir_y0 = game->player->dir_y - game->player->planeY;
	f->ray_dir_y1 = game->player->dir_y + game->player->planeY;
	f->pos_z = game->screen_height * 0.5f;
	f->diff_ray_dir_x = f->ray_dir_x1 - f->ray_dir_x0;
	f->diff_ray_dir_y = f->ray_dir_y1 - f->ray_dir_y0;
	f->screen_mid = game->screen_height >> 1;
	f->screen_width_inv = 1.0f / game->screen_width;
	f->diff_screen_x = f->diff_ray_dir_x * f->screen_width_inv;
	f->diff_screen_y = f->diff_ray_dir_y * f->screen_width_inv;
}

static void	get_step_floorcast(t_game *game, t_floorcast *f, int y)
{
	f->p = y - f->screen_mid;
	if (f->p == 0)
		f->p = 1;
	f->row_distance = f->pos_z / fabsf(f->p);
	f->floor_step_x = f->row_distance * f->diff_screen_x;
	f->floor_step_y = f->row_distance * f->diff_screen_y;
	f->floor_x = game->player->x + f->row_distance * f->ray_dir_x0;
	f->floor_y = game->player->y + f->row_distance * f->ray_dir_y0;
}

void	cast_floor(t_game *game, t_floorcast *floorcast)
{
	int			x;

	init_calc_floorcast(game, floorcast);
	init_floorcast(game, floorcast);
	get_step_floorcast(game, floorcast, floorcast->y);
	x = -1;
	while (++x < game->screen_width)
		secure_pixel_put(game, x, floorcast->y, get_color(game, floorcast, x, floorcast->y));
}
