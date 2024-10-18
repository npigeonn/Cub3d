/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:32:11 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 18:34:50 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_door(t_game *game, t_game_message msg)
{
	t_door	*current;

	current = game->door;
	while (current)
	{
		if (current->x == msg.x && current->y == msg.y
			&& current->floor == msg.floor)
		{
			current->open = msg.open;
			break ;
		}
		current = current->next;
	}
}
