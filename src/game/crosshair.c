/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crosshair.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:16:38 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/21 00:01:28 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	crosshair(t_game *game)
{
	int		x;
	int		y;
	float	alpha;
	int 	cross_len;
	int		cross_width;

	cross_len = game->screen_height * 0.02;
	cross_width = game->screen_height * 0.005;
	alpha = 0.6; // Ajuster cette valeur entre 0 et 1 pour changer l'opacité

	y = game->screen_height / 2 - cross_len - 1;
	while (++y <= game->screen_height / 2 + cross_len)
	{
		x = game->screen_width / 2 - cross_len - 1;
		while (++x <= game->screen_width / 2 + cross_len)
			if (((game->screen_width - cross_width) / 2 <= x
				&& x <= (game->screen_width + cross_width) / 2)
				|| ((game->screen_height - cross_width) / 2 <= y
				&& y <= (game->screen_height + cross_width) / 2))
				pixel_put(game, x, y,
				blend_colors(get_pixel_color_from_image(game, x, y),
				9830400, alpha));
	}
}


void	draw_image(t_game *game, t_image *img, t_draw_info info)
{
	int			x1, y1;
	int			pos_x, pos_y;
	int			original_pixel_index;

	for (y1 = 0; y1 < info.height; y1++)
	{
		for (x1 = 0; x1 < info.height; x1++)
		{
			int source_x = (x1 * img->width / info.height);//  + (pos_x * img->width)
			int source_y = (y1 * img->height / info.height);//  + (pos_y * img->height)
			original_pixel_index = source_y * img->size_line + source_x * (img->bpp / 8);
			int color;
			color = *(unsigned int *)(img->data + original_pixel_index);
			if (color >= 0)
				pixel_put(game, info.x + x1, info.y + y1, color);
		}
	}
}

void	gun_draw(t_game *game)
{
	t_image		*im;
	t_draw_info	info;
	
	im = malloc(sizeof(t_image));
	if (game->player->anim_shoot < 0)
		game->player->anim_shoot = 0;
	if (game->player->anim_shoot > 0)
	{
		game->player->anim_shoot -= 15 * game->delta_time;
		info = init_draw_info(game->screen_height * 0.10, " ", game->screen_width * 0.473, game->screen_height * 0.552);
		draw_image(game, game->textures->fire, info);
	}
	info = init_draw_info(game->screen_height * 0.33, " ", game->screen_width * 0.38, game->screen_height * 0.61);
	draw_image(game, game->textures->weapon, info);
	// mlx_put_image_to_window(game->mlx, game->win, im->img, 0, 0);
}
