/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:03:47 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 16:03:52 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_player_node(t_server *server, int id, char *pseudo)
{
	t_player_info	*new_node;
	t_player_info	*current;

	new_node = malloc(sizeof(t_player_info));
	new_node->player_id = id;
	ft_strcpy(new_node->pseudo, pseudo);
	new_node->x = -1;
	new_node->y = -1;
	new_node->dir_x = 0;
	new_node->dir_y = 1;
	new_node->floor = 0;
	new_node->health = 100;
	new_node->height = 0;
	new_node->next = NULL;
	if (!server->players)
		server->players = new_node;
	else
	{
		current = server->players;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	update_player_node(t_server *server, char *pseudo, t_game_message msg)
{
	t_player_info	*player;

	player = find_player_by_pseudo(server, pseudo);
	if (!player)
		return ;
	player->x = msg.x;
	player->y = msg.y;
	player->dir_x = msg.dir_x;
	player->dir_y = msg.dir_y;
	player->floor = msg.floor;
	player->health = msg.health;
	player->height = msg.height;
}
