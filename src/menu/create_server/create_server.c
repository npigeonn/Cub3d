/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_server.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:49:50 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:16:56 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_button_with_selection(t_game *game, t_draw_info btn_info,
int is_selected, char *label)
{
	t_draw_info	info;

	if (is_selected)
	{
		info = init_draw_info(btn_info.height + 8, "", btn_info.x - 4,
				btn_info.y - 4);
		info.color = MENU_BUTTON_SELECTED_COLOR;
		info.width = btn_info.width + 8;
		info.radius = 10;
		draw_rounded_rectangle(game, info);
	}
	btn_info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, btn_info);
	info = init_draw_info(btn_info.height * 0.5, label, btn_info.x
			+ btn_info.width / 2, btn_info.y + btn_info.height / 3 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_create_server_name(t_game *game, int btn_height, int y)
{
	t_draw_info	info;

	info = init_draw_info(btn_height * 0.9, "Create Server",
			game->screen_width >> 1, y - 140);
	info.color = 0xFFFFF;
	draw_text(game, info);
}

void	draw_create_server_error(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.33;
	t_draw_info	info;

	if (game->menu->text_field_selected == 1 || game->menu->error_name)
	{
		info = init_draw_info(btn_height + 8, "", x - 4, y + 30 - 4);
		if (game->menu->error_name)
			info.color = 0xFF0000;
		else
			info.color = MENU_BUTTON_SELECTED_COLOR;
		info.width = btn_width + 8;
		info.radius = 10;
		draw_rounded_rectangle(game, info);
	}
}

void	draw_buttons_create_server(t_game *game, int btn_width, int btn_height,
int y)
{
	const int	btn_h_width = btn_width * 0.45;
	const int	total_btn_width = 2 * btn_h_width + game->screen_width * 0.02;
	const int	btn_x_start = (game->screen_width - total_btn_width) / 2;
	const int	btn_y_start = y + 2 * (btn_height + game->screen_height * 0.06);
	t_draw_info	btn_info;

	btn_info = init_draw_info(btn_height, "", btn_x_start, btn_y_start);
	btn_info.width = btn_h_width;
	draw_button_with_selection(game, btn_info,
		game->menu->button_selected == 1, "Back");
	btn_info = init_draw_info(btn_height, "", btn_x_start + btn_h_width
			+ game->screen_width * 0.02, btn_y_start);
	btn_info.width = btn_h_width;
	draw_button_with_selection(game, btn_info,
		game->menu->button_selected == 2, "Create");
	draw_gear_icon(game, game->screen_width - game->screen_width * 0.035 - 17,
		15, game->screen_width * 0.035);
}

void	draw_create_server_menu(t_game *game)
{
	char		*ip;
	const int	btn_height = game->screen_height * 0.08;
	const int	x = (game->screen_width - game->screen_width * 0.25) * 0.5;
	const int	y = game->screen_height * 0.33;
	t_draw_info	info;

	draw_create_server_name(game, btn_height, y);
	draw_create_server_error(game);
	info = init_draw_info(btn_height, "Server Name", x, y + 30);
	info.width = game->screen_width * 0.25;
	draw_labeled_text_field(game, gc_strdup(game->mem, game->client->name),
		info);
	info = init_draw_info(btn_height, "Server IP", x,
			y + (btn_height + game->screen_height * 0.06) + 30);
	info.width = game->screen_width * 0.25;
	ip = get_local_ip(game);
	draw_labeled_text_field(game, ip, info);
	draw_buttons_create_server(game, game->screen_width * 0.25, btn_height, y);
}
