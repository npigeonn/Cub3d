/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:55:57 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 13:15:40 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	draw_text_fields(t_game *game, int x, int y, int btn_width)
{
	t_draw_info	info;
	t_draw_info	info3;
	const int	btn_height = game->screen_height * 0.08;

	if (game->menu->text_field_selected == 1 || game->menu->error_name)
	{
		info3 = init_draw_info(btn_height + 8, "", x - 4, y + 30 - 4);
		if (game->menu->text_field_selected == 1)
			info3.color = MENU_BUTTON_SELECTED_COLOR;
		else
			info3.color = 0xFF0000;
		info3.width = btn_width + 8;
		info3.radius = 10;
		draw_rounded_rectangle(game, info3);
	}
	info = init_draw_info(btn_height * 0.5, "Server IP",
			x + btn_width / 2, y - 10);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
	info = init_draw_info(btn_height, "", x, y + 30);
	info.width = btn_width;
	draw_text_field(game, info, game->client->ip);
}

static void	draw_button(t_game *game, int info_n[5], const char *label)
{
	t_draw_info	info;
	t_draw_info	info3;

	if (game->menu->button_selected == info_n[4])
	{
		info3 = init_draw_info(info_n[3] + 8, "", info_n[0] - 4, info_n[1] - 4);
		info3.color = MENU_BUTTON_SELECTED_COLOR;
		info3.width = info_n[2] + 8;
		info3.radius = 10;
		draw_rounded_rectangle(game, info3);
	}
	info = init_draw_info(info_n[3] * 0.5, label, info_n[0] + info_n[2] / 2,
			info_n[1] + info_n[3] / 3 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	info3 = init_draw_info(0, "", info_n[0], info_n[1]);
	info3.color = MENU_BUTTON_COLOR;
	info3.height = info_n[3];
	info3.width = info_n[2];
	draw_rectangle(game, info3);
	draw_text(game, info);
}

static void	draw_buttons(t_game *game, int x, int y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	btn_half_width = btn_width * 0.45;
	const int	spacing = game->screen_width * 0.02;
	int			info[5];

	info[0] = x;
	info[1] = y;
	info[2] = btn_half_width;
	info[3] = btn_height;
	info[4] = 1;
	draw_button(game, info, "Back");
	info[0] = x + btn_half_width + spacing;
	info[4] = 2;
	draw_button(game, info, "Join");
}

static void	draw_header(t_game *game, int y)
{
	t_draw_info	info;

	info = init_draw_info(game->screen_height * 0.08 * 0.9, "Join Server",
			game->screen_width >> 1, y - 200);
	info.color = 0xFFFFFF;
	draw_text(game, info);
}

void	draw_join_server_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	spacing = game->screen_height * 0.06;
	const int	x = (game->screen_width - btn_width) / 2;
	const int	y = game->screen_height * 0.40;

	draw_header(game, y);
	draw_text_fields(game, x, y, btn_width);
	draw_buttons(game, (game->screen_width - (btn_width * 0.9)) / 2 - 19,
		y + (btn_height + spacing));
	draw_gear_icon(game, game->screen_width - game->screen_width
		* 0.035 - 17, 15, game->screen_width * 0.035);
}
