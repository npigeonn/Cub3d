/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_stats.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:28:50 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/12/07 03:36:42 by ybeaucou         ###   ########.fr       */
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

// static void	fill_player_data(t_player_stats *player, char *line)
// {
// 	sscanf(line, "%49[^,],%d,%d,%d,%d,%f",
// 		player->name,
// 		&player->games_played,
// 		&player->victories,
// 		&player->defeats,
// 		&player->kills,
// 		&player->play_time_hours);
// }

// static int	init_load_player_stats(t_game *game, FILE *file,
// const char *filename, int *num_players)
// {
// 	char	line[256];

// 	if (!file)
// 	{
// 		printf("Error opening file: %s\n", filename);
// 		return (0);
// 	}
// 	*num_players = 0;
// 	while (fgets(line, sizeof(line), file))
// 	{
// 		if (is_a_player(line))
// 			(*num_players)++;
// 	}
// 	return (1);
// }

// t_player_stats	*load_player_stats(t_game *game, const char *filename,
// int *num_players)
// {
// 	FILE			*file;
// 	t_player_stats	*stats;
// 	char			line[256];
// 	int				i;

// 	file = fopen(filename, "r");
// 	if (!init_load_player_stats(game, file, filename, num_players))
// 		return (NULL);
// 	rewind(file);
// 	stats = gc_malloc(game->mem, sizeof(t_player_stats) * (*num_players));
// 	i = 0;
// 	while (i < *num_players)
// 	{
// 		fgets(line, sizeof(line), file);
// 		if (!is_a_player(line))
// 		{
// 			i--;
// 			continue ;
// 		}
// 		fill_player_data(&stats[i], line);
// 		i++;
// 	}
// 	fclose(file);
// 	return (stats);
// }

t_player_stats*	load_player_stats(t_game *game, const char *filename, int *num_players)
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
	{
		if (is_a_player(line))
			(*num_players)++;
	}
	rewind(file);
	stats = gc_malloc(game->mem, sizeof(t_player_stats) * (*num_players));
	for (int i = 0; i < *num_players; i++)
	{
		fgets(line, sizeof(line), file);
		if (!is_a_player(line))
		{
			i--;
			continue;
		}
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
