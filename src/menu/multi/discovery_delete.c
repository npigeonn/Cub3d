/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   discovery_delete.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:50:33 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 12:47:49 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	delete_server(t_game *game, t_server_info *prev,
t_server_info *current)
{
	t_server_info	*to_delete;

	to_delete = current;
	if (prev)
		prev->next = current->next;
	else
		game->servers = current->next;
	gc_free(game->mem, to_delete->name);
	gc_free(game->mem, to_delete->ip);
	gc_free(game->mem, to_delete);
}

void	purge_old_servers(t_game *game, time_t now)
{
	t_server_info	*current;
	t_server_info	*prev;

	prev = NULL;
	pthread_mutex_lock(&game->mutex);
	current = game->servers;
	while (current)
	{
		if (difftime(now, current->last_seen) > 10)
			delete_server(game, prev, current);
		else
			prev = current;
		current = current->next;
	}
	pthread_mutex_unlock(&game->mutex);
}
