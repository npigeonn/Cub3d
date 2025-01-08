/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:50:42 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 16:42:40 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_stat_info(t_game *game, char *stat, int x, int y)
{
	t_draw_info	stat_info;

	stat_info = init_draw_info(20, stat, x, y);
	stat_info.color = 0xFFFFFF;
	draw_text_right(game, stat_info);
}

void	draw_player_stats(t_game *game, t_player_stats player, t_draw_info info,
int text_y)
{
	t_draw_info	name_info;	

	name_info = init_draw_info(20, player.name, info.x + info.padding + 5,
			text_y);
	name_info.color = 0xFFFFFF;
	draw_text_left(game, name_info);
	draw_stat_info(game, gc_itoa(game->mem, player.games_played), info.x
		+ info.width - info.padding - 423, text_y);
	draw_stat_info(game, gc_itoa(game->mem, player.victories), info.x
		+ info.width - info.padding - 312, text_y);
	draw_stat_info(game, gc_itoa(game->mem, player.defeats), info.x
		+ info.width - info.padding - 207, text_y);
	draw_stat_info(game, gc_itoa(game->mem, player.kills), info.x
		+ info.width - info.padding - 138, text_y);
	draw_stat_info(game, gc_itoa(game->mem, (int)(player.play_time_hours)),
		info.x + info.width - info.padding - 20, text_y);
}

void	draw_player_stats_row(t_game *game, t_player_stats player,
t_draw_info info, int index)
{
	const int	row_y = info.y + (4 + index) * info.padding + 20;
	int			row_color;
	const int	text_y = row_y + 8;
	t_draw_info	player_row_background;

	row_color = MENU_BUTTON_SELECTED_COLOR;
	if (index % 2 != 0)
		row_color = MENU_BUTTON_SELECTED_COLOR - 0x101010;
	player_row_background = init_draw_info(0, "", info.x + info.padding, row_y);
	player_row_background.width = (0.6 * game->screen_width) - 2
		* info.padding - 10;
	player_row_background.height = info.height;
	player_row_background.color = row_color;
	draw_rectangle(game, player_row_background);
	draw_player_stats(game, player, info, text_y);
}

void	draw_stats_menu(t_game *game)
{
	const int	stats_width = game->screen_width * 0.6;
	const int	stats_height = game->screen_height * 0.6;
	const int	x = (game->screen_width - stats_width) * 0.5;
	const int	y = (game->screen_height - stats_height) * 0.25;

	game->menu->scroll_height = stats_height - 110;
	draw_stats_menu_bg(game, stats_width, stats_height);
	draw_stats_menu_header(game, stats_width, stats_height);
	draw_stats_menu_player_scroll(game, x, y, stats_height);
	draw_gear_icon(game, game->screen_width - game->screen_width * 0.035 - 17,
		15, game->screen_width * 0.035);
}
