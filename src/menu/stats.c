/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:50:42 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/25 22:12:35 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

typedef struct	s_player_stats
{
	char	name[50];
	int		games_played;
	int		victories;
	int		defeats;
	int		kills;
	float	play_time_hours;
}	t_player_stats;

void	update_stats_menu_click(t_game *game, int x, int y, int keycode)
{
	if (keycode == 1 && game->menu->button_selected == 1)
	{
		game->menu->status = MAIN_MENU;
		game->menu->button_selected = 0;
		game->player->scroll = 0;
	}
	if (keycode == 5)
		game->player->scroll += 1;
	if (keycode == 4)
	{
		game->player->scroll -= 1;
		if (game->player->scroll < 0)
			game->player->scroll = 0;
	}
}

void	update_stats_menu(t_game *game, int x, int y)
{
	const int	button_x = (game->screen_width - game->screen_width * 0.25) / 2;
	const int	button_y = game->screen_height * 0.6;
	const int	button_width = game->screen_width * 0.25;
	const int	button_height = game->screen_height * 0.1;

	game->menu->button_selected = 0;
	if (x >= button_x && x <= button_x + button_width
		&& y >= button_y && y <= button_y + button_height)
		game->menu->button_selected = 1;
}

t_player_stats*	load_player_stats(const char *filename, int *num_players)
{
	FILE			*file;
	t_player_stats	*stats;
	char			line[256];
	
	file = fopen(filename, "r");
	if (!file)
	{
		printf("Error opening file: %s\n", filename);
		return NULL;
	}
	*num_players = 0;
	while (fgets(line, sizeof(line), file))
		(*num_players)++;
	rewind(file);
	stats = malloc(sizeof(t_player_stats) * (*num_players));
	for (int i = 0; i < *num_players; i++)
	{
		fgets(line, sizeof(line), file);
		sscanf(line, "%49[^,],%d,%d,%d,%d,%f",
				stats[i].name,
				&stats[i].games_played,
				&stats[i].victories,
				&stats[i].defeats,
				&stats[i].kills,
				&stats[i].play_time_hours);
	}
	fclose(file);
	return (stats);
}

void draw_stats_scroll_bar(t_game *game, int x, int y, int stats_height, int num_players)
{
	int	bar_width = 10;
	int	max_visible_players = 27;
	int	bar_height;

	if (num_players <= max_visible_players)
		bar_height = stats_height;
	else
		bar_height = (stats_height * max_visible_players) / num_players;

	int	bar_y = y + ((stats_height - bar_height) * game->player->scroll) / (num_players - max_visible_players);
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
	if (game->player->scroll + 27 > num_players)
		game->player->scroll = num_players - 27;
	if (num_players < 27)
		game->player->scroll = 0;
	for (int i = 0; i < num_players && i < 27; i++)
		draw_player_stats_row(game, player_stats[i + game->player->scroll], x, y - 6, padding, row_height, i, stats_width);
	draw_back_button(game, x, y, stats_height);
	draw_stats_scroll_bar(game, x - 25, y + 95, stats_height - 110, num_players);
	free(player_stats);
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

void	draw_stat_info(t_game *game, char *stat, int x, int y)
{
	t_draw_info stat_info = init_draw_info(20, stat, x, y);
	stat_info.color = 0xFFFFFF;
	draw_text_right(game, stat_info);
}

void	draw_back_button(t_game *game, int x, int y, int stats_height)
{
	int button_x = (game->screen_width - game->screen_width * 0.25) / 2;
	int button_y = y + stats_height + 35;

	t_draw_info button_info = init_draw_info(game->screen_height * 0.1, "", button_x, button_y);
	button_info.width = game->screen_width * 0.25;
	button_info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, button_info);
	t_draw_info back_text_info = init_draw_info(game->screen_height * 0.1 * 0.5, "Back", (game->screen_width >> 1) - game->screen_width * 0.25 / 2 + 4 + game->screen_width * 0.25 / 2, y + stats_height + 35 + game->screen_height * 0.1 / 3 - 5);
	back_text_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, back_text_info);
}
