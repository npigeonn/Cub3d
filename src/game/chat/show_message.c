/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_message.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:44:10 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 23:52:21 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	draw_message_background(t_game *game, int height, int width,
double elapsed_time)
{
	double		alpha;
	t_draw_info	info;

	info = init_draw_info(25, "", 60, height - 5);
	alpha = 0.6 * (1 - (elapsed_time / 7.0));
	if (alpha < 0)
		alpha = 0;
	info.alpha = alpha;
	info.width = width;
	info.color = 0xc2bfba;
	draw_transparent_rectangle(game, info);
}

static void	show_message_with_pseudo(t_game *game, t_message *current,
int *height, int width)
{
	char		message[MAX_MESSAGE_LENGTH + MAX_PSEUDO_LENGTH];
	int			width_message;
	t_draw_info	info;

	ft_strlcpy(message, current->pseudo, MAX_PSEUDO_LENGTH);
	ft_strlcat(message, ": ", MAX_MESSAGE_LENGTH + MAX_PSEUDO_LENGTH);
	ft_strlcat(message, current->message,
		MAX_MESSAGE_LENGTH + MAX_PSEUDO_LENGTH);
	width_message = get_text_width(game, message, 22);
	while (width_message > width)
	{
		*height -= 19;
		width_message -= width;
	}
	info = init_draw_info(22, message, 70, *height);
	info.color = current->color;
	info.width = width;
	info.max_width = width - 40;
	draw_wrapped_text_left(game, info);
}

static void	show_message_without_pseudo(t_game *game, t_message *current,
int height)
{
	draw_text_left(game, current->message, 70, height, 22, current->color);
}

void	show_message(t_game *game)
{
	t_message	*current;
	int			height;
	int			width;
	time_t		current_time;
	double		elapsed_time;

	height = game->screen_height / 3 + 130;
	width = game->screen_width / 2.5;
	current_time = time(NULL);
	current = game->chatbox->messages;
	while (current && height >= 150)
	{
		elapsed_time = difftime(current_time, current->time.tv_sec);
		if (!(!game->chatbox->visible && elapsed_time < 7.0)
			&& !game->chatbox->visible)
			break ;
		if (current->pseudo[0] != '\0')
			show_message_with_pseudo(game, current, &height, width);
		else
			show_message_without_pseudo(game, current, height);
		if (!game->chatbox->visible && elapsed_time < 7.0)
			draw_message_background(game, height, width, elapsed_time);
		current = current->next;
		height -= 25;
	}
}
