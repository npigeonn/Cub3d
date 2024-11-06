/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:49:39 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/06 12:08:33 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_game_message_queue	*create_game_message_queue(t_server *server, t_game_message msg)
{
	t_game_message_queue	*new_node;

	new_node = gc_malloc(server->mem, sizeof(t_game_message_queue));
	new_node->message = msg;
	new_node->next = NULL;
	return (new_node);
}

void	add_game_message_to_queue(t_server *server, t_game_message msg)
{
	t_game_message_queue	*current;

	if (!server->game_queue)
		server->game_queue = create_game_message_queue(server, msg);
	else
	{
		current = server->game_queue;
		while (current->next)
			current = current->next;
		current->next = create_game_message_queue(server, msg);
	}
}
