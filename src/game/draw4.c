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
