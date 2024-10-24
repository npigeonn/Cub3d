/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 23:32:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/24 14:58:21 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/cub3d.h"

void	update_main_menu_click(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
	{
		game->menu->status = PLAYING;
		mlx_mouse_hide(game->mlx, game->win);
	}
	else if (game->menu->button_selected == 2)
	{
		game->menu->status = SERVERS;
		pthread_create(&game->discover_servers_thread, NULL, discover_servers_thread, game);
	}
	else if (game->menu->button_selected == 3)
		game->menu->status = OPTIONS;
	else if (game->menu->button_selected == 4)
		handle_close(game);
	game->menu->button_selected = 0;
}

void	update_main_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	y = game->screen_height * 0.25;

	game->menu->button_selected = 0;
	if (mouse_x >= (game->screen_width - btn_width) * 0.5 && mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y && mouse_y <= y + btn_height)
			game->menu->button_selected = 1;
		else if (mouse_y >= y + btn_height + spacing && mouse_y <= y + btn_height + spacing + btn_height)
			game->menu->button_selected = 2;
		else if (mouse_y >= y + 2 * (btn_height + spacing) && mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 3;
		else if (mouse_y >= y + 3 * (btn_height + spacing) && mouse_y <= y + 3 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 4;
	}
}

static void	draw_selected_button(t_game *game)
{
	t_draw_info	info;
	const int	btn_width = game->screen_width * 0.25 + 4;
	const int	btn_height = game->screen_height * 0.1 + 4;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25 - 2;

	info = init_draw_info(0, "", x, y);
	info.width = btn_width;
	info.height = btn_height;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	info.radius = 10;
	if (game->menu->button_selected == 1)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + spacing - 4;
	if (game->menu->button_selected == 2)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + spacing - 4;
	if (game->menu->button_selected == 3)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + spacing - 4;
	if (game->menu->button_selected == 4)
		draw_rounded_rectangle(game, info);
}

void	draw_line(t_game *game, int x1, int y1, int x2, int y2, int color)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x1 < x2 ? 1 : -1;
	int sy = y1 < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2;

	while (1)
	{
		pixel_put(game, x1, y1, color);
		if (x1 == x2 && y1 == y2)
			break;
		e2 = err;
		if (e2 > -dx)
		{
			err -= dy;
			x1 += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			y1 += sy;
		}
	}
}

#include <math.h>

void	draw_gear_icon(t_game *game, int x, int y, int size)
{
	const int center_x = x + size / 2;
	const int center_y = y + size / 2;
	const int outer_radius = size / 2;
	const int inner_radius = size / 3;
	const int num_teeth = 8;
	const double angle_step = M_PI * 2 / num_teeth;
	const int tooth_width = size / 10;
	const int tooth_length = size / 8;

	draw_arc(game, center_x, center_y, inner_radius, 0, M_PI * 2, 0xFFFFFF);

	for (int i = 0; i < num_teeth; i++)
	{
		double angle = i * angle_step;
		double next_angle = (i + 1) * angle_step;

		int outer_x1 = center_x + cos(angle) * (outer_radius + tooth_length);
		int outer_y1 = center_y + sin(angle) * (outer_radius + tooth_length);
		int outer_x2 = center_x + cos(next_angle) * (outer_radius + tooth_length);
		int outer_y2 = center_y + sin(next_angle) * (outer_radius + tooth_length);

		int inner_x1 = center_x + cos(angle) * outer_radius;
		int inner_y1 = center_y + sin(angle) * outer_radius;
		int inner_x2 = center_x + cos(next_angle) * outer_radius;
		int inner_y2 = center_y + sin(next_angle) * outer_radius;
		draw_line(game, inner_x1, inner_y1, outer_x1, outer_y1, 0xFFFFFF);
		draw_line(game, outer_x1, outer_y1, outer_x2, outer_y2, 0xFFFFFF);
		draw_line(game, outer_x2, outer_y2, inner_x2, inner_y2, 0xFFFFFF);
		draw_line(game, inner_x2, inner_y2, inner_x1, inner_y1, 0xFFFFFF);
	}
	draw_arc(game, center_x, center_y, inner_radius / 2, 0, M_PI * 2, 0xFFFFFF);
}



void	draw_main_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;
	t_draw_info	info2;
	t_draw_info	info;

	draw_selected_button(game);
	info = init_draw_info(btn_height * 0.5, "Solo", x + btn_width * 0.5, y + btn_height * 0.33 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	info2 = init_draw_info(0, "y", x, y);
	info2.color = MENU_BUTTON_COLOR;
	info2.height = btn_height;
	info2.width = btn_width;
	draw_rectangle(game, info2);
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Multi");
	info.y += btn_height + spacing;
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Stats");
	info.y += btn_height + spacing;
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Exit");
	info.y += btn_height + spacing;
	draw_text(game, info);
	const int gear_size = game->screen_width * 0.05;
	const int gear_x = game->screen_width - gear_size - 10;
	const int gear_y = 10;
	draw_gear_icon(game, gear_x, gear_y, gear_size);
}

