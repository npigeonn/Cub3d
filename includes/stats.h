/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:08:54 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/09 08:59:02 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATS_H
# define STATS_H

typedef struct s_player_stats
{
	char	name[50];
	int		games_played;
	int		victories;
	int		defeats;
	int		kills;
	float	play_time_hours;
}	t_player_stats;

t_player_stats	*load_player_stats(t_game *game, const char *filename,
					int *num_players);
bool			save_player_stats(t_game *game, const char *filename);
bool			parse_player_stats(FILE *file, t_game *game,
					t_player_stats **stats, int *count);

#endif