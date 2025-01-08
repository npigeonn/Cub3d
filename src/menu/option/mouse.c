/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:57:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:31:32 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_option_menu_button_mouse(t_game *game, int mouse_x, int mouse_y)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const int	slider_x = (panel_width - panel_width * 0.3)
		* 0.5 + (game->screen_width - panel_width) * 0.5;
	const int	slider_y = (game->screen_height - panel_height)
		* 0.5 + panel_height * 0.4;

	if (!game->menu->dragging)
		return ;
	if (!(mouse_x >= slider_x && mouse_x <= slider_x + panel_width * 0.3
			&& mouse_y >= slider_y
			&& mouse_y <= slider_y + game->screen_height * 0.025))
	{
		game->menu->dragging = false;
		return ;
	}
	game->menu->mouse_sensitivity = ((float)(mouse_x - slider_x)
			/ (panel_width * 0.3)) * 100;
	if (game->menu->mouse_sensitivity < 0.0001)
		game->menu->mouse_sensitivity = 0.0001;
	if (game->menu->mouse_sensitivity > 100)
		game->menu->mouse_sensitivity = 100;
}

static void	draw_mouse_header(t_game *game)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	x = (game->screen_width - panel_width)
		* 0.5 + panel_width * 0.5;
	const float	y = (game->screen_height - panel_height)
		* 0.5 + 50 + panel_height * 0.1;
	t_draw_info	header_info;

	header_info = init_draw_info(game->screen_height * 0.06, "Mouse Options", x,
			y);
	header_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, header_info);
}

static void	draw_mouse_checkbox(t_game *game, float checkbox_x,
float checkbox_y, int checkbox_size)
{
	t_draw_info	checkbox_info;

	checkbox_info = init_draw_info(
			checkbox_size + 4, "", checkbox_x - 2, checkbox_y - 2);
	checkbox_info.color = 0xFFFFFF;
	checkbox_info.width = checkbox_size + 4;
	draw_rectangle(game, checkbox_info);
	checkbox_info = init_draw_info(checkbox_size, "", checkbox_x, checkbox_y);
	checkbox_info.color = MENU_BUTTON_COLOR;
	checkbox_info.width = checkbox_size;
	if (!game->player->invert_mouse_x)
		draw_rectangle(game, checkbox_info);
	checkbox_info.color = 0xFF0000;
	if (game->player->invert_mouse_x)
		draw_rectangle(game, checkbox_info);
}

static void	draw_mouse_option_name(t_game *game, int i, float panel_x,
int option_y)
{
	const float	panel_width = game->screen_width * 0.9;
	const char	*options[] = {"Mouse Sensitivity", "Invert Mouse X"};
	t_draw_info	info;

	info = init_draw_info(
			game->screen_height * 0.03, (char *)options[i],
			(panel_width - panel_width * 0.3) * 0.5 + panel_x, option_y);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, info);
}

void	draw_options_mouse(t_game *game)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	panel_x = (game->screen_width - panel_width) * 0.5;
	const float	panel_y = (game->screen_height - panel_height) * 0.5;
	float		option_y;

	option_y = panel_y + panel_height * 0.372;
	draw_mouse_header(game);
	draw_mouse_option_name(game, 0, panel_x, option_y);
	draw_slider(game, (panel_width - panel_width * 0.3) * 0.5 + panel_x,
		panel_y + panel_height * 0.4, game->menu->mouse_sensitivity);
	option_y += panel_height * 0.15;
	draw_mouse_option_name(game, 1, panel_x, option_y);
	draw_mouse_checkbox(game, (panel_x + panel_width * 0.135)
		+ panel_width * 0.5, option_y - 5, 20);
}
