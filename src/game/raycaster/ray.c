/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 00:57:29 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/19 02:49:20 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	get_step(t_player *p, t_raycast *r)
{
	if (r->ray_dir_x < 0)
	{
		r->step_x = -1;
		r->side_dist_x = (p->x - r->map_x) * r->delta_dist_x;
	}
	else
	{
		r->step_x = 1;
		r->side_dist_x = (r->map_x + 1.0 - p->x) * r->delta_dist_x;
	}
	if (r->ray_dir_y < 0)
	{
		r->step_y = -1;
		r->side_dist_y = (p->y - r->map_y) * r->delta_dist_y;
	}
	else
	{
		r->step_y = 1;
		r->side_dist_y = (r->map_y + 1.0 - p->y) * r->delta_dist_y;
	}
}

static void	calc_base_raycast(t_game *game)
{
	t_raycast	*raycast;
	t_player	*player;

	player = game->player;
	raycast = player->raycast;
	raycast->map_x = (int)player->x;
	raycast->map_y = (int)player->y;
	raycast->camera_x = 2 * raycast->x / (float)game->screen_width - 1;
	raycast->ray_dir_x = player->dirX + player->planeX * raycast->camera_x;
	raycast->ray_dir_y = player->dirY + player->planeY * raycast->camera_x;
	if (raycast->ray_dir_x == 0)
		raycast->delta_dist_x = 1e30;
	else
		raycast->delta_dist_x = fabs(1 / raycast->ray_dir_x);
	if (raycast->ray_dir_y == 0)
		raycast->delta_dist_y = 1e30;
	else
		raycast->delta_dist_y = fabs(1 / raycast->ray_dir_y);
	get_step(player, raycast);
}

static void	get_side(t_raycast *raycast)
{
	if (raycast->side_dist_x < raycast->side_dist_y)
	{
		raycast->side_dist_x += raycast->delta_dist_x;
		raycast->map_x += raycast->step_x;
		if (raycast->step_x > 0)
			raycast->side = SIDE_EAST;
		else
			raycast->side = SIDE_WEST;
	}
	else
	{
		raycast->side_dist_y += raycast->delta_dist_y;
		raycast->map_y += raycast->step_y;
		if (raycast->step_y > 0)
			raycast->side = SIDE_SOUTH;
		else
			raycast->side = SIDE_NORTH;
	}
}

void	cast_rays(t_game *game)
{
	t_raycast	*r;

	r = game->player->raycast;
	r->x = -1;
	while (++r->x < game->screen_width)
	{
		calc_base_raycast(game);
		while (1)
		{
			get_side(r);
			if (game->map[game->player->floor][r->map_y][r->map_x] == '1')
			{
				draw_wall(game);
				break ;
			}
			if (handle_door(game))
				break ;
		}
	}
}
