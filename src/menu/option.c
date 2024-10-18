/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 14:42:20 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/15 12:47:01 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_option_menu_slider(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	const int		btn_width = game->screen_width * 0.25;
	const int		btn_height = game->screen_height * 0.1;
	const int		spacing = game->screen_height * 0.05;
	const int		x = (game->screen_width - btn_width) * 0.5;
	const int		y = game->screen_height * 0.25;

	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
	{
		game->menu->volume = (mouse_x - x) * 100 / btn_width;
		if (game->menu->volume < 0) game->menu->volume = 0;
		if (game->menu->volume > 100) game->menu->volume = 100;
	}
	else if (game->menu->button_selected == 2)
	{
		game->menu->mouse_sensitivity = (mouse_x - x) * 10.0f / btn_width;
		if (game->menu->mouse_sensitivity < 0.1f) game->menu->mouse_sensitivity = 0.1f;
		if (game->menu->mouse_sensitivity > 10.0f) game->menu->mouse_sensitivity = 10.0f;
	}
}

void	update_option_menu_click(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 3)
	{
		game->menu->status = MAIN_MENU;
		game->menu->button_selected = 0;
		return ;
	}
	update_option_menu_slider(game, mouse_x, mouse_y, keycode);
}

void	update_option_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;

	if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y && mouse_y <= y + btn_height)
		game->menu->button_selected = 1;
	else if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + btn_height + spacing && mouse_y <= y + 2 * btn_height + spacing)
		game->menu->button_selected = 2;
	else if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + 2 * (btn_height + spacing) && mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
		game->menu->button_selected = 3;
	else
		game->menu->button_selected = 0;
}


static void	draw_slider(t_game *game, int x, int y, int width, int height, float value)
{
	const int	slider_width = width;
	const int	slider_height = height * 0.33;
	const int	cursor_width = 10;
	const int	cursor_x = x + (int)(value * (slider_width - cursor_width));

	draw_rectangle(game, x, y + slider_height * 0.5, slider_width, slider_height, 0xCCCCCC);
	draw_rectangle(game, cursor_x, y, cursor_width, slider_height + cursor_width * 2 + slider_height * 0.5, MENU_BUTTON_SELECTED_COLOR);
}

void	draw_options_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;

	draw_text(game, "Volume", game->screen_width >> 1, y - 20, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_slider(game, x, y + 30, btn_width, btn_height, game->menu->volume / 100.0f); // menu->Volume entre 0 et 100

	draw_text(game, "Sensitivity", game->screen_width >> 1, y + btn_height + spacing - 20, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_slider(game, x, y + btn_height + spacing + 30, btn_width, btn_height, game->menu->mouse_sensitivity / 10.0f); // Sensibilité entre 0.1 et 10

	if (game->menu->button_selected == 3)
		draw_rectangle(game, x - 2, y + 2 * (btn_height + spacing) - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, x, y + 2 * (btn_height + spacing), btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Back", game->screen_width >> 1, y + 2 * (btn_height + spacing) + btn_height / 3 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
}
