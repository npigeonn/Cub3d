/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:04:42 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 18:15:07 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	draw_selected_button_multi(t_game *game)
{
	t_draw_info	info;
	const int	list_width = game->screen_width * 0.65;
	const int	list_x = (game->screen_width - list_width) * 0.1;
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;

	info = init_draw_info(0, "", list_x + list_width
			+ ((game->screen_width - (list_x + list_width)) - btn_width)
			* 0.5 - 4, game->screen_height * 0.25 - 4);
	info.width = btn_width + 8;
	info.height = btn_height + 8;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	info.radius = 10;
	if (game->menu->button_selected == 1)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + game->screen_height * 0.05;
	if (game->menu->button_selected == 2)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + game->screen_height * 0.05;
	if (game->menu->button_selected == 3)
		draw_rounded_rectangle(game, info);
}

void	show_bg_servers_btn(t_game *game, int list_x, int server_y_offset,
int i)
{
	t_draw_info	info3;
	const int	list_width = game->screen_width * 0.65;

	info3 = init_draw_info(84, "", list_x + 8, server_y_offset - 2);
	info3.color = 0xda1254;
	info3.width = list_width - 16;
	info3.radius = 15;
	if (game->menu->server_selected == i)
		draw_rounded_rectangle(game, info3);
	info3 = init_draw_info(80, "", list_x + 10, server_y_offset);
	info3.color = MENU_BUTTON_SELECTED_COLOR;
	info3.width = list_width - 20;
	info3.radius = 15;
	draw_rounded_rectangle(game, info3);
}

void	draw_servers_case(t_game *game, int list_x, int list_y)
{
	t_draw_info	info;
	t_draw_info	info2;

	info = init_draw_info(game->screen_height * 0.1 * 0.5, "Available Servers",
			list_x + game->screen_width * 0.65 * 0.5, list_y + 20);
	info.color = MENU_BUTTON_TEXT_COLOR;
	info2 = init_draw_info(0, "", list_x, list_y);
	info2.height = game->screen_height * 0.8;
	info2.width = game->screen_width * 0.65;
	info2.color = MENU_BUTTON_COLOR;
	info2.radius = 25;
	draw_rounded_rectangle(game, info2);
	draw_text(game, info);
	show_list_servers(game, list_x, list_y + 50);
	draw_selected_button_multi(game);
}

void	draw_servers_button(t_game *game, int btn_w, int btn_h, int spacing)
{
	const int	list_x = (game->screen_width - game->screen_width * 0.65) * 0.1;
	const int	remaining_space = game->screen_width
		- (list_x + game->screen_width * 0.65);
	const int	btn_x = list_x + game->screen_width * 0.65
		+ (remaining_space - btn_w) * 0.5;
	t_draw_info	info2;
	t_draw_info	info;

	init_info(game, &info, &info2, list_x);
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Join Server");
	info.x = btn_x + btn_w * 0.5;
	info.y = game->screen_height * 0.25 + btn_h * 0.33 - 5;
	draw_text(game, info);
	info2.y += btn_h + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Create Server");
	info.y = game->screen_height * 0.25 + btn_h + spacing + btn_h * 0.33 - 5;
	draw_text(game, info);
	info2.y += btn_h + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Back");
	info.y = game->screen_height * 0.25 + 2 * (btn_h + spacing) + btn_h
		* 0.33 - 5;
	draw_text(game, info);
}

void	draw_multiplayer_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	list_width = game->screen_width * 0.65;
	const int	list_height = game->screen_height * 0.8;

	draw_gear_icon(game, game->screen_width - game->screen_width * 0.035 - 17,
		15, game->screen_width * 0.035);
	draw_servers_case(game, (game->screen_width - list_width)
		* 0.1, (game->screen_height - list_height) * 0.35);
	draw_servers_button(game, btn_width, btn_height, spacing);
}
