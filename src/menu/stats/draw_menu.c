/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:28:50 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:39:27 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_back_stats_button(t_game *game, int y, int stats_height)
{
	const int	button_x = (game->screen_width - game->screen_width * 0.25) / 2;
	const int	button_y = y + stats_height + 35;
	t_draw_info	info;

	if (game->menu->button_selected == 1)
	{
		info = init_draw_info(game->screen_height * 0.1 + 8, "",
				button_x - 4, button_y - 4);
		info.width = game->screen_width * 0.25 + 8;
		info.color = MENU_BUTTON_SELECTED_COLOR;
		info.radius = 10;
		draw_rounded_rectangle(game, info);
	}
	info = init_draw_info(game->screen_height * 0.1, "", button_x, button_y);
	info.width = game->screen_width * 0.25;
	info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, info);
	info = init_draw_info(game->screen_height * 0.1 * 0.5, "Back",
			(game->screen_width >> 1) - game->screen_width * 0.25 / 2 + 4
			+ game->screen_width * 0.25 / 2, y + stats_height + 35
			+ game->screen_height * 0.1 / 3 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_stats_menu_bg(t_game *game, int stats_width, int stats_height)
{
	const int	x = (game->screen_width - stats_width) * 0.5;
	const int	y = (game->screen_height - stats_height) * 0.25;
	t_draw_info	stats_background;

	stats_background = init_draw_info(0, "", x, y);
	stats_background.width = stats_width;
	stats_background.height = stats_height;
	stats_background.color = MENU_BUTTON_COLOR;
	stats_background.radius = 25;
	draw_rounded_rectangle(game, stats_background);
}

void	draw_stats_menu_header(t_game *game, int stats_width, int stats_height)
{
	t_draw_info	title_info;
	const int	x = (game->screen_width - stats_width) * 0.5;
	const int	y = (game->screen_height - stats_height) * 0.25;

	title_info = init_draw_info(30, "Player Statistics",
			x + stats_width * 0.5, y + 20);
	title_info.color = 0xFFFFFF;
	draw_text(game, title_info);
	title_info = init_draw_info(20, "Name", x + 20, y + 2 * 20 + 22);
	title_info.color = 0xFFFF00;
	draw_text_left(game, title_info);
	title_info = init_draw_info(20,
			"Games      Victories      Defeats      Kills      Play Time",
			x + stats_width - 20 - 20, y + 2 * 20 + 22);
	title_info.color = 0xFFFF00;
	draw_text_right(game, title_info);
}
