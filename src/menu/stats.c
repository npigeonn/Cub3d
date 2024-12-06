/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:50:42 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/21 18:27:01 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_stat_info(t_game *game, char *stat, int x, int y)
{
	t_draw_info	stat_info;

	stat_info = init_draw_info(20, stat, x, y);
	stat_info.color = 0xFFFFFF;
	draw_text_right(game, stat_info);
}

void	update_stats_menu_keycode(t_game *game, int keycode)
{
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
}

void	update_stats_menu_click(t_game *game, int x, int y, int keycode)
{
	const int	scroll_bar_x = (game->screen_width - game->screen_width * 0.6)
		* 0.5 - 25 + game->screen_width * 0.6 + 5;
	const int	scroll_width = 10;
	const int	scroll_height = (game->menu->scroll_height * 27)
		/ game->menu->nb_scroll;
	const int	scroll_bar_y = (game->screen_height - game->screen_height * 0.6)
		* 0.25 + 95;

	update_stats_menu_keycode(game, keycode);
	if (keycode == 1 && x >= scroll_bar_x && x <= scroll_bar_x + scroll_width
		&& y >= scroll_bar_y
		&& y <= scroll_bar_y + game->screen_height * 0.6 - 110)
	{
		game->menu->dragging = true;
		game->menu->scroll = ((y - scroll_bar_y) * (game->menu->nb_scroll - 27))
			/ (game->menu->scroll_height - scroll_height);
		if (game->menu->scroll < 0)
			game->menu->scroll = 0;
		else if (game->menu->scroll > game->menu->nb_scroll - 27)
			game->menu->scroll = game->menu->nb_scroll - 27;
	}
}

void	is_dragging_scroll_bar(t_game *game, int y, int scroll_bar_y)
{
	if (game->menu->dragging)
	{
		game->menu->scroll = ((y - scroll_bar_y) * (game->menu->nb_scroll - 27))
			/ (game->menu->scroll_height - (game->menu->scroll_height * 27)
				/ game->menu->nb_scroll);
		if (game->menu->scroll < 0)
			game->menu->scroll = 0;
		else if (game->menu->scroll > game->menu->nb_scroll - 27)
			game->menu->scroll = game->menu->nb_scroll - 27;
	}
}

void	update_stats_menu(t_game *game, int x, int y)
{
	const int	button_x = (game->screen_width - game->screen_width * 0.25) / 2;
	const int	button_y = (game->screen_height - game->screen_height * 0.6)
		* 0.25 + game->screen_height * 0.6 + 35;
	const int	button_height = game->screen_height * 0.1;
	const int	scroll_bar_x = (game->screen_width - game->screen_width * 0.6)
		* 0.5 - 25 + game->screen_width * 0.6 + 5;
	const int	scroll_bar_y = (game->screen_height - game->screen_height * 0.6)
		* 0.25 + 95;

	game->menu->button_selected = 0;
	if (x >= button_x && x <= button_x + game->screen_width * 0.25
		&& y >= button_y && y <= button_y + button_height)
		game->menu->button_selected = 1;
	if (!(x >= scroll_bar_x - 10 && x <= scroll_bar_x + 20 && y >= scroll_bar_y
			&& y <= scroll_bar_y + game->screen_height * 0.6 - 110))
		game->menu->dragging = false;
	is_dragging_scroll_bar(game, y, scroll_bar_y);
	check_mouse_on_gear(game, x, y);
}

void	draw_stats_scroll_bar(t_game *game, t_draw_info info2, int num_players)
{
	const int	bar_width = 10;
	int			bar_height;
	const int	bar_y = info2.y + ((info2.height - bar_height)
			* game->menu->scroll) / (num_players - 27);
	t_draw_info	info;

	if (num_players <= 27)
		bar_height = info2.height;
	else
		bar_height = (info2.height * 27) / num_players;
	info = init_draw_info(info2.height, "", info2.x + game->screen_width
			* 0.6 + 5, info2.y);
	info.width = bar_width;
	info.color = 0x333333;
	draw_rectangle(game, info);
	info = init_draw_info(bar_height, "", info2.x + game->screen_width
			* 0.6 + 5, bar_y);
	info.width = bar_width;
	info.color = 0xAAAAAA;
	draw_rectangle(game, info);
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
			(game->screen_width >> 1) - game->screen_width * 0.25 / 2 + 4
			+ game->screen_width * 0.25 / 2, y + stats_height + 35
			+ game->screen_height * 0.1 / 3 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_stats_menu_bg(t_game *game, int stats_width, int stats_height)
{
	const int	x = (game->screen_width - stats_width) * 0.5;
	const int	y = (game->screen_height - stats_height) * 0.25;
	t_draw_info	stats_background;

	stats_background = init_draw_info(0, "", x, y);
	stats_background.width = stats_width;
	stats_background.height = stats_height;
	stats_background.color = MENU_BUTTON_COLOR;
	stats_background.radius = 25;
	draw_rounded_rectangle(game, stats_background);
}

void	draw_stats_menu_header(t_game *game, int stats_width, int stats_height)
{
	t_draw_info	title_info;
	const int	x = (game->screen_width - stats_width) * 0.5;
	const int	y = (game->screen_height - stats_height) * 0.25;

	title_info = init_draw_info(30, "Player Statistics",
			x + stats_width * 0.5, y + 20);
	title_info.color = 0xFFFFFF;
	draw_text(game, title_info);
	title_info = init_draw_info(20, "Name", x + 20, y + 2 * 20 + 22);
	title_info.color = 0xFFFF00;
	draw_text_left(game, title_info);
	title_info = init_draw_info(20,
			"Games      Victories      Defeats      Kills      Play Time",
			x + stats_width - 20 - 20, y + 2 * 20 + 22);
	title_info.color = 0xFFFF00;
	draw_text_right(game, title_info);
}

int	draw_stats_menu_player_scroll(t_game *game, int x, int y, int stats_height)
{
	t_player_stats	*player_stats;
	t_draw_info		player;
	const int		stats_width = game->screen_width * 0.6;
	int				num_players;
	int				i;

	player_stats = load_player_stats(game, "stats.txt", &num_players);
	if (!player_stats)
		return ;
	i = -1;
	while (++i < num_players && i < 27)
	{
		player = init_draw_info(20, "", x, y - 6);
		player.padding = 20;
		player.width = stats_width;
		draw_player_stats_row(game, player_stats[i + game->menu->scroll],
			player, i);
	}
	draw_back_button(game, x, y, stats_height);
	player = init_draw_info(stats_height - 110, "", x - 25, y + 95);
	draw_stats_scroll_bar(game, player, num_players);
	gc_free(game->mem, player_stats);
	return (num_players);
}

void	draw_stats_menu(t_game *game)
{
	const int		stats_width = game->screen_width * 0.6;
	const int		stats_height = game->screen_height * 0.6;
	const int		x = (game->screen_width - stats_width) * 0.5;
	const int		y = (game->screen_height - stats_height) * 0.25;
	int				num_players;

	game->menu->scroll_height = stats_height - 110;
	draw_stats_menu_bg(game, stats_width, stats_height);
	draw_stats_menu_header(game, stats_width, stats_height);
	draw_line(game, x + 20, y + 4 * 20, x + stats_width - 20, y + 4 * 20,
		0xFFFFFF);
	num_players = draw_stats_menu_player_scroll(game, x, y, stats_height);
	game->menu->nb_scroll = num_players;
	if (game->menu->scroll + 27 > num_players)
		game->menu->scroll = num_players - 27;
	if (num_players < 27)
		game->menu->scroll = 0;
	draw_gear_icon(game, game->screen_width - game->screen_width * 0.035 - 17,
		15, game->screen_width * 0.035);
}
