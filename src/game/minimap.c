/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:12:49 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/22 09:33:54 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/cub3d.h"

// Fonction pour obtenir la couleur d'un pixel à une position donnée dans une image
int get_pixel_color_from_image(t_game *game, int x, int y)
{
	const		t_image *img = game->images->base;
	const int	offset = y * img->size_line + x * (img->bpp >> 3);
		
	return *(int *)(img->data + offset);
}

// Fonction pour placer un pixel à une position donnée dans une image
void	put_pixel_to_image(char *img_data, int x, int y, int color, int line_length, int bpp)
{
	char	*dst;

	// Calculer l'adresse du pixel
	dst = img_data + (y * line_length + x * (bpp / 8));
	*(unsigned int *)dst = color;
}

// Fonction pour mélanger deux couleurs avec un certain niveau d'alpha
int blend_colors(int bg_color, int fg_color, float alpha)
{
	int r_bg = (bg_color >> 16) & 0xFF;
	int g_bg = (bg_color >> 8) & 0xFF;
	int b_bg = bg_color & 0xFF;

	int r_fg = (fg_color >> 16) & 0xFF;
	int g_fg = (fg_color >> 8) & 0xFF;
	int b_fg = fg_color & 0xFF;

	int r_result = (int)(r_fg * alpha + r_bg * (1 - alpha));
	int g_result = (int)(g_fg * alpha + g_bg * (1 - alpha));
	int b_result = (int)(b_fg * alpha + b_bg * (1 - alpha));

	return (r_result << 16 | g_result << 8 | b_result);
}

int	y_size_floor(t_game *game)
{
	int	i;

	i = -1;
	while (game->map[game->player->floor][++i])
		continue ;
	return (i);
}

int x_size_floor(t_game *game)
{
	int	i;
	int	x;
	int res;

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

void	put_pixel_minimap(t_game *game, int x, int y, int size)
{
	int i;
	int j;
	int	color;
	
	i = -1;
	if (game->map[game->player->floor][y][x] == '1')
		color = 16777215;
	else if (game->map[game->player->floor][y][x] == 'D')
		color = 6579300;
	else if ('a' <= game->map[game->player->floor][y][x]
		&& game->map[game->player->floor][y][x] <= 'z'
		&& game->map[game->player->floor][y][x] != 'e')
		color = ((90 + (33 * ('z' - game->map[game->player->floor][y][x]))
			% 165) << 16) | (0 << 8) | 0;
	else
		return ;
	while (++i <= size)
	{
		j = -1;
		while (++j <= size)
			pixel_put(game, game->x_minimap + x * size + j,
				game->y_minimap + y * size + i, color);
	}
}

void	put_pixel_player(t_game *game, int size)
{
	int i;
	int j;

	
	i = -1 - size / 2;
	while (++i <= size / 2)
	{
		j = -1 - size / 2;
		while (++j <= size / 2)
		{
			if (i * i + j * j <= size * size / 4)
				pixel_put(game, game->x_minimap + game->player->x * size + j,
				game->y_minimap + game->player->y * size + i, 65280);
		}
	}
}

void	print_wall_door_player(t_game *game)
{
	int	x;
	int	y;
	int	h;
	int	size_pix;

	h = (game->screen_width - game->x_minimap) / 10;
	size_pix = (game->screen_width - game->x_minimap - 2 * h)
		/ x_size_floor(game);
	if (size_pix > (game->screen_height - game->y_minimap - 2 * h)
		/ y_size_floor(game))
		size_pix = (game->screen_height - game->y_minimap - 2 * h)
			/ y_size_floor(game);
	y = -1;
	game->x_minimap += h + (game->screen_width - game->x_minimap)
		/ x_size_floor(game);
	game->y_minimap += .5 * h + (game->screen_height - game->y_minimap)
		/ y_size_floor(game);
	while (game->map[game->player->floor][++y])
	{
		x = -1;
		while (game->map[game->player->floor][y][++x])
			put_pixel_minimap(game, x, y, size_pix);
	}
	put_pixel_player(game, size_pix);
}


void	mini_map(t_game *game)
{
	int	x;
	int	y;
	int	background_color;
	int	grey_with_alpha;
	float	alpha = 0.4; // Ajuster cette valeur entre 0 et 1 pour changer l'opacité

	int grey = 0x000000; // Gris
	game->y_minimap = game->screen_height - game->screen_height / 4;
	game->x_minimap = game->screen_width - game->screen_width / 8;
	y = game->y_minimap;
	while (y <= game->screen_height)
	{
		x = game->x_minimap;
		while (x <= game->screen_width)
		{
			background_color = get_pixel_color_from_image(game, x, y);
			grey_with_alpha = blend_colors(background_color, grey, alpha);
			pixel_put(game, x, y, grey_with_alpha);
			x++;
		}
		y++;
	}
	print_wall_door_player(game);
}
