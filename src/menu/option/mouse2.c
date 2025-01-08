/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:07:22 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/02 19:07:36 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_mouse_check_box(t_game *game, int mouse_x, int mouse_y,
int info[5])
{
	const int	checkbox_size = 20;
	const int	checkbox_x = (info[2] + info[0] * 0.135) + info[0] * 0.5;
	const int	checkbox_y = info[4] - 5;

	if (mouse_x >= checkbox_x && mouse_x <= checkbox_x + checkbox_size
		&& mouse_y >= checkbox_y && mouse_y <= checkbox_y + checkbox_size)
		game->player->invert_mouse_x = !game->player->invert_mouse_x;
}

void	draw_mouse_slider(t_game *game, int mouse_x, int mouse_y, int info[5])
{
	const int	slider_x = (info[0] - info[0] * 0.3) * 0.5 + info[2];
	const int	slider_y = info[3] + info[1] * 0.4;
	const int	slider_width = info[0] * 0.3;
	const int	slider_height = game->screen_height * 0.025;

	if (mouse_x >= slider_x && mouse_x <= slider_x + slider_width
		&& mouse_y >= slider_y && mouse_y <= slider_y + slider_height)
	{
		game->menu->mouse_sensitivity = ((float)(mouse_x - slider_x)
				/ slider_width) * 100;
		if (game->menu->mouse_sensitivity < 0)
			game->menu->mouse_sensitivity = 0;
		if (game->menu->mouse_sensitivity > 100)
			game->menu->mouse_sensitivity = 100;
		game->menu->dragging = true;
	}
}

void	update_mouse_options_interaction(t_game *game, int mouse_x, int mouse_y)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	panel_x = (game->screen_width - panel_width) * 0.5;
	const float	panel_y = (game->screen_height - panel_height) * 0.5;
	int			info[5];

	info[0] = panel_width;
	info[1] = panel_height;
	info[2] = panel_x;
	info[3] = panel_y;
	info[4] = panel_y + panel_height * 0.372;
	draw_mouse_slider(game, mouse_x, mouse_y, info);
	info[4] += panel_height * 0.15;
	draw_mouse_check_box(game, mouse_x, mouse_y, info);
}
