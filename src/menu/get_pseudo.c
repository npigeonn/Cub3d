/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pseudo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:28:40 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:15:51 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_get_pseudo_click(t_game *game, int mouse_x, int mouse_y,
int keycode)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.40;

	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
	{
		if (game->client->pseudo[0] == '\0')
			game->menu->error_name = true;
		else
			game->menu->status = MAIN_MENU;
	}
	if (mouse_x >= x && mouse_x <= x + btn_width
		&& mouse_y >= y + 30 && mouse_y <= y + 30 + btn_height)
		game->menu->text_field_selected = 1;
	else
		game->menu->text_field_selected = 0;
	game->menu->button_selected = 0;
}

void	update_get_pseudo_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	y = game->screen_height * 0.40;

	game->menu->button_selected = 0;
	if (mouse_x >= (game->screen_width - btn_width) * 0.5
		&& mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y + 200 && mouse_y <= y + 200 + btn_height)
			game->menu->button_selected = 1;
	}
}

static void	draw_text_field_with_feedback(t_game *game, int x, int y)
{
	t_draw_info	info;
	const int	width = game->screen_width * 0.25;
	const int	height = game->screen_height * 0.08;

	if (game->menu->text_field_selected == 1)
	{
		info = init_draw_info(height + 8, "", x - 4, y - 4);
		info.color = MENU_BUTTON_SELECTED_COLOR;
		info.width = width + 8;
		info.radius = 10;
		draw_rounded_rectangle(game, info);
	}
	if (game->menu->error_name)
	{
		info = init_draw_info(height + 8, "", x - 4, y - 4);
		info.color = 0xFF0000;
		info.width = width + 8;
		info.radius = 10;
		draw_rounded_rectangle(game, info);
	}
	info = init_draw_info(height, "", x, y);
	info.width = width;
	draw_text_field(game, info, game->client->pseudo);
}

static void	draw_continue_button(t_game *game, int x, int y)
{
	t_draw_info	info;
	const int	width = game->screen_width * 0.25;
	const int	height = game->screen_height * 0.08;

	if (game->menu->button_selected == 1)
	{
		info = init_draw_info(height + 8, "", x - 4, y - 4);
		info.color = MENU_BUTTON_SELECTED_COLOR;
		info.width = width + 8;
		info.radius = 10;
		draw_rounded_rectangle(game, info);
	}
	info = init_draw_info(0, "", x, y);
	info.color = MENU_BUTTON_COLOR;
	info.height = height;
	info.width = width;
	draw_rectangle(game, info);
	info = init_draw_info(height * 0.5, "Continue",
			x + width * 0.5, y + height * 0.33 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_get_pseudo_menu(t_game *game)
{
	const int	x = (game->screen_width - (game->screen_width * 0.25)) * 0.5;
	const int	y = game->screen_height * 0.40;
	t_draw_info	info;

	info = init_draw_info(game->screen_height * 0.08 * 0.9, "Enter your pseudo",
			game->screen_width >> 1, y - 90);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
	draw_text_field_with_feedback(game, x, y + 30);
	draw_continue_button(game, x, y + 200);
}
