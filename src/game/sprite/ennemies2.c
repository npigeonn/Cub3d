/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ennemies2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:19:39 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 11:19:39 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

bool	is_within_fov(float angle_diff, float fov)
{
	return (angle_diff <= fov * 0.5f);
}

bool	check_line_of_sight(t_game *game, t_sprite *sprite,
float dx, float dy)
{
	const float	distance = sqrt(dx * dx + dy * dy);
	const float	step_x = dx / distance;
	const float	step_y = dy / distance;
	float		t;
	float		ray[2];

	t = 0;
	while (t < distance)
	{
		sprite->x += step_x * 0.1f;
		sprite->y += step_y * 0.1f;
		ray[0] = sprite->x;
		ray[1] = sprite->y;
		if (!can_move(game->map, game->door, ray, sprite->floor))
			return (false);
		t += 0.1f;
	}
	return (true);
}

bool	has_line_of_sight(t_game *game, t_sprite *sprite)
{
	const float	dx = game->player->x - sprite->x;
	const float	dy = game->player->y - sprite->y;
	const float	angle_to_player = atan2(dy, dx) * (180.0f / M_PI);
	const float	angle_diff = fabsf(angle_to_player - sprite->dir);
	t_sprite	*copy;

	copy = gc_malloc(game->mem, sizeof(t_sprite));
	ft_memcpy(copy, sprite, sizeof(t_sprite));
	if (is_within_fov(angle_diff, sprite->fov))
	{
		if (check_line_of_sight(game, copy, dx, dy))
		{
			gc_free(game->mem, copy);
			return (true);
		}
		return (false);
	}
	gc_free(game->mem, copy);
	return (false);
}

void	damages_red_draw(t_game *game, int y)
{
	int		x;
	float	alpha;
	float	max_dist_x;
	float	max_dist_y;
	float	dist_y;

	max_dist_x = game->cen_x * 0.5;
	max_dist_y = game->cen_y * 0.5;
	alpha = 0.2 * (1 - game->player->health);
	dist_y = fabsf(y - game->cen_y) / max_dist_y;
	x = -1;
	while (++x <= game->screen_width)
		pixel_put(game, x, y, blend_colors(get_pixel_color_from_image(game,
					x, y), 9830400, alpha * (1 + fabsf(x - game->cen_x)
					/ max_dist_x + dist_y)));
}
