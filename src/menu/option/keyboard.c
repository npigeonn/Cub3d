/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:59:03 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 13:36:44 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_key_highlight(t_game *game, int key_x, int key_height,
int info[3])
{
	t_draw_info	highlight_info;

	highlight_info = init_draw_info(key_height + 10, "", key_x - 10,
			info[2] - 10);
	highlight_info.width = info[0] * 0.15 + 10;
	highlight_info.color = 0xFFFFFF;
	draw_rectangle(game, highlight_info);
}

static void	draw_key_rectangles(t_game *game, int key_x1, int key_x2,
int info[3])
{
	t_draw_info	key_info;
	const int	key_height = game->screen_height * 0.03 + 10;

	key_info = init_draw_info(key_height, "", key_x1 - 5, info[2] - 5);
	key_info.width = info[0] * 0.15;
	key_info.color = MENU_BUTTON_SELECTED_COLOR;
	draw_rectangle(game, key_info);
	key_info.x = key_x2 - 5;
	draw_rectangle(game, key_info);
}

static void	draw_key_text(t_game *game, t_key_binding *bindings, int i,
int info[3])
{
	t_draw_info	key_info;
	const int	key_x1 = (info[0] - info[0] * 0.5) * 0.5 + info[0] * 0.24;
	const int	key_x2 = key_x1 + info[0] * 0.2;

	key_info = init_draw_info(game->screen_height * 0.03, bindings[i].key1,
			key_x1, info[2] + 8);
	key_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, key_info);
	key_info = init_draw_info(game->screen_height * 0.03, bindings[i].key2,
			key_x2, info[2] + 8);
	key_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, key_info);
}

static void	draw_binding_option(t_game *game, t_key_binding *bindings, int i,
int info[3])
{
	const int	key_x1 = (info[0] - info[0] * 0.5) * 0.5 + info[0] * 0.24;
	const int	key_x2 = key_x1 + info[0] * 0.2;
	const int	key_height = game->screen_height * 0.03 + 10;
	t_draw_info	key_info;

	key_info = init_draw_info(game->screen_height * 0.03, bindings[i].action,
			(info[0] - info[0] * 0.5) * 0.47 + info[1], info[2] + 8);
	key_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, key_info);
	if (game->menu->text_field_selected == (i * 2 + 1))
		draw_key_highlight(game, key_x1, key_height, info);
	else if (game->menu->text_field_selected == (i * 2 + 2))
		draw_key_highlight(game, key_x2, key_height, info);
	draw_key_rectangles(game, key_x1, key_x2, info);
	draw_key_text(game, bindings, i, info);
}

void	draw_options_keyboard(t_game *game)
{
	const float		panel_height = game->screen_height * 0.9;
	int				info[3];
	int				i;
	t_key_binding	*bindings;
	t_draw_info		header_info;

	info[0] = game->screen_width * 0.9;
	info[1] = (game->screen_width - info[0]) * 0.5;
	info[2] = (game->screen_height - panel_height) * 0.5 + panel_height * 0.30;
	bindings = get_binding(game);
	header_info = init_draw_info(game->screen_height * 0.06, "Key Bindings",
			info[1] + info[0] * 0.5, (game->screen_height - panel_height)
			* 0.5 + 50 + panel_height * 0.1);
	header_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, header_info);
	i = -1;
	while (++i < 8)
	{
		draw_binding_option(game, bindings, i, info);
		info[2] += 50;
	}
}
