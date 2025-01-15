/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:13:26 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/15 09:34:49 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

void	calculate_wall_distance(t_game *game, t_raycast *raycast)
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

float	calculate_wall_xx(t_game *game, t_raycast *raycast)
{
	float	wall_x;

	if (raycast->side == SIDE_EAST || raycast->side == SIDE_WEST)
		wall_x = game->player->y + raycast->perp_wall_dist * raycast->ray_dir_y;
	else
		wall_x = game->player->x + raycast->perp_wall_dist * raycast->ray_dir_x;
	return (wall_x - (int)wall_x);
}

t_image	*select_texture(t_game *game, t_raycast *raycast)
{
	if (raycast->side == SIDE_EAST)
		return (game->textures->east);
	if (raycast->side == SIDE_WEST)
		return (game->textures->west);
	if (raycast->side == SIDE_NORTH)
		return (game->textures->north);
	if (raycast->side == SIDE_SOUTH)
		return (game->textures->south);
	return (NULL);
}
