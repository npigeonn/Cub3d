/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scroll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:27:17 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:40:14 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	is_dragging_scroll_bar(t_game *game, int y, int scroll_bar_y)
{
	const int	delta_y = y - scroll_bar_y;
	const int	max_scroll_position = game->menu->scroll_height
		- game->menu->nb_scroll;

	if (game->menu->dragging)
	{
		game->menu->scroll = ((float)delta_y / (game->menu->scroll_height - 27))
			* max_scroll_position;
		if (game->menu->scroll < 0)
			game->menu->scroll = 0;
		else if (game->menu->scroll > max_scroll_position)
			game->menu->scroll = max_scroll_position;
	}
}

void	draw_stats_scroll_bar(t_game *game, t_draw_info info2, int num_players)
{
	const int	bar_width = 10;
	int			bar_height;
	int			bar_y;
	t_draw_info	info;

	if (num_players <= 27)
		bar_height = info2.height;
	else
		bar_height = (info2.height * 27) / num_players;
	bar_y = info2.y + ((info2.height - bar_height)
			* game->menu->scroll) / (num_players - 27);
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

void	check_scroll_position(t_game *game, int num_players)
{
	game->menu->nb_scroll = num_players;
	if (game->menu->scroll + 27 > num_players)
		game->menu->scroll = num_players - 27;
	if (num_players < 27)
		game->menu->scroll = 0;
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
		return (-1);
	i = -1;
	check_scroll_position(game, num_players);
	while (++i < num_players && i < 27)
	{
		player = init_draw_info(20, "", x, y - 6);
		player.padding = 20;
		player.width = stats_width;
		draw_player_stats_row(game, player_stats[i + game->menu->scroll],
			player, i);
	}
	draw_back_stats_button(game, y, stats_height);
	player = init_draw_info(stats_height - 110, "", x - 25, y + 95);
	draw_stats_scroll_bar(game, player, num_players);
	gc_free(game->mem, player_stats);
	return (num_players);
}
