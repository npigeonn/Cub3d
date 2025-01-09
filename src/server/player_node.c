/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:03:47 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/09 10:00:57 by ybeaucou         ###   ########.fr       */
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
	new_node->ammo = 30;
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

void	send_msg_destroy_sprite(t_server *server, t_sprite *current,
t_sprite *p)
{
	t_game_message	msg;

	current->still_exist = 0;
	msg.type = MSG_DESTROY_SPRITE;
	msg.x = current->x;
	msg.y = current->y;
	msg.floor = current->floor;
	msg.player_id = p->player_id;
	msg.health = p->health;
	msg.ammo = p->ammo;
	notify_all_players(server, msg);
}

void	detect_amo_and_health(t_server *server, t_sprite *p)
{
	t_sprite		*current;

	current = server->sprites;
	while (current)
	{
		if (current->type == SPRITE_AMMO && (int)p->x == (int)current->x
			&& (int)p->y == (int)current->y && p->floor == current->floor
			&& current->still_exist == 1)
		{
			p->ammo += 25;
			send_msg_destroy_sprite(server, current, p);
		}
		if (current->type == SPRITE_HEALTH && (int)p->x == (int)current->x
			&& (int)p->y == (int)current->y && p->floor == current->floor
			&& current->still_exist == 1)
		{
			p->health += 25;
			if (p->health > 1)
				p->health = 1;
			send_msg_destroy_sprite(server, current, p);
		}
		current = current->next;
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
	player->plane_x = msg.plane_x;
	player->plane_y = msg.plane_y;
	player->selected_anim = msg.selected_anim;
	detect_amo_and_health(server, player);
}
