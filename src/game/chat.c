/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:24:33 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/17 15:52:27 by ybeaucou         ###   ########.fr       */
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
	{
		game->menu->status = MULTI_PLAYER;
		game->chatbox->is_writting = false;
		game->chatbox->visible = false;
		mlx_mouse_hide(game->mlx, game->win);
		add_message(game);
	}
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

void	draw_wrapped_text_left(t_game *game, char *str, int x, int y, int height, int color, int max_width)
{
	int		i = 0;
	int		line_start = 0;
	int		current_width = 0;
	char	temp[1024];
	int		char_width;
	float	line_spacing = 0.8;

	while (str[i])
	{
		char_width = (str[i] == ' ') ? height * 0.33 : get_char_width_opti(game, str[i]) * height / 480 + 3;
		if (current_width + char_width > max_width || str[i] == '\n')
		{
			ft_strlcpy(temp, &str[line_start], i - line_start + 1);
			temp[i - line_start] = '\0';
			draw_text_left(game, temp, x, y, height, color);
			y += height * line_spacing;
			line_start = (str[i] == '\n') ? i + 1 : i;
			current_width = 0;
		}
		else
		{
			current_width += char_width;
			i++;
		}
	}
	if (line_start < i)
	{
		ft_strlcpy(temp, &str[line_start], i - line_start + 1);
		temp[i - line_start] = '\0';
		draw_text_left(game, temp, x, y, height, color);
	}
}

void	draw_transparent_rectangle(t_game *game, int x, int y, int width, int height, int color, float alpha)
{
	int		i;
	int		j;
	int		background_color;
	int		grey_with_alpha;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (x + j >= 0 && x + j < game->screen_width && y + i >= 0 && y + i < game->screen_height)
			{
				background_color = get_pixel_color_from_image(game, x + j, y + i);
				grey_with_alpha = blend_colors(background_color, color, alpha);
				pixel_put(game, x + j, y + i, grey_with_alpha);
			}
		}
	}
}

void	show_message(t_game *game)
{
	t_message	*current;
	int			height = game->screen_height / 3 + 130;
	int			width = game->screen_width / 2.5;
	time_t		current_time = time(NULL);

	current = game->chatbox->messages;
	while (current)
	{
		if (!current->message || height < 150)
			break;
		double elapsed_time = difftime(current_time, current->time.tv_sec);
		int show_background = (!game->chatbox->visible && elapsed_time < 7.0);

		if (!show_background && !game->chatbox->visible)
			break;

		if (current->pseudo[0] != '\0')
		{
			char	message[MAX_MESSAGE_LENGTH + MAX_PSEUDO_LENGTH];
			ft_strlcpy(message, current->pseudo, MAX_PSEUDO_LENGTH);
			ft_strlcat(message, ": ", MAX_MESSAGE_LENGTH + MAX_PSEUDO_LENGTH);
			ft_strlcat(message, current->message, MAX_MESSAGE_LENGTH + MAX_PSEUDO_LENGTH);
			int	width_message = get_text_width(game, message, 22);

			while (width_message > width)
			{
				height -= 19;
				width_message -= width;
			}
			if (show_background)
			{
				double alpha = 0.6 * (1 - (elapsed_time / 7.0));
				if (alpha < 0) alpha = 0; 
				draw_transparent_rectangle(game, 60, height - 5, width, 25, 0xc2bfba, alpha);
			}
			draw_wrapped_text_left(game, message, 70 , height, 22, current->color, width - 20);
		}
		else
		{
			if (show_background)
			{
				double alpha = 0.6 * (1 - (elapsed_time / 7.0));
				if (alpha < 0) alpha = 0; 
				draw_transparent_rectangle(game, 60, height - 5, width, 25, 0xc2bfba, alpha);
			}
			draw_text_left(game, current->message, 70, height, 22, current->color);
		}

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

	draw_transparent_rectangle(game, x, y, width, height, grey, alpha);
	int input_box_y = y + height;
	draw_transparent_rectangle(game, x, input_box_y, width, 40, grey, 0.9);
	if (game->chatbox->message[0] != '\0')
		draw_text_max_right(game, game->chatbox->message, width + 40, input_box_y + 12, 22, 0x000000, width - 20);
	else
		draw_text_right(game, "Enter a message", width + 40, input_box_y + 12, 22, 0x000000);
}

