/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:58:50 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:51:55 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

float	distance_squared(float x1, float y1, float x2, float y2)
{
	const float	dx = x2 - x1;
	const float	dy = y2 - y1;

	return (dx * dx + dy * dy);
}

char	*existing_player(t_server *server, int new_socket)
{
	char			pseudo[MAX_PSEUDO_LENGTH];
	t_sprite		*player;

	pseudo[0] = '\0';
	if (new_socket < 0)
		return (gc_strdup(server->mem, pseudo));
	recv(new_socket, pseudo, sizeof(pseudo), 0);
	pthread_mutex_lock(server->game_lock);
	player = find_player_by_pseudo(server, pseudo);
	if (player)
	{
		if (player->player_id >= 0
			&& server->client_sockets[player->player_id] >= 0)
		{
			close(new_socket);
			pthread_mutex_unlock(server->game_lock);
			return (gc_strdup(server->mem, pseudo));
		}
		else if (reconnect_player(server, player, new_socket))
			return (NULL);
	}
	pthread_mutex_unlock(server->game_lock);
	return (gc_strdup(server->mem, pseudo));
}
