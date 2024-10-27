/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:50:42 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/27 17:26:12 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_stat_info(t_game *game, char *stat, int x, int y)
{
	t_draw_info stat_info = init_draw_info(20, stat, x, y);
	stat_info.color = 0xFFFFFF;
	draw_text_right(game, stat_info);
}

void	update_stats_menu_click(t_game *game, int x, int y, int keycode)
{
	int	scroll_bar_x = (game->screen_width - game->screen_width * 0.6) * 0.5 - 25 + game->screen_width * 0.6 + 5;
	int	scroll_bar_width = 10;
	int	scroll_bar_height = (game->menu->scroll_height * 27) / game->menu->nb_scroll;
	int	scroll_bar_y = (game->screen_height - game->screen_height * 0.6) * 0.25 + 95;

	if (keycode == 1)
	{
		if (game->menu->button_selected == 1)
			game->menu->status = MAIN_MENU;
		if (game->menu->button_selected == 5)
		{
			game->menu->status = OPTIONS_KEYBOARD;
			game->menu->last_status = STATS;
		}
		game->menu->button_selected = 0;
		game->menu->scroll = 0;
	}
	if (keycode == 5)
		game->menu->scroll += 1;
	if (keycode == 4)
	{
		game->menu->scroll -= 1;
		if (game->menu->scroll < 0)
			game->menu->scroll = 0;
	}
	if (keycode == 1 && x >= scroll_bar_x && x <= scroll_bar_x + scroll_bar_width && y >= scroll_bar_y && y <= scroll_bar_y + game->screen_height * 0.6 - 110)
	{
		game->menu->dragging = true;
		game->menu->scroll = ((y - scroll_bar_y) * (game->menu->nb_scroll - 27)) / (game->menu->scroll_height - scroll_bar_height);
		if (game->menu->scroll < 0)
			game->menu->scroll = 0;
		else if (game->menu->scroll > game->menu->nb_scroll - 27)
			game->menu->scroll = game->menu->nb_scroll - 27;
	}
}

void	update_stats_menu(t_game *game, int x, int y)
{
	const int	button_x = (game->screen_width - game->screen_width * 0.25) / 2;
	const int	button_y = (game->screen_height - game->screen_height * 0.6) * 0.25 + game->screen_height * 0.6 + 35;
	const int	button_width = game->screen_width * 0.25;
	const int	button_height = game->screen_height * 0.1;
	const int	scroll_bar_x = (game->screen_width - game->screen_width * 0.6) * 0.5 - 25 + game->screen_width * 0.6 + 5;
	const int	scroll_bar_width = 20;
	const int	scroll_bar_y = (game->screen_height - game->screen_height * 0.6) * 0.25 + 95;
	const int	scroll_bar_height = (game->menu->scroll_height * 27) / game->menu->nb_scroll;

	game->menu->button_selected = 0;
	if (x >= button_x && x <= button_x + button_width
		&& y >= button_y && y <= button_y + button_height)
		game->menu->button_selected = 1;
	if (!(x >= scroll_bar_x - 10 && x <= scroll_bar_x + scroll_bar_width && y >= scroll_bar_y && y <= scroll_bar_y + game->screen_height * 0.6 - 110))
		game->menu->dragging = false;
	if (game->menu->dragging)
	{
		game->menu->scroll = ((y - scroll_bar_y) * (game->menu->nb_scroll - 27)) / (game->menu->scroll_height - scroll_bar_height);
		if (game->menu->scroll < 0)
			game->menu->scroll = 0;
		else if (game->menu->scroll > game->menu->nb_scroll - 27)
			game->menu->scroll = game->menu->nb_scroll - 27;
	}
	check_mouse_on_gear(game, x, y);
}

void draw_stats_scroll_bar(t_game *game, int x, int y, int stats_height, int num_players)
{
	int	bar_width = 10;
	int	bar_height;

	if (num_players <= 27)
		bar_height = stats_height;
	else
		bar_height = (stats_height * 27) / num_players;

	int	bar_y = y + ((stats_height - bar_height) * game->menu->scroll) / (num_players - 27);
	int	bar_color = 0xAAAAAA;

	t_draw_info scroll_bar_background = init_draw_info(stats_height, "", x + game->screen_width * 0.6 + 5, y);
	scroll_bar_background.width = bar_width;
	scroll_bar_background.color = 0x333333;
	draw_rectangle(game, scroll_bar_background);

	t_draw_info scroll_bar = init_draw_info(bar_height, "", x + game->screen_width * 0.6 + 5, bar_y);
	scroll_bar.width = bar_width;
	scroll_bar.color = bar_color;
	draw_rectangle(game, scroll_bar);
}

void	draw_player_stats_row(t_game *game, t_player_stats player, int x, int y, int padding, int row_height, int index, int stats_width)
{
	int	row_y = y + (4 + index) * padding + 20;
	int	row_color = (index % 2 == 0) ? MENU_BUTTON_SELECTED_COLOR : MENU_BUTTON_SELECTED_COLOR - 0x101010;
	int	text_y = row_y + 8;

	t_draw_info player_row_background = init_draw_info(0, "", x + padding, row_y);
	player_row_background.width = (0.6 * game->screen_width) - 2 * padding - 10;
	player_row_background.height = row_height;
	player_row_background.color = row_color;
	draw_rectangle(game, player_row_background);

	t_draw_info name_info = init_draw_info(20, player.name, x + padding + 5, text_y);
	name_info.color = 0xFFFFFF;
	draw_text_left(game, name_info);
	draw_stat_info(game, ft_itoa(player.games_played), x + stats_width - padding - 423, text_y);
	draw_stat_info(game, ft_itoa(player.victories), x + stats_width - padding - 312, text_y);
	draw_stat_info(game, ft_itoa(player.defeats), x + stats_width - padding - 207, text_y);
	draw_stat_info(game, ft_itoa(player.kills), x + stats_width - padding - 138, text_y);
	draw_stat_info(game, ft_itoa(player.play_time_hours), x + stats_width - padding - 20, text_y);
}

void	draw_back_button(t_game *game, int x, int y, int stats_height)
{
	const int	button_x = (game->screen_width - game->screen_width * 0.25) / 2;
	const int	button_y = y + stats_height + 35;
	t_draw_info	info;

	if (game->menu->button_selected == 1)
	{
		info = init_draw_info(game->screen_height * 0.1 + 8, "",
			button_x - 4, button_y - 4);
		info.width = game->screen_width * 0.25 + 8;
		info.color = MENU_BUTTON_SELECTED_COLOR;
		info.radius = 10;
		draw_rounded_rectangle(game, info);
	}
	info = init_draw_info(game->screen_height * 0.1, "", button_x, button_y);
	info.width = game->screen_width * 0.25;
	info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, info);
	info = init_draw_info(game->screen_height * 0.1 * 0.5, "Back",
		(game->screen_width >> 1) - game->screen_width * 0.25 / 2 + 4 +
		game->screen_width * 0.25 / 2, y + stats_height + 35 +
		game->screen_height * 0.1 / 3 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_stats_menu(t_game *game)
{
	t_player_stats	*player_stats;
	const int		stats_width = game->screen_width * 0.6;
	const int		stats_height = game->screen_height * 0.6;
	const int		x = (game->screen_width - stats_width) * 0.5;
	const int		y = (game->screen_height - stats_height) * 0.25;
	const int		padding = 20;
	const int		row_height = 20;
	int				num_players;
	int				separator_y = y + 3 * padding + 20;

	player_stats = load_player_stats("stats.txt", &num_players);
	if (!player_stats)
		return;
	game->menu->nb_scroll = num_players;
	game->menu->scroll_height = stats_height - 110;
	t_draw_info stats_background = init_draw_info(0, "", x, y);
	stats_background.width = stats_width;
	stats_background.height = stats_height;
	stats_background.color = MENU_BUTTON_COLOR;
	stats_background.radius = 25;
	draw_rounded_rectangle(game, stats_background);

	t_draw_info title_info = init_draw_info(30, "Player Statistics", x + stats_width * 0.5, y + padding);
	title_info.color = 0xFFFFFF;
	draw_text(game, title_info);

	t_draw_info header_name_info = init_draw_info(20, "Name", x + padding, y + 2 * padding + 22);
	header_name_info.color = 0xFFFF00;
	draw_text_left(game, header_name_info);

	t_draw_info header_stats_info = init_draw_info(20, "Games      Victories      Defeats      Kills      Play Time", x + stats_width - padding - 20, y + 2 * padding + 22);
	header_stats_info.color = 0xFFFF00;
	draw_text_right(game, header_stats_info);

	draw_line(game, x + padding, separator_y, x + stats_width - padding, separator_y, 0xFFFFFF);
	if (game->menu->scroll + 27 > num_players)
		game->menu->scroll = num_players - 27;
	if (num_players < 27)
		game->menu->scroll = 0;
	for (int i = 0; i < num_players && i < 27; i++)
		draw_player_stats_row(game, player_stats[i + game->menu->scroll], x, y - 6, padding, row_height, i, stats_width);
	draw_back_button(game, x, y, stats_height);
	draw_stats_scroll_bar(game, x - 25, y + 95, stats_height - 110, num_players);
	free(player_stats);
	const int	gear_size = game->screen_width * 0.035;
	const int	gear_x = game->screen_width - gear_size - 17;
	const int	gear_y = 15;
	draw_gear_icon(game, gear_x, gear_y, gear_size);
}
