/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_over.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:35:41 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 16:38:11 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_game_over_click(t_game *game, int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
	{
		save_player_stats(game, "stats.txt");
		reset_game(game);
	}
	game->menu->button_selected = 0;
	game->menu->text_field_selected = 0;
}

void	update_game_over_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	y = game->screen_height * 0.25;

	game->menu->button_selected = 0;
	if (mouse_x >= (game->screen_width - btn_width) * 0.5
		&& mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y + 2 * (btn_height + spacing)
			&& mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 1;
	}
}

static void	draw_main_menu_button_gm(t_game *game, int x, int y, int btn_width)
{
	t_draw_info	info;
	t_draw_info	info2;
	const int	btn_height = game->screen_height * 0.1;

	info2 = init_draw_info(btn_height, "", x, y);
	info2.color = MENU_BUTTON_COLOR;
	info2.width = btn_width;
	draw_rectangle(game, info2);
	info = init_draw_info(0, "Main menu", x + btn_width / 2,
			y + btn_height * 0.33 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	info.height = btn_height * 0.5;
	draw_text(game, info);
}

static void	draw_main_menu_button_gm_hover(t_game *game, int x, int y,
int btn_width)
{
	t_draw_info	info;
	const int	btn_height = game->screen_height * 0.1;

	info = init_draw_info(0, "", x - 4, y - 4);
	info.width = btn_width + 8;
	info.height = btn_height + 8;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	info.radius = 10;
	draw_rounded_rectangle(game, info);
}

void	draw_game_over(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_h = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;

	draw_game_over_bg(game);
	draw_game_over_text(game);
	if (game->menu->button_selected == 1)
		draw_main_menu_button_gm_hover(game, x, y + 2 * (btn_h + spacing),
			btn_width);
	draw_main_menu_button_gm(game, x, y + 2 * (btn_h + spacing), btn_width);
	game->chatbox->visible = false;
	game->chatbox->is_writting = false;
}
