/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_stats.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:45:43 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 03:26:10 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	save_player_stats(t_game *game, const char *filename)
{
	FILE *file;
	t_player_stats *existing_stats = NULL;
	t_player_stats	new_stats;
	int num_existing_stats = 0;
	int i, j;
	bool found;

	file = fopen(filename, "r");
	if (file)
	{
		char line[256];
		while (fgets(line, sizeof(line), file))
		{
			if (is_a_player(line))
				num_existing_stats++;
		}
		rewind(file);
		existing_stats = malloc(sizeof(t_player_stats) * num_existing_stats);
		for (i = 0; i < num_existing_stats; i++)
		{
			fgets(line, sizeof(line), file);
			if (!is_a_player(line))
			{
				i--;
				continue;
			}
			sscanf(line, "%[^,],%d,%d,%d,%d,%f",
				   existing_stats[i].name, &existing_stats[i].games_played,
				   &existing_stats[i].victories, &existing_stats[i].defeats,
				   &existing_stats[i].kills, &existing_stats[i].play_time_hours);
		}
		fclose(file);
	}
	else
		num_existing_stats = 0;
	found = false;
	for (j = 0; j < num_existing_stats; j++)
	{
		if (strcmp(game->client->pseudo, existing_stats[j].name) == 0)
		{
			existing_stats[j].games_played += 1;
			if (game->menu->status == GAME_OVER)
				existing_stats[j].defeats += 1;
			else
				existing_stats[j].victories += 1;
			existing_stats[j].kills += game->player->stats->nb_kills;
			found = true;
			break;
		}
	}
	file = fopen(filename, "w");
	if (!file)
	{
		free(existing_stats);
		return false;
	}
	if (!found)
	{
		fprintf(file, "%s,%d,%d,%d,%d,%.2f\n",
				game->client->pseudo, 1,
				game->menu->status == GAME_OVER ? 0 : 1,
				game->menu->status == GAME_OVER ? 1 : 0,
				game->player->stats->nb_kills, 0.0);
	}
	for (i = 0; i < num_existing_stats; i++)
	{
		fprintf(file, "%s,%d,%d,%d,%d,%.2f\n",
				existing_stats[i].name, existing_stats[i].games_played,
				existing_stats[i].victories, existing_stats[i].defeats,
				existing_stats[i].kills, existing_stats[i].play_time_hours);
	}
	fclose(file);
	free(existing_stats);
	return true;
}
