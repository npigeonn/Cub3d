/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:19:15 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 23:58:17 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_player_position(t_client *client, t_game_message msg)
{
	t_player_info	*player;

	player = find_player_by_id(client->players, msg.player_id);
	if (player)
	{
		player->x = msg.x;
		player->y = msg.y;
		player->dir_x = msg.dir_x;
		player->dir_y = msg.dir_y;
		player->health = msg.health;
		player->height = msg.height;
		player->floor = msg.floor;
	}
}

void	add_player(t_game *game, t_game_message msg)
{
	t_player_info	*new_player;
	t_player_info	*current;

	new_player = malloc(sizeof(t_player_info));
	new_player->player_id = msg.player_id;
	new_player->x = msg.x;
	new_player->y = msg.y;
	new_player->dir_x = msg.dir_x;
	new_player->dir_y = msg.dir_y;
	new_player->health = msg.health;
	new_player->height = msg.height;
	new_player->floor = msg.floor;
	new_player->next = NULL;
	ft_strlcpy(new_player->pseudo, msg.pseudo, MAX_PSEUDO_LENGTH);
	if (msg.type != MSG_GET_PLAYER)
		add_connection_msg(game, msg.pseudo);
	if (!game->client->players)
		game->client->players = new_player;
	else
	{
		current = game->client->players;
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
	t_player_info	*current;
	t_player_info	*prev;
	t_message		*new_msg;

	prev = NULL;
	current = game->client->players;
	while (current)
	{
		if (current->player_id == player_id)
		{
			new_msg = get_message_deconnection(game, current->pseudo);
			game->chatbox->messages = new_msg;
			if (prev)
				prev->next = current->next;
			else
				game->client->players = current->next;
			free(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}
