/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:59:37 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 11:59:37 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_pixel_color_from_image(t_game *game, int x, int y)
{
	const t_image	*img = game->images->base;
	const int		offset = y * img->size_line + x * (img->bpp >> 3);

	return (*(int *)(img->data + offset));
}

int	blend_colors(int bg_color, int fg_color, float alpha)
{
	const int	alpha_int = (int)(alpha * 255);
	const int	inv_alpha_int = 255 - alpha_int;
	const int	r_result = ((fg_color & 0xFF0000) * alpha_int
			+ (bg_color & 0xFF0000) * inv_alpha_int) >> 8 & 0xFF0000;
	const int	g_result = ((fg_color & 0x00FF00) * alpha_int
			+ (bg_color & 0x00FF00) * inv_alpha_int) >> 8 & 0x00FF00;
	const int	b_result = ((fg_color & 0x0000FF) * alpha_int
			+ (bg_color & 0x0000FF) * inv_alpha_int) >> 8 & 0x0000FF;

	return (r_result | g_result | b_result);
}

int	y_size_floor(t_game *game)
{
	int	i;

	i = -1;
	while (game->map[game->player->floor][++i])
		continue ;
	return (i);
}

int	x_size_floor(t_game *game)
{
	int	i;
	int	x;
	int	res;

	i = -1;
	res = 0;
	while (game->map[game->player->floor][++i])
	{
		x = -1;
		while (game->map[game->player->floor][i][++x])
			continue ;
		if (x > res)
			res = x;
	}
	return (res);
}
