/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:24:33 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/12/10 12:27:08 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_draw_info	init_draw_info(int height, char *str, int x, int y)
{
	t_draw_info	draw_info;

	draw_info.width = 0;
	draw_info.height = height;
	ft_strcpy(draw_info.str, str);
	draw_info.color = 0x00000;
	draw_info.x = x;
	draw_info.y = y;
	draw_info.max_width = 0;
	draw_info.alpha = 0.5;
	return (draw_info);
}

void	add_message(t_game *game)
{
	t_message		*new;
	t_game_message	chat_msg;

	new = gc_malloc(game->mem, sizeof(t_message));
	ft_bzero(&chat_msg, sizeof(t_game_message));
	ft_strlcpy(new->message, game->chatbox->message, MAX_MESSAGE_LENGTH);
	ft_strlcpy(new->pseudo, game->client->pseudo, 20);
	gettimeofday(&new->time, NULL);
	new->color = 0x000000;
	gettimeofday(&new->time, NULL);
	new->next = game->chatbox->messages;
	game->chatbox->messages = new;
	game->chatbox->message[0] = '\0';
	chat_msg.player_id = game->client->player_id;
	ft_strlcpy(chat_msg.pseudo, game->client->pseudo, 20);
	ft_strlcpy(chat_msg.message, new->message, MAX_MESSAGE_LENGTH);
	chat_msg.type = MSG_CHAT;
	send(game->client->sock, &chat_msg, sizeof(t_game_message), 0);
}

static t_draw_info	get_default_draw_info(int height, int width, int x, int y)
{
	t_draw_info	info;

	info = init_draw_info(22, "Enter a message", x, y);
	info.alpha = 0.6;
	info.color = 0xc2bfba;
	info.width = width;
	info.height = height;
	return (info);
}

void	draw_basic_message(t_game *game, t_draw_info info)
{
	info.x += 10;
	info.y += 10;
	info.color = 0x000000;
	info.height = 22;
	ft_strcpy(info.str, "Enter a message");
	draw_text_max_right(game, info);
}

void	chat_draw(t_game *game)
{
	const int	width = game->screen_width / 2.5;
	const int	height = game->screen_height / 3;
	const int	x = 50;
	const int	y = 150;
	t_draw_info	info;

	if (!game->chatbox->visible)
		return ;
	info = get_default_draw_info(height, width, x, y);
	draw_transparent_rectangle(game, info);
	info.y = y + height;
	info.height = 40;
	info.alpha = 0.9;
	draw_transparent_rectangle(game, info);
	ft_strcpy(info.str, game->chatbox->message);
	info.x = width + 20;
	info.y = y + height + 14;
	info.color = 0x000000;
	info.max_width = width - 20;
	info.height = 22;
	if (game->chatbox->message[0] != '\0')
		draw_text_max_right(game, info);
	else
		draw_basic_message(game, info);
}
