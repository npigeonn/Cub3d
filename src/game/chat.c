/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:24:33 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/17 14:35:50 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	handle_mouse_chat(t_game *game, int x, int y, int keycode)
{
	int	width = game->screen_width / 2.5;
	int	height = game->screen_height / 3;
	int	box_x = 50;
	int	box_y = 150;
	
	if (keycode != 1)
		return ;
	if (x >= box_x && x <= box_x + width && y >= box_y && y <= box_y + height + 40)
	{
		if (y >= box_y + height)
			game->chatbox->is_writting = true;
	}
	else
	{
		game->chatbox->visible = false;
		game->chatbox->is_writting = false;
		mlx_mouse_hide(game->mlx, game->win);
		game->menu->status = MULTI_PLAYER;
	}
}

void	add_message(t_game *game)
{
	t_message	*new;
	GameMessage	chat_msg;

	new = malloc(sizeof(t_message));
	ft_strlcpy(new->message, game->chatbox->message, MAX_MESSAGE_LENGTH);
	ft_strlcpy(new->pseudo, game->server->pseudo, 20);
	gettimeofday(&new->time, NULL);
	new->color = 0x000000;
	gettimeofday(&new->time, NULL);
	new->next = game->chatbox->messages;
	game->chatbox->messages = new;
	game->chatbox->message[0] = '\0';
	chat_msg.player_id = game->server->player_id;
	ft_strlcpy(chat_msg.pseudo, game->server->pseudo, 20);
	ft_strlcpy(chat_msg.message, new->message, MAX_MESSAGE_LENGTH);
	chat_msg.type = MSG_CHAT;
	send(game->server->sock, &chat_msg, sizeof(GameMessage), 0);
}

void	chat_input(t_game *game, int keycode)
{	
	const int	len = ft_strlen(game->chatbox->message);
	
	if (!game->chatbox->is_writting)
		return;
	if (keycode >= 32 && keycode <= 126)
	{
		if (len < MAX_MESSAGE_LENGTH - 1)
		{
			game->chatbox->message[len] = (char)keycode;
			game->chatbox->message[len + 1] = '\0';
		}
	}
	else if (keycode == 65288)
	{
		if (len > 0)
			game->chatbox->message[len - 1] = '\0';
	}
	else if (keycode == 65293 && game->chatbox->message[0] != 0)
		add_message(game);
}

void	draw_text_max_right(t_game *game, char *str, int x, int y, int height, int color, int max_width)
{
	int			i;
	t_image		*img;
	int			text_width;
	int			char_width;
	int			start_idx;

	text_width = get_text_width(game, str, height);
	start_idx = 0;
	while (text_width > max_width && str[start_idx])
	{
		text_width -= get_char_width_opti(game, str[start_idx]) * height / 480 + 3;
		start_idx++;
	}
	x = x - text_width;
	y -= 7;
	i = start_idx - 1;
	while (str[++i])
	{
		if (str[i] == ' ')
			x += height * 0.33;
		else
		{
			draw_char(game, x, y, height, str[i], color);
			char_width = get_char_width_opti(game, str[i]);
			x += char_width * height / 480 + 3;
		}
	}
}

void	show_message(t_game *game)
{
	t_message	*current;
	int			height = game->screen_height / 3 + 130;

	current = game->chatbox->messages;
	while (current)
	{
		if (!current->message)
			break;
		if (height < 150)
			break;
		if (current->pseudo[0] != '\0')
		{
			draw_text_left(game, current->pseudo, 70, height, 22, current->color);
			draw_text_left(game, ": ", 70 + get_text_width(game, current->pseudo, 22), height, 22, current->color);
		}
		draw_text_left(game, current->message, 70, height, 22, current->color);
		current = current->next;
		height -= 25;
	}
}

void	chat_draw(t_game *game)
{
	int width = game->screen_width / 2.5;
	int height = game->screen_height / 3;

	int x = 50;
	int y = 150;

	float alpha = 0.6;
	int grey = 0xc2bfba;

	if (!game->chatbox->visible)
		return ;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (x + j >= 0 && x + j < game->screen_width && y + i >= 0 && y + i < game->screen_height)
			{
				int background_color = get_pixel_color_from_image(game, x + j, y + i);
				int grey_with_alpha = blend_colors(background_color, grey, alpha);
				pixel_put(game, x + j, y + i, grey_with_alpha);
			}
		}
	}
	int input_box_y = y + height;

	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (x + j >= 0 && x + j < game->screen_width && input_box_y + i >= 0 && input_box_y + i < game->screen_height)
			{
				int background_color = get_pixel_color_from_image(game, x + j, input_box_y + i);
				int grey_with_alpha = blend_colors(background_color, grey, 0.9);
				pixel_put(game, x + j, input_box_y + i, grey_with_alpha);
			}
		}
	}
	if (game->chatbox->message[0] != '\0')
		draw_text_max_right(game, game->chatbox->message, width + 40, input_box_y + 12, 22, 0x000000, width - 20);
	else
		draw_text_right(game, "Enter a message", width + 40, input_box_y + 12, 22, 0x000000);
	show_message(game);
}

