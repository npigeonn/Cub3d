/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:51:08 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/14 13:22:37 by ybeaucou         ###   ########.fr       */
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

void	draw_text_max_right(t_game *game, t_draw_info info)
{
	int			i;
	t_image		*img;
	int			text_width;
	int			index;

	text_width = get_text_width(game, info.str, info.height);
	index = -1;
	while (text_width > info.max_width && info.str[++index])
		text_width -= get_char_width_opti(game, info.str[index])
			* info.height / 480 + 3;
	info.x = info.x - text_width;
	info.y -= 7;
	i = index;
	while (info.str[++i])
	{
		if (info.str[i] == ' ')
			info.x += info.height * 0.33;
		else
		{
			info.c = info.str[i];
			draw_char(game, info);
			info.x += get_char_width_opti(game, info.str[i])
				* info.height / 480 + 3;
		}
	}
}

static int	draw_wrapped_text_left2(t_game *game, t_draw_info info, int i,
int *current_width)
{
	int	char_width;

	if (info.str[i] == ' ')
		char_width = info.height * 0.33;
	else
		char_width = get_char_width_opti(game, info.str[i])
			* info.height / 480 + 3;
	if (*current_width + char_width > info.max_width)
		return (0);
	*current_width += char_width;
	return (1);
}

void	draw_wrapped_text_left_loop(t_game *game, int i, int line_length,
t_draw_info info)
{
	t_draw_info	info2;
	char		temp[1024];

	ft_strlcpy(temp, &info.str[i - line_length], line_length + 1);
	temp[line_length] = '\0';
	info2 = init_draw_info(info.height, temp, info.x, info.y);
	info2.color = info.color;
	info2.max_width = info.max_width;
	draw_text_left(game, info2);
	info.y += info.height * 0.8;
}

void	draw_wrapped_text_left(t_game *game, t_draw_info info)
{
	int			i;
	int			current_width;
	int			line_length;

	i = 0;
	current_width = 0;
	while (info.str[i])
	{
		line_length = 0;
		while (info.str[i])
		{
			if (!draw_wrapped_text_left2(game, info, i, &current_width))
				break ;
			line_length++;
			i++;
		}
		draw_wrapped_text_left_loop(game, i, line_length, info);
		current_width = 0;
	}
	if (!info.str[i])
		return ;
	draw_text_left(game, info);
}

void	draw_transparent_rectangle(t_game *game, t_draw_info info)
{
	int		i;
	int		j;
	int		background_color;
	int		grey_with_alpha;

	i = -1;
	while (++i < info.height)
	{
		j = -1;
		while (++j < info.width)
		{
			if (info.x + j >= 0 && info.x + j < game->screen_width
				&& info.y + i >= 0 && info.y + i < game->screen_height)
			{
				background_color = get_pixel_color_from_image(game, info.x + j,
						info.y + i);
				grey_with_alpha = blend_colors(background_color, info.color,
						info.alpha);
				pixel_put(game, info.x + j, info.y + i, grey_with_alpha);
			}
		}
	}
}
