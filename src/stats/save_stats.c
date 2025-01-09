/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_stats.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:52:37 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/09 08:57:46 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	add_new_player_stats(t_game *game, FILE *file)
{
	int	victories;
	int	defeats;

	victories = 0;
	defeats = 0;
	if (game->menu->status == GAME_OVER)
	{
		victories = 0;
		defeats = 1;
	}
	else
	{
		victories = 1;
		defeats = 0;
	}
	fprintf(file, "%s,%d,%d,%d,%d,%.2f\n",
		game->client->pseudo, 1, victories, defeats,
		game->player->stats->nb_kills, 0.0);
}

static void	update_or_add_stats(t_game *game, t_player_stats *stats, int count,
FILE *file)
{
	bool	found;
	int		i;

	i = -1;
	found = false;
	while (++i < count)
	{
		if (strcmp(game->client->pseudo, stats[i].name) == 0)
		{
			stats[i].games_played++;
			if (game->menu->status == GAME_OVER)
				stats[i].defeats++;
			else
				stats[i].victories++;
			stats[i].kills += game->player->stats->nb_kills;
			found = true;
			break ;
		}
	}
	if (!found)
		add_new_player_stats(game, file);
}

static bool	open_file_and_parse(const char *filename, t_game *game,
t_player_stats **stats, int *count)
{
	FILE	*file;

	file = fopen(filename, "r");
	if (file)
	{
		if (!parse_player_stats(file, game, stats, count))
		{
			fclose(file);
			return (false);
		}
		fclose(file);
	}
	else
		*count = 0;
	return (true);
}

static bool	write_updated_stats(const char *filename, t_game *game,
t_player_stats *stats, int count)
{
	FILE	*file;
	int		i;

	file = fopen(filename, "w");
	if (!file)
		return (false);
	update_or_add_stats(game, stats, count, file);
	i = -1;
	while (++i < count)
		fprintf(file, "%s,%d,%d,%d,%d,%.2f\n",
			stats[i].name, stats[i].games_played, stats[i].victories,
			stats[i].defeats, stats[i].kills, stats[i].play_time_hours);
	fclose(file);
	return (true);
}

bool	save_player_stats(t_game *game, const char *filename)
{
	t_player_stats	*stats;
	int				count;

	stats = NULL;
	if (!open_file_and_parse(filename, game, &stats, &count))
		return (false);
	if (!write_updated_stats(filename, game, stats, count))
	{
		gc_free(game->mem, stats);
		return (false);
	}
	gc_free(game->mem, stats);
	return (true);
}
