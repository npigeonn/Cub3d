/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:06:07 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 18:02:18 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_slider(t_game *game, int x, int y, float value)
{
	const int	width = game->screen_width * 0.9 * 0.3;
	const int	filled_width = width * (int)value / 100;
	t_draw_info	info;
	const int	height = game->screen_height * 0.025;

	info = init_draw_info(height, "", x, y);
	info.width = width;
	info.color = 0x333333;
	draw_rectangle(game, info);
	info.width = filled_width;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	draw_rectangle(game, info);
}

static void	draw_back_button_highlight(t_game *game)
{
	const float	button_width = (game->screen_width * 0.9) * 0.25;
	const float	button_height = game->screen_height * 0.09;
	const float	button_x = (game->screen_width - button_width) * 0.5;
	const float	button_y = (game->screen_height - game->screen_height * 0.9)
		* 0.5 + (game->screen_height * 0.9) * 0.9 - 25;
	t_draw_info	rect_info;

	rect_info = init_draw_info(
			button_height + 8, "", button_x - 4, button_y - 4);
	rect_info.width = button_width + 8;
	rect_info.radius = 10;
	rect_info.color = MENU_BUTTON_SELECTED_COLOR;
	draw_rounded_rectangle(game, rect_info);
}

static void	draw_back_button_base(t_game *game)
{
	const float	button_width = (game->screen_width * 0.9) * 0.25;
	const float	button_height = game->screen_height * 0.09;
	const float	button_x = (game->screen_width - button_width) * 0.5;
	const float	button_y = (game->screen_height - game->screen_height * 0.9)
		* 0.5 + (game->screen_height * 0.9) * 0.9 - 25;
	t_draw_info	rect_info;

	rect_info = init_draw_info(
			button_height, "", button_x, button_y);
	rect_info.color = MENU_BUTTON_COLOR;
	rect_info.width = button_width;
	draw_rectangle(game, rect_info);
}

static void	draw_back_button_text(t_game *game)
{
	t_draw_info	info;
	const float	button_width = (game->screen_width * 0.9) * 0.25;
	const float	button_height = game->screen_height * 0.09;
	const float	button_x = (game->screen_width - button_width) * 0.5;
	const float	button_y = (game->screen_height - game->screen_height * 0.9)
		* 0.5 + (game->screen_height * 0.9) * 0.9 - 25;

	info = init_draw_info(
			button_height * 0.6, "Back", button_x + button_width * 0.5,
			button_y + button_height * 0.33 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_back_buttons(t_game *game)
{
	if (game->menu->button_selected == 4)
		draw_back_button_highlight(game);
	draw_back_button_base(game);
	draw_back_button_text(game);
}
