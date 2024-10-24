/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:42:41 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/24 15:54:28 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct	s_player_stats
{
	char 	name[50];
	int		games_played;
	int		victories;
	int		defeats;
	int		kills;
	float	play_time_hours;
}	t_player_stats;

t_player_stats* load_player_stats(const char *filename, int *num_players)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void draw_stats_menu(t_game *game)
{
	t_player_stats *player_stats;
	const int stats_width = game->screen_width * 0.6;
	const int stats_height = game->screen_height * 0.5;
	const int x = (game->screen_width - stats_width) * 0.5;
	const int y = (game->screen_height - stats_height) * 0.25;
	const int padding = 20;
	const int row_height = 30;

	t_draw_info info;
	char buffer[64];

	int num_players;
	player_stats = load_player_stats("stats.txt", &num_players);
	if (!player_stats)
		return;

	t_draw_info stats_background = init_draw_info(0, "", x, y);
	stats_background.width = stats_width;
	stats_background.height = stats_height;
	stats_background.color = 0x333333;
	draw_rectangle(game, stats_background);

	info = init_draw_info(30, "Player Statistics", x + stats_width * 0.5, y + padding);
	info.color = 0xFFFFFF;
	draw_text(game, info);

	snprintf(buffer, sizeof(buffer), "Name        Games   Victories  Defeats  Kills   Play Time");
	info = init_draw_info(20, buffer, x + padding, y + 2 * padding + 22);
	info.color = 0xFFFF00;
	draw_text_left(game, info);

	int separator_y = y + 3 * padding + 20;
	draw_line(game, x + padding, separator_y, x + stats_width - padding, separator_y, 0xFFFFFF);

	for (int i = 0; i < num_players; i++)
	{
		int row_color = (i % 2 == 0) ? 0x444444 : 0x555555;
		t_draw_info player_row_background = init_draw_info(0, "", x + padding, y + (4 + i) * padding + 20);
		player_row_background.width = stats_width - 2 * padding;
		player_row_background.height = row_height;
		player_row_background.color = row_color;
		draw_rectangle(game, player_row_background);

		snprintf(buffer, sizeof(buffer), "%-10s %5d   %5d       %5d     %5d   %.2f",
				 player_stats[i].name,
				 player_stats[i].games_played,
				 player_stats[i].victories,
				 player_stats[i].defeats,
				 player_stats[i].kills,
				 player_stats[i].play_time_hours);
		info = init_draw_info(20, buffer, x + padding, y + (4 + i) * padding + 20);
		info.color = 0xFFFFFF;
		draw_text_left(game, info);
	}

	// Clean up
	free(player_stats);
}

