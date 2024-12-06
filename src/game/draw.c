/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:55:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/27 16:54:55 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_index_char(char c)
	{
	const char	list[96] = "!#$&(),;@[]:?_\"|\\\\/*<>%-'`~£$+=+0123456789"
		"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUu"
		"VvWwXxYyZz.";
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

int	get_char_width_return(int left_bound, int right_bound)
{
	if (left_bound > right_bound)
		return (0);
	return (right_bound - left_bound + 1);
}

int	get_char_width(t_image *img, char c)
{
	int			left_bound;
	int			right_bound;
	const int	char_x = (get_index_char(c) % 10) * 480;
	const int	char_y = (get_index_char(c) / 10) * 480;
	int			pos[2];

	left_bound = 480;
	right_bound = -1;
	pos[0] = -1;
	while (++pos[0] < 480)
	{
		pos[1] = -1;
		while (++pos[1] < 480)
		{
			if (!is_pixel_transparent(img, pos[1] + char_x, pos[0] + char_y))
			{
				if (pos[1] < left_bound)
					left_bound = pos[1];
				if (pos[1] > right_bound)
					right_bound = pos[1];
			}
		}
	}
	return (get_char_width_return(left_bound, right_bound));
}

int	get_char_width_opti(t_game *game, char x)
{
	return (game->menu->width_letter[get_index_char(x)]);
}

void	set_width_all_letter(t_game *game)
{
	const char	list[96] = "!#$&(),;@[]:?_\"|\\\\/*<>%-'`~£$+=+0123456789"
		"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUu"
		"VvWwXxYyZz.";
	int			i;
	int			width;
	int			index;

	i = -1;
	while (list[++i])
	{
		width = get_char_width(game->images->alphanum_sprite, list[i]);
		index = get_index_char(list[i]);
		if (index >= 0 && index < 96)
			game->menu->width_letter[index] = width;
	}
}

void	pixel_put(t_game *game, int x, int y, int color)
{
	int		offset;
	t_image	*img;

	img = game->images->base;
	offset = y * img->size_line + x * img->bpp / 8;
	if (x < 0 || x >= game->screen_width || y < 0 || y >= game->screen_height)
		return ;
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
	if (x < 0 || x >= game->screen_width || y < 0 || y >= game->screen_height)
		return ;
	if (*pixel == 0)
		*pixel = color;
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

static int	get_original_pixel_index(t_image *img, int src_x, int src_y)
{
	return (src_y * img->size_line + src_x * (img->bpp / 8));
}

static void	draw_char_pixel(t_game *game, t_draw_info *info, t_image *img,
int info_tab[4])
{
	int	src_x;
	int	src_y;
	int	pixel_index;

	src_x = (info_tab[3] * 480 / info->height) + (info_tab[0] * 480);
	src_y = (info_tab[2] * 480 / info->height) + (info_tab[1] * 480);
	pixel_index = get_original_pixel_index(img, src_x, src_y);
	if (img->data[pixel_index] != 0)
		pixel_put(game, info->x + info_tab[3], info->y + info_tab[2],
			info->color);
}

void	draw_char(t_game *game, t_draw_info info)
{
	t_image	*img;
	int		info_tab[4];

	img = game->images->alphanum_sprite;
	get_pos_char(info.c, &info_tab[0], &info_tab[1]);
	info_tab[2] = -1;
	while (++info_tab[2] < info.height)
	{
		info_tab[3] = -1;
		while (++info_tab[3] < info.height)
			draw_char_pixel(game, &info, img, info_tab);
	}
}

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
