/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 14:42:20 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/07 22:45:15 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_option_menu_slider(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	const int		btn_width = game->screen_width * 0.25;
	const int		btn_height = game->screen_height * 0.1;
	const int		spacing = game->screen_height * 0.05;
	const int		x = (game->screen_width - btn_width) / 2;
	const int		y = game->screen_height * 0.25;

	if (keycode != 1)
		return ;
	if (game->button_selected == 1)
	{
		game->volume = (mouse_x - x) * 100 / btn_width;
		if (game->volume < 0) game->volume = 0;
		if (game->volume > 100) game->volume = 100;
	}
	else if (game->button_selected == 2)
	{
		game->mouse_sensitivity = (mouse_x - x) * 10.0f / btn_width;
		if (game->mouse_sensitivity < 0.1f) game->mouse_sensitivity = 0.1f;
		if (game->mouse_sensitivity > 10.0f) game->mouse_sensitivity = 10.0f;
	}
}

void	update_option_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) / 2;
	const int	y = game->screen_height * 0.25;

	if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y && mouse_y <= y + btn_height)
		game->button_selected = 1;
	else if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + btn_height + spacing && mouse_y <= y + 2 * btn_height + spacing)
		game->button_selected = 2;
	else if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + 2 * (btn_height + spacing) && mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
		game->button_selected = 3;
	else
		game->button_selected = 0;
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

	draw_text(game, "Volume", x + btn_width * 0.5, y - 20, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_slider(game, x, y + 30, btn_width, btn_height, game->volume / 100.0f); // Volume entre 0 et 100

	draw_text(game, "Sensitivity", x + btn_width * 0.5, y + btn_height + spacing - 20, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_slider(game, x, y + btn_height + spacing + 30, btn_width, btn_height, game->mouse_sensitivity / 10.0f); // Sensibilité entre 0.1 et 10

	if (game->button_selected == 3)
		draw_rectangle(game, x - 2, y + 2 * (btn_height + spacing) - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, x, y + 2 * (btn_height + spacing), btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Back", x + btn_width * 0.5, y + 2 * (btn_height + spacing) + btn_height / 3 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
}
