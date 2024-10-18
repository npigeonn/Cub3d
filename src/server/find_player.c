/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:37:17 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 15:47:30 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_player_info	*find_player_by_pseudo(t_server *server, char *pseudo)
{
	t_player_info	*current;

	current = server->players;
	while (current)
	{
		if (strcmp(current->pseudo, pseudo) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_player_info	*find_player_by_id(t_player_info *this_players, int id)
{
	t_player_info	*current;

	current = this_players;
	while (current)
	{
		if (current->player_id == id)
			return (current);
		current = current->next;
	}
	return (NULL);
}
