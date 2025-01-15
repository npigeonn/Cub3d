/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:04:25 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 13:04:25 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
	t_image	*img;
	int		offset;
	int		*pixel;

	if (x < 0 || x >= game->screen_width || y < 0 || y >= game->screen_height)
		return ;
	img = game->images->base;
	if (!img || !img->data || img->bpp <= 0 || img->size_line <= 0)
		return ;
	offset = y * img->size_line + x * (img->bpp >> 3);
	if (offset < 0 || offset >= img->size_line * game->screen_height)
		return ;
	pixel = (int *)(img->data + offset);
	if (!pixel)
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
