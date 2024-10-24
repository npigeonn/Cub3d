/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 23:32:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/24 09:13:31 by ybeaucou         ###   ########.fr       */
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
	if (game->menu->button_selected == 1)
		draw_rectangle(game, info);
	info.y += btn_height + spacing - 4;
	if (game->menu->button_selected == 2)
		draw_rectangle(game, info);
	info.y += btn_height + spacing - 4;
	if (game->menu->button_selected == 3)
		draw_rectangle(game, info);
	info.y += btn_height + spacing - 4;
	if (game->menu->button_selected == 4)
		draw_rectangle(game, info);
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

	info = init_draw_info(btn_height * 0.5, "Solo", x + btn_width * 0.5, y + btn_height * 0.33 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	info2 = init_draw_info(0, "y", x, y);
	info2.color = MENU_BUTTON_COLOR;
	info2.height = btn_height;
	info2.width = btn_width;
	draw_selected_button(game);
	draw_rectangle(game, info2);
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	info.str = "Multi";
	info.y += btn_height + spacing;
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	info.str = "Options";
	info.y += btn_height + spacing;
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	info.str = "Exit";
	info.y += btn_height + spacing;
	draw_text(game, info);
}
