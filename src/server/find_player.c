/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:37:17 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/14 11:26:01 by ybeaucou         ###   ########.fr       */
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

static t_sprite	*init_get_player(t_server *server, float *dx, float *dy,
t_sprite *current)
{
	t_sprite	*copy;

	*dx = current->x - server->current_enemy->x;
	*dy = current->y - server->current_enemy->y;
	copy = gc_malloc(server->mem, sizeof(t_sprite));
	ft_memcpy(copy, current, sizeof(t_sprite));
	return (copy);
}

t_sprite	*get_target_player(t_server *server, float *distance,
t_sprite *current_enemy)
{
	t_sprite	*current;
	t_sprite	*copy;
	float		dx;
	float		dy;

	current = server->sprites;
	while (current)
	{
		if (current->type != SPRITE_PLAYER || current->player_id < 0
			|| current->floor != current_enemy->floor)
		{
			current = current->next;
			continue ;
		}
		copy = init_get_player(server, &dx, &dy, current);
		if (sqrt(dx * dx + dy * dy) < *distance && sqrt(dx * dx + dy * dy) < 10
			&& has_line_of_sight_server(server, copy, current))
		{
			*distance = sqrt(dx * dx + dy * dy);
			return (gc_free(server->mem, copy), current);
		}
		current = current->next;
		gc_free(server->mem, copy);
	}
	return (NULL);
}
