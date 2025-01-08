/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_move.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:27:52 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/03 16:40:51 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	check_mouvement(t_game *game, t_player *p, double new_x,
	double new_y)
{
	float	ray[2];

	ray[0] = new_x;
	ray[1] = new_y;
	if (!can_move(game->map, game->door, ray, game->player->floor))
		return ;
	p->stats->distanc_travel += sqrt(pow(new_x - p->x, 2)
			+ pow(new_y - p->y, 2));
	p->x = new_x;
	p->y = new_y;
}

static void	mouvement2(t_game *game, t_player *p, double *new_x, double *new_y)
{
	if (is_key_pressed(game, p->key->right)
		|| is_key_pressed(game, p->key->right2))
	{
		*new_x += p->plane_x * 0.1;
		*new_y += p->plane_y * 0.1;
	}
	if (is_key_pressed(game, p->key->left)
		|| is_key_pressed(game, p->key->left2))
	{
		*new_x -= p->plane_x * 0.1;
		*new_y -= p->plane_y * 0.1;
	}
}

void	mouvement(t_game *game, t_player *p)
{
	double	new_x;
	double	new_y;

	new_x = p->x;
	new_y = p->y;
	if (game->player->health <= 0)
		return ;
	if (is_key_pressed(game, p->key->up) || is_key_pressed(game, p->key->up2))
	{
		new_x += p->dir_x * 0.1;
		new_y += p->dir_y * 0.1;
	}
	if (is_key_pressed(game, p->key->down)
		|| is_key_pressed(game, p->key->down2))
	{
		new_x -= p->dir_x * 0.1;
		new_y -= p->dir_y * 0.1;
	}
	mouvement2(game, p, &new_x, &new_y);
	check_mouvement(game, p, new_x, new_y);
}
