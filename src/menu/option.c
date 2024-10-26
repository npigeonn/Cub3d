/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 14:42:20 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/26 03:31:50 by ybeaucou         ###   ########.fr       */
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
		game->menu->dragging = true;
	}
	else if (game->menu->button_selected == 2)
	{
		game->menu->mouse_sensitivity = (mouse_x - x) * 100 / btn_width;
		if (game->menu->mouse_sensitivity < 0.0001) game->menu->mouse_sensitivity = 0.0001;
		if (game->menu->mouse_sensitivity > 100) game->menu->mouse_sensitivity = 100;
		game->menu->dragging = true;
	}
	else
		game->menu->dragging = false;
}

void	update_option_menu_click(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 3)
	{
		game->menu->status = game->menu->last_status;
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
	{
		game->menu->button_selected = 0;
		game->menu->dragging = false;
	}
	if (game->menu->dragging && game->menu->button_selected == 1)
	{
		game->menu->volume = (mouse_x - x) * 100 / btn_width;
		if (game->menu->volume < 0) game->menu->volume = 0;
		if (game->menu->volume > 100) game->menu->volume = 100;
	}
	else if (game->menu->dragging && game->menu->button_selected == 2)
	{
		game->menu->mouse_sensitivity = (mouse_x - x) * 100 / btn_width;
		if (game->menu->mouse_sensitivity < 0.0001) game->menu->mouse_sensitivity = 0.0001;
		if (game->menu->mouse_sensitivity > 100) game->menu->mouse_sensitivity = 100;
	}
}


static void	draw_slider(t_game *game, int x, int y, int width, int height, float value)
{
	const int	slider_width = width;
	const int	slider_height = height * 0.33;
	const int	cursor_width = 10;
	const int	cursor_x = x + (int)(value * (slider_width - cursor_width));
	t_draw_info	info;

	info = init_draw_info(0, "", x, (int)(y + slider_height * 0.5));
	info.width = slider_width;
	info.height = slider_height;
	info.color = 0xCCCCCC;
	draw_rectangle(game, info);
	info = init_draw_info(0, "", cursor_x, y);
	info.height = slider_height + cursor_width * 2 + slider_height * 0.5;
	info.width = cursor_width;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	draw_rectangle(game, info);
}

void	draw_options_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;
	t_draw_info	info2;
	t_draw_info	info;

	info = init_draw_info(btn_height * 0.5, "Volume", game->screen_width >> 1, y - 20);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
	draw_slider(game, x, y + 30, btn_width, btn_height, game->menu->volume / 100); // menu->Volume entre 0 et 100

	ft_strcpy(info.str, "Sensitive");
	info.y = y + btn_height + spacing - 20;
	draw_text(game, info);
	draw_slider(game, x, y + btn_height + spacing + 30, btn_width, btn_height, game->menu->mouse_sensitivity / 100); // Sensibilité entre 0 et 100

	info2 = init_draw_info(0, "", x - 2, y + 2 * (btn_height + spacing) - 2);
	info2.height = btn_height + 4;
	info2.width =  btn_width + 4;
	info2.color = MENU_BUTTON_SELECTED_COLOR;
	if (game->menu->button_selected == 3)
		draw_rectangle(game, info2);
	info2 = init_draw_info(0, "", x, y + 2 * (btn_height + spacing));
	info2.height = btn_height;
	info2.width = btn_width;
	info2.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Back");
	info.y = y + 2 * (btn_height + spacing) + btn_height / 3 - 5;
	draw_text(game, info);
}
