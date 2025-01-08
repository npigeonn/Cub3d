/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:55:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/02 15:51:07 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	get_pos_char(char c, int *x, int *y)
{
	int	index;

	index = get_index_char(c);
	*x = index % 10;
	*y = index / 10;
}

int	get_original_pixel_index(t_image *img, int src_x, int src_y)
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
