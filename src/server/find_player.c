/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:37:17 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/29 17:36:47 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_sprite	*find_player_by_pseudo(t_server *server, char *pseudo)
{
	t_sprite	*current;

	current = server->sprites;
	while (current)
	{
		if (current->type == SPRITE_PLAYER
			&& ft_strcmp(current->pseudo, pseudo) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_sprite	*find_player_by_id(t_sprite *this_players, int id)
{
	t_sprite	*current;

	current = this_players;
	while (current)
	{
		if (current->type == SPRITE_PLAYER && current->player_id == id)
			return (current);
		current = current->next;
	}
	return (NULL);
}
