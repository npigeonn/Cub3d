/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:50:26 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 12:50:26 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_door	*get_door(t_door *door, int x, int y, int floor)
{
	t_door	*current;

	if (!door)
		return (NULL);
	current = door;
	while (current)
	{
		if ((int)current->x == x && (int)current->y == y
			&& current->floor == floor)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	add_door(t_game *game, int x, int y, int floor)
{
	t_door	*new_door;

	new_door = gc_malloc(game->mem, sizeof(t_door));
	new_door->x = x;
	new_door->y = y;
	new_door->floor = floor;
	new_door->open = false;
	new_door->animation = 0;
	new_door->next = game->door;
	game->door = new_door;
}
