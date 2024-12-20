/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:19:15 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/18 16:36:40 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_player_position(t_game *game, t_game_message msg)
{
	t_sprite	*player;

	player = find_player_by_id(game->sprites, msg.player_id);
	if (player)
	{
		player->x = msg.x;
		player->y = msg.y;
		player->dir_x = msg.dir_x;
		player->dir_y = msg.dir_y;
		player->health = msg.health;
		player->floor = msg.floor;
		player->plane_x = msg.plane_x;
		player->plane_y = msg.plane_y;
	}
}

void	add_player_to_node(t_game *game, t_sprite *new_player)
{
	t_sprite	*current;

	if (!game->sprites)
		game->sprites = new_player;
	else
	{
		current = game->sprites;
		while (current->next)
			current = current->next;
		current->next = new_player;
	}
}

void	add_player(t_game *game, t_game_message msg)
{
	t_sprite	*new_player;

	new_player = gc_malloc(game->mem, sizeof(t_sprite));
	ft_bzero(new_player, sizeof(t_sprite));
	new_player->player_id = msg.player_id;
	new_player->x = msg.x;
	new_player->y = msg.y;
	new_player->dir_x = msg.dir_x;
	new_player->dir_y = msg.dir_y;
	new_player->health = msg.health;
	new_player->floor = msg.floor;
	new_player->next = NULL;
	new_player->type = SPRITE_PLAYER;
	new_player->dir = 0;
	new_player->scale = 1;
	new_player->z_offset = 0;
	new_player->spritecast = gc_malloc(game->mem, sizeof(t_spritecast));
	ft_strlcpy(new_player->pseudo, msg.pseudo, MAX_PSEUDO_LENGTH);
	add_connection_msg(game, msg.pseudo, msg);
	add_player_to_node(game, new_player);
}

static t_message	*get_message_deconnection(t_game *game, char *pseudo)
{
	t_message	*new_msg;

	new_msg = gc_malloc(game->mem, sizeof(t_message));
	ft_strlcpy(new_msg->message, pseudo, MAX_PSEUDO_LENGTH);
	ft_strlcat(new_msg->message + ft_strlen(pseudo), " left the game.",
		MAX_MESSAGE_LENGTH);
	new_msg->pseudo[0] = '\0';
	new_msg->next = game->chatbox->messages;
	new_msg->color = 0xFF4500;
	gettimeofday(&new_msg->time, NULL);
	return (new_msg);
}

void	remove_player(t_game *game, int player_id)
{
	t_sprite	*current;
	t_sprite	*prev;
	t_message	*new_msg;

	prev = NULL;
	current = game->sprites;
	while (current)
	{
		if (current->type == SPRITE_PLAYER && current->player_id == player_id)
		{
			new_msg = get_message_deconnection(game, current->pseudo);
			game->chatbox->messages = new_msg;
			if (prev)
				prev->next = current->next;
			else
				game->sprites = current->next;
			gc_free(game->mem, current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}
