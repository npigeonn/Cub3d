/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:19:15 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/05 12:13:45 by ybeaucou         ###   ########.fr       */
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
	}
}

void	add_player(t_game *game, t_game_message msg)
{
	t_sprite	*new_player;
	t_sprite	*current;

	new_player = malloc(sizeof(t_sprite));
	new_player->player_id = msg.player_id;
	new_player->x = msg.x;
	new_player->y = msg.y;
	new_player->dir_x = msg.dir_x;
	new_player->dir_y = msg.dir_y;
	new_player->health = msg.health;
	new_player->floor = msg.floor;
	new_player->next = NULL;
	new_player->type = SPRITE_PLAYER;
	ft_strlcpy(new_player->pseudo, msg.pseudo, MAX_PSEUDO_LENGTH);
	if (msg.type != MSG_GET_PLAYER)
		add_connection_msg(game, msg.pseudo);
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

static t_message	*get_message_deconnection(t_game *game, char *pseudo)
{
	t_message	*new_msg;

	new_msg = malloc(sizeof(t_message));
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
			free(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}
