/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head_up_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 09:25:20 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/23 11:39:18 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	head_up_display(t_game *game)
{
		mini_map(game);
		crosshair(game);
		gun_draw(game);
		health_point_draw(game);
		ammo_written(game);
		damages_red_draw(game);

}
