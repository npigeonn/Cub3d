/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:48:42 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 18:50:32 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	handle_mouse_chat(t_game *game, int x, int y, int keycode)
{
	const int	width = game->screen_width / 2.5;
	const int	height = game->screen_height / 3;
	const int	box_x = 50;
	const int	box_y = 150;

	if (keycode != 1)
		return ;
	if (x >= box_x && x <= box_x + width && y >= box_y
		&& y <= box_y + height + 40)
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

void	chat_input(t_game *game, int keycode)
{
	const int	len = ft_strlen(game->chatbox->message);

	if (!game->chatbox->is_writting)
		return ;
	if (keycode >= 32 && keycode <= 126)
	{
		if (len < MAX_MESSAGE_LENGTH - 1)
		{
			game->chatbox->message[len] = (char)keycode;
			game->chatbox->message[len + 1] = '\0';
		}
	}
	else if (keycode == 65288 && len > 0)
		game->chatbox->message[len - 1] = '\0';
	else if (keycode == 65293 && game->chatbox->message[0] != 0)
	{
		game->menu->status = MULTI_PLAYER;
		game->chatbox->is_writting = false;
		game->chatbox->visible = false;
		mlx_mouse_hide(game->mlx, game->win);
		add_message(game);
	}
}
