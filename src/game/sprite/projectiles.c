/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectiles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:33:47 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 11:33:47 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	projectile_move(t_game *game, t_projectile *p, bool *collision)
{
	float	distance;
	float	traveled;
	float	ray[2];

	distance = sqrt((cos(p->direction * (M_PI / 180.0f)) * p->speed)
			* (cos(p->direction * (M_PI / 180.0f)) * p->speed)
			+ (sin(p->direction * (M_PI / 180.0f)) * p->speed)
			* (sin(p->direction * (M_PI / 180.0f)) * p->speed));
	ray[0] = p->x;
	ray[1] = p->y;
	traveled = 0;
	while (traveled < distance && !*collision)
	{
		ray[0] += cos(p->direction * (M_PI / 180.0f)) * 0.1;
		ray[1] += sin(p->direction * (M_PI / 180.0f)) * 0.1;
		if (!can_move(game->map, game->door, ray, p->floor)
			|| check_collision_with_entity(game, p, ray))
			*collision = true;
		p->x = ray[0];
		p->y = ray[1];
		traveled += 0.1;
	}
}

void	projectile_collision(t_game *game, t_projectile **projectile,
t_projectile **prev, int collision)
{
	t_projectile	*temp;

	if (collision)
	{
		temp = *projectile;
		if (*prev)
			(*prev)->next = (*projectile)->next;
		else
			game->projectiles = (*projectile)->next;
		*projectile = (*projectile)->next;
		gc_free(game->mem, temp);
	}
	else
	{
		*prev = *projectile;
		*projectile = (*projectile)->next;
	}
}
