/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:31:37 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/03 16:32:01 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_mouse_on_gear(t_game *game, int mouse_x, int mouse_y)
{
	const int	gear_size = game->screen_width * 0.035;
	const int	dx = mouse_x - (game->screen_width - gear_size / 2 - 17);
	const int	dy = mouse_y - (gear_size / 2 + 15);
	const int	distance_squared = dx * dx + dy * dy;

	if (distance_squared >= (gear_size / 2 - 5) * (gear_size / 2 - 5)
		&& distance_squared <= (gear_size / 2 + 5) * (gear_size / 2 + 5))
		game->menu->button_selected = 5;
	else if (distance_squared <= (gear_size / 2) * (gear_size / 2))
		game->menu->button_selected = 5;
}
