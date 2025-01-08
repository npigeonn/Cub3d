/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:26:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:41:20 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_stats_menu_keycode(t_game *game, int keycode)
{
	if (keycode == 1)
	{
		if (game->menu->button_selected == 1)
			game->menu->status = MAIN_MENU;
		if (game->menu->button_selected == 5)
		{
			game->menu->status = OPTIONS_KEYBOARD;
			game->menu->last_status = STATS;
		}
		game->menu->button_selected = 0;
		game->menu->scroll = 0;
	}
	if (keycode == 5)
		game->menu->scroll += 1;
	if (keycode == 4)
	{
		game->menu->scroll -= 1;
		if (game->menu->scroll < 0)
			game->menu->scroll = 0;
	}
}

void	update_stats_menu_click(t_game *game, int x, int y, int keycode)
{
	const int	scroll_bar_x = (game->screen_width - game->screen_width * 0.6)
		* 0.5 - 25 + game->screen_width * 0.6 + 5;
	const int	scroll_bar_y = (game->screen_height - game->screen_height * 0.6)
		* 0.25 + 95;

	update_stats_menu_keycode(game, keycode);
	if (keycode == 1 && x >= scroll_bar_x && x <= scroll_bar_x + 10
		&& y >= scroll_bar_y
		&& y <= scroll_bar_y + game->screen_height * 0.6 - 110)
	{
		game->menu->dragging = true;
		game->menu->scroll = ((float)(y - scroll_bar_y)
				/ (game->screen_height * 0.6 - 110))
			* (game->menu->nb_scroll - 27);
		if (game->menu->scroll < 0)
			game->menu->scroll = 0;
		else if (game->menu->scroll > game->menu->nb_scroll - 27)
			game->menu->scroll = game->menu->nb_scroll - 27;
	}
}

void	update_stats_menu(t_game *game, int x, int y)
{
	const int	button_x = (game->screen_width - game->screen_width * 0.25) / 2;
	const int	button_y = (game->screen_height - game->screen_height * 0.6)
		* 0.25 + game->screen_height * 0.6 + 35;
	const int	button_height = game->screen_height * 0.1;
	const int	scroll_bar_x = (game->screen_width - game->screen_width * 0.6)
		* 0.5 - 25 + game->screen_width * 0.6 + 5;
	const int	scroll_bar_y = (game->screen_height - game->screen_height * 0.6)
		* 0.25 + 95;

	game->menu->button_selected = 0;
	if (x >= button_x && x <= button_x + game->screen_width * 0.25
		&& y >= button_y && y <= button_y + button_height)
		game->menu->button_selected = 1;
	if (!(x >= scroll_bar_x - 10 && x <= scroll_bar_x + 20 && y >= scroll_bar_y
			&& y <= scroll_bar_y + game->screen_height * 0.6 - 110))
		game->menu->dragging = false;
	is_dragging_scroll_bar(game, y, scroll_bar_y);
	check_mouse_on_gear(game, x, y);
}
