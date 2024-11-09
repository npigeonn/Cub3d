/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:55:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/09 18:00:23 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_index_char(char c)
{
	const char	list[96] = "!#$&(),;@[]:?_\"|\\\\/*<>%-'`~£$+=+0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz.";
	int			i;

	i = -1;
	while (list[++i])
	{
		if (list[i] == c)
		{
			if (i > 32)
				return (i - 1);
			return (i);
		}
	}
	return (-1);
}

int	get_char_width(t_image *img, char c)
{
	int	index = get_index_char(c);
	int	left_bound = 480;
	int	right_bound = -1;
	int	char_x = (index % 10) * 480;
	int	char_y = (index / 10) * 480;
	
	for (int y = 0; y < 480; y++)
	{
		for (int x = 0; x < 480; x++)
		{
			if (!is_pixel_transparent(img, x + char_x, y + char_y))
			{
				if (x < left_bound) left_bound = x;
				if (x > right_bound) right_bound = x;
			}
		}
	}
	if (left_bound > right_bound)
		return (0);
	return (right_bound - left_bound + 1);
}

int	get_char_width_opti(t_game *game, char x)
{
	return (game->menu->width_letter[get_index_char(x)]);
}

void	set_width_all_letter(t_game *game)
{
	const char	list[96] = "!#$&(),;@[]:?_\"|\\\\/*<>%-'`~£$+=+0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz.";
	int			i;

	i = -1;
	while (list[++i] && i <= 95)
	{
		int width = get_char_width(game->images->alphanum_sprite, list[i]);
		int index = get_index_char(list[i]);
		
		if (index >= 0 && index < 96)
			game->menu->width_letter[index] = width;
	}
}

int	get_color(t_game *game, int x, int y, int color)
{
	const float	adjusted_alpha = 0.2 * (1 - game->player->health) *
		(1 + (abs(x - game->cen_x) / (game->half_cen_x)) +
		(abs(y - game->cen_y) / (game->half_cen_y)));
	return (blend_colors(color, 9830400, adjusted_alpha));
}

void	pixel_put(t_game *game, int x, int y, int color)
{
	int		offset;
	t_image	*img;

	img = game->images->base;
	offset = y * img->size_line + x * img->bpp / 8;
	if (game->player->health < 1)
		*((int *)(img->data + offset)) = get_color(game, x, y, color);
	else
		*((int *)(img->data + offset)) = color;
}

void	secure_pixel_put(t_game *game, int x, int y, int color)
{
	int		offset;
	int		*pixel;
	t_image	*img;

	img = game->images->base;
	offset = y * img->size_line + x * (img->bpp >> 3);
	pixel = (int *)(img->data + offset);
	if (*pixel == 0)
	{
		if (game->player->health < 1)
			*pixel = get_color(game, x, y, color);
		else
			*pixel = color;
	}
}

int	is_pixel_transparent(t_image *img, int x, int y)
{
	int	pixel;

	if (!img || !img->data)
		return (1);
	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return (1);
	pixel = *(int *)(img->data + y * img->size_line + x * (img->bpp / 8));
	return (pixel <= 0);
}

void	get_pos_char(char c, int *x, int *y)
{
	int	index;

	index = get_index_char(c);
	*x = index % 10;
	*y = index / 10;
}

void	draw_char(t_game *game, t_draw_info info)
{
	int			x1, y1;
	int			pos_x, pos_y;
	int			original_pixel_index;
	t_image		*img = game->images->alphanum_sprite;

	get_pos_char(info.c, &pos_x, &pos_y);
	for (y1 = 0; y1 < info.height; y1++)
	{
		for (x1 = 0; x1 < info.height; x1++)
		{
			int source_x = (x1 * 480 / info.height) + (pos_x * 480);
			int source_y = (y1 * 480 / info.height) + (pos_y * 480);
			original_pixel_index = source_y * img->size_line
				+ source_x * img->bpp * 0.125;
			if (game->images->alphanum_sprite->data[original_pixel_index] != 0)
				pixel_put(game, info.x + x1, info.y + y1, info.color);
		}
	}
}

int	get_text_width(t_game *game, char *str, int height)
{
	int	total_width = 0;
	int	i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == ' ')
			total_width += height * 0.33;
		else if (str[i] != '\n')
		{
			int char_width = game->menu->width_letter[get_index_char(str[i])];
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
			info.x += char_width *info. height / 480 + 3;
		}
	}
}

void draw_rectangle(t_game *game, t_draw_info info)
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
