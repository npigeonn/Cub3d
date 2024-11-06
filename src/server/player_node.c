/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:03:47 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/06 12:07:51 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_player_node(t_server *server, int id, char *pseudo)
{
	t_sprite	*new_node;
	t_sprite	*current;

	new_node = gc_malloc(server->mem, sizeof(t_sprite));
	new_node->player_id = id;
	ft_strcpy(new_node->pseudo, pseudo);
	new_node->x = -1;
	new_node->y = -1;
	new_node->type = SPRITE_PLAYER;
	new_node->dir_x = 0;
	new_node->dir_y = 1;
	new_node->floor = 0;
	new_node->health = 100;
	new_node->next = NULL;
	if (!server->sprites)
		server->sprites = new_node;
	else
	{
		current = server->sprites;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	update_player_node(t_server *server, char *pseudo, t_game_message msg)
{
	t_sprite	*player;

	player = find_player_by_pseudo(server, pseudo);
	if (!player)
		return ;
	player->x = msg.x;
	player->y = msg.y;
	player->dir_x = msg.dir_x;
	player->dir_y = msg.dir_y;
	player->floor = msg.floor;
	player->health = msg.health;
}
