/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_stats.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:28:50 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 18:43:41 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

bool	is_a_player(char *line)
{
	int	i;
	int	has_decimal;
	int	j;

	i = 0;
	while (ft_isalnum(line[i]))
		i++;
	if (line[i] != ',')
		return (false);
	j = -1;
	while (++j < 5)
	{
		has_decimal = 0;
		while (ft_isdigit(line[++i]) || (line[i] == '.' && !has_decimal))
			if (line[i] == '.')
				has_decimal = 1;
		if (j < 4)
			if (line[i] != ',')
				return (false);
		i++;
	}
	if (line[i] != '\0')
		return (false);
	return (true);
}

bool	check_condition(FILE *file, int *num_players)
{
	char	line[256];

	if (!file)
		return (false);
	*num_players = 0;
	while (fgets(line, sizeof(line), file))
	{
		if (is_a_player(line))
			(*num_players)++;
	}
	return (true);
}

t_player_stats	*load_player_stats(t_game *game, const char *filename,
int *num_players)
{
	FILE			*file;
	t_player_stats	*stats;
	char			line[256];
	int				i;

	file = fopen(filename, "r");
	if (!check_condition(file, num_players))
		return (NULL);
	rewind(file);
	stats = gc_malloc(game->mem, sizeof(t_player_stats) * (*num_players));
	i = -1;
	while (++i < *num_players)
	{
		fgets(line, sizeof(line), file);
		if (!is_a_player(line))
		{
			i--;
			continue ;
		}
		sscanf(line, "%[^,],%d,%d,%d,%d,%f", stats[i].name, &stats[i].games_played,
			&stats[i].victories, &stats[i].defeats, &stats[i].kills,
			&stats[i].play_time_hours);
	}
	fclose(file);
	return (stats);
}
