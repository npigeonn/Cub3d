/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:16:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/12/10 12:56:41 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	use_door_in_view(t_game *game)
{
	const t_player	*player = game->player;
	t_door			*door;
	int				x;
	int				y;

	if (game->menu->message != CLOSE_DOOR && game->menu->message != OPEN_DOOR)
		return ;
	x = (int)(player->x + player->dir_x * 1);
	y = (int)(player->y + player->dir_y * 1);
	door = get_door(game->door, x, y, player->floor);
	if (door)
	{
		door->open = !door->open;
		send_door_msg(game, door);
	}
}

static float	calculate_perp_wall_dist(t_raycast *r, t_game *game)
{
	if (r->side == SIDE_EAST || r->side == SIDE_WEST)
		return ((r->map_x - game->player->x + (1 - r->step_x) * 0.5)
			/ r->ray_dir_x);
	return ((r->map_y - game->player->y + (1 - r->step_y) * 0.5)
		/ r->ray_dir_y);
}

static void	calculate_draw_limits(t_raycast *doorcast, t_game *game,
		float perp_wall_dist)
{
	doorcast->line_height = (int)(game->screen_height / perp_wall_dist);
	doorcast->draw_start = game->screen_height * 0.5
		- doorcast->line_height * 0.5
		- (int)(game->player->height * doorcast->line_height);
	doorcast->draw_end = doorcast->draw_start + doorcast->line_height - 1;
	if (doorcast->draw_start < 0)
		doorcast->draw_start = 0;
	if (doorcast->draw_end >= game->screen_height)
		doorcast->draw_end = game->screen_height - 1;
}

static float	calculate_wall_x(t_raycast *r, t_game *game, float perp_dist)
{
	float	wall_x;

	if (r->side == SIDE_EAST || r->side == SIDE_WEST)
		wall_x = game->player->y + perp_dist * r->ray_dir_y;
	else
		wall_x = game->player->x + perp_dist * r->ray_dir_x;
	return (wall_x - floor(wall_x));
}

void	draw_door(t_game *game, t_raycast *r)
{
	t_image		*texture;
	t_raycast	doorcast;
	float		perp_wall_dist;
	float		wall_x;

	texture = game->textures->door;
	perp_wall_dist = calculate_perp_wall_dist(r, game);
	calculate_draw_limits(&doorcast, game, perp_wall_dist);
	wall_x = calculate_wall_x(r, game, perp_wall_dist);
	game->wall_distances[r->x] = perp_wall_dist;
	doorcast.x = r->x;
	draw_vertical_line_with_texture(game, &doorcast, texture, wall_x);
}
