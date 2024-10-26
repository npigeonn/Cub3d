
#ifndef STATS_H
# define STATS_H

typedef struct	s_player_stats
{
	char	name[50];
	int		games_played;
	int		victories;
	int		defeats;
	int		kills;
	float	play_time_hours;
}	t_player_stats;

t_player_stats*	load_player_stats(const char *filename, int *num_players);

#endif