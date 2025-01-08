/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_field.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:33:18 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 12:55:11 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_text_field(t_game *game, t_draw_info info, char *text)
{
	info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, info);
	if (text)
	{
		info = init_draw_info(info.height * 0.5, text, info.x + 10,
				info.y + info.height / 2 - 10);
		info.color = MENU_BUTTON_TEXT_COLOR;
		ft_strlcpy(info.str, text, MAX_MESSAGE_LENGTH);
		draw_text_left(game, info);
	}
}

void	draw_labeled_text_field(t_game *game, char *content, t_draw_info info2)
{
	t_draw_info	info;

	info = init_draw_info(info2.height * 0.5, info2.str, game->screen_width / 2,
			info2.y - 45);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
	info = init_draw_info(info2.height, "", info2.x, info2.y);
	info.width = info2.width;
	draw_text_field(game, info, content);
	gc_free(game->mem, content);
}
