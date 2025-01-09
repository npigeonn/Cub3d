/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_stats2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 08:55:46 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/09 08:57:53 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	parse_player_stats(FILE *file, t_game *game, t_player_stats **stats,
int *count)
{
	char	line[256];
	int		i;

	i = 0;
	*count = 0;
	while (fgets(line, sizeof(line), file))
		if (is_a_player(line))
			(*count)++;
	rewind(file);
	*stats = gc_malloc(game->mem, sizeof(t_player_stats) * (*count));
	if (!*stats)
		return (false);
	while (i < *count && fgets(line, sizeof(line), file))
	{
		if (!is_a_player(line))
			continue ;
		sscanf(line, "%[^,],%d,%d,%d,%d,%f",
			(*stats)[i].name, &(*stats)[i].games_played,
			&(*stats)[i].victories, &(*stats)[i].defeats,
			&(*stats)[i].kills, &(*stats)[i].play_time_hours);
		i++;
	}
	return (true);
}
