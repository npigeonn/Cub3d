/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:02:14 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 13:02:14 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_text_width(t_game *game, char *str, int height)
{
	int	total_width;
	int	i;
	int	char_width;

	total_width = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == ' ')
			total_width += height * 0.33;
		else if (str[i] != '\n')
		{
			char_width = game->menu->width_letter[get_index_char(str[i])];
			total_width += (char_width * height / 480) + 3;
		}
	}
	return (total_width);
}

void	draw_text(t_game *game, t_draw_info info)
{
	int			i;
	t_image		*img;
	int			text_width;
	int			char_width;

	img = game->images->alphanum_sprite;
	text_width = get_text_width(game, info.str, info.height);
	if (info.str == NULL)
		return ;
	info.x = info.x - (text_width >> 1);
	info.y -= 7;
	i = -1;
	while (info.str[++i])
	{
		if (info.str[i] == ' ')
			info.x += info.height * 0.33;
		else
		{
			info.c = info.str[i];
			draw_char(game, info);
			char_width = get_char_width_opti(game, info.str[i]);
			info.x += char_width * info.height / 480 + 3;
		}
	}
}

void	draw_text_left(t_game *game, t_draw_info info)
{
	int			i;
	int			char_width;
	t_image		*img;

	img = game->images->alphanum_sprite;
	if (info.str == NULL)
		return ;
	i = -1;
	info.y -= 7;
	while (info.str[++i])
	{
		if (info.str[i] == ' ')
			info.x += info.height * 0.33;
		else
		{
			info.c = info.str[i];
			draw_char(game, info);
			char_width = get_char_width_opti(game, info.str[i]);
			info.x += char_width * info.height / 480 + 3;
		}
	}
}

void	draw_text_right(t_game *game, t_draw_info info)
{
	int			i;
	t_image		*img;
	int			text_width;
	int			char_width;

	img = game->images->alphanum_sprite;
	text_width = get_text_width(game, info.str, info.height);
	if (info.str == NULL)
		return ;
	info.x = info.x - text_width;
	i = -1;
	info.y -= 7;
	while (info.str[++i])
	{
		if (info.str[i] == ' ')
			info.x += info.height * 0.33;
		else
		{
			info.c = info.str[i];
			draw_char(game, info);
			char_width = get_char_width_opti(game, info.c);
			info.x += char_width * info.height / 480 + 3;
		}
	}
}

void	draw_rectangle(t_game *game, t_draw_info info)
{
	int		size_line;
	char	*data;
	int		off_base;
	int		*line_buffer;
	int		i[2];

	size_line = game->images->base->size_line;
	data = game->images->base->data;
	off_base = info.y * size_line + info.x * game->images->base->bpp * 0.125;
	line_buffer = gc_malloc(game->mem, info.width * sizeof(int));
	if (info.x < 0 || info.x >= game->screen_width
		|| info.y < 0 || info.y >= game->screen_height)
		return ;
	if (info.x + info.width > game->screen_width)
		info.width = game->screen_width - info.x;
	if (info.y + info.height > game->screen_height)
		info.height = game->screen_height - info.y;
	i[1] = -1;
	while (++i[1] < info.width)
		line_buffer[i[1]] = info.color;
	i[0] = -1;
	while (++i[0] < info.height)
		ft_memcpy(data + off_base + i[0] * size_line,
			line_buffer, info.width * sizeof(int));
	gc_free(game->mem, line_buffer);
}
