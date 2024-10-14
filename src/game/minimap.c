/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:12:49 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/14 18:09:45 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// // Fonction pour obtenir la couleur d'un pixel à une position donnée dans une image
// int	get_pixel_color_from_image(char *img_data, int x, int y, int line_length, int bpp)
// {
// 	int	color;
// 	char	*pixel;

// 	// Calculer l'adresse du pixel en fonction de x, y, et les propriétés de l'image
// 	pixel = img_data + (y * line_length + x * (bpp / 8));
// 	color = *(unsigned int *)pixel;
// 	return (color);
// }

// // Fonction pour placer un pixel à une position donnée dans une image
// void	put_pixel_to_image(char *img_data, int x, int y, int color, int line_length, int bpp)
// {
// 	char	*dst;

// 	// Calculer l'adresse du pixel
// 	dst = img_data + (y * line_length + x * (bpp / 8));
// 	*(unsigned int *)dst = color;
// }

// // Fonction pour mélanger deux couleurs avec un certain niveau d'alpha
// int blend_colors(int bg_color, int fg_color, float alpha)
// {
// 	int r_bg = (bg_color >> 16) & 0xFF;
// 	int g_bg = (bg_color >> 8) & 0xFF;
// 	int b_bg = bg_color & 0xFF;

// 	int r_fg = (fg_color >> 16) & 0xFF;
// 	int g_fg = (fg_color >> 8) & 0xFF;
// 	int b_fg = fg_color & 0xFF;

// 	// Mélange des composants rouge, vert, bleu
// 	int r_result = (int)(r_fg * alpha + r_bg * (1 - alpha));
// 	int g_result = (int)(g_fg * alpha + g_bg * (1 - alpha));
// 	int b_result = (int)(b_fg * alpha + b_bg * (1 - alpha));

// 	// Combiner les composantes en une seule couleur
// 	return (r_result << 16 | g_result << 8 | b_result);
// }

// void	mini_map(t_game *game)
// {
// 	int	x_minmap;
// 	int	y_minmap;
// 	int	background_color;
// 	int	grey_with_alpha;
// 	float	alpha = 0.8; // Ajuster cette valeur entre 0 et 1 pour changer l'opacité

// 	// Variables pour manipuler l'image de MLX
// 	char	*img_data = game->images->base->data; // Données de l'image (à initialiser avec mlx_get_data_addr)
// 	int	line_length = game->screen_width; // Longueur de la ligne de l'image (à initialiser avec mlx_get_data_addr)
// 	int	bpp = game->images->base->bpp; // Bits par pixel (à initialiser avec mlx_get_data_addr)

// 	int grey = 0x000000; // Gris

// 	// Commence à dessiner le rectangle à partir du bas droit de l'écran
// 	y_minmap = game->screen_height - game->screen_height / 4;
// 	while (y_minmap <= game->screen_height)
// 	{
// 		x_minmap = game->screen_width - game->screen_width / 8;
// 		while (x_minmap <= game->screen_width)
// 		{
// 			// Récupérer la couleur de fond à cette position
// 			background_color = get_pixel_color_from_image(img_data, x_minmap, y_minmap, line_length, bpp);

// 			// Mélange entre le gris et la couleur de fond avec l'opacité définie
// 			grey_with_alpha = blend_colors(background_color, grey, alpha);

// 			// Placer le pixel final dans l'image
// 			put_pixel_to_image(img_data, x_minmap, y_minmap, grey_with_alpha, line_length, bpp);

// 			x_minmap++;
// 		}
// 		y_minmap++;
// 	}
// }




void	mini_map(t_game *game)
{
	int	x;
	int	y;
	// printf("la taille du screen largeur = %d\n",  game->screen_width);
	game->y_minimap = game->screen_height - game->screen_height / 4;
	game->x_minimap = game->screen_width - game->screen_width / 8;
	game->x_mid_minimap = (game->x_minimap + game->screen_width) / 2;
	game->y_mid_minimap = (game->y_minimap + game->screen_height) / 2;
	y = game->y_minimap;
	while (y <= game->screen_height)
	{
		x = game->x_minimap;
		while (x <= game->screen_width)
		{
			pixel_put(game, x, y, 0); // blanc =16777215
			x++;
		}
		y++;
	}
	print_wall_door_player(game);
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
		color = 65280;
	else
		color = 0;
	while (++i <= size)
	{
		j = -1;
		while (++j <= size)
			pixel_put(game, game->x_minimap + x * size + j, game->y_minimap + y * size + i, color);
	}
}

void	print_wall_door_player(t_game *game)
{
	int	x;
	int	y;
	int	h;
	int	size_pix;


	h = (game->screen_width - game->x_minimap) / 10;
	size_pix = (game->screen_width - game->x_minimap - 2 * h) / x_size_floor(game);
	if (size_pix > (game->screen_height - game->y_minimap - 2 * h) / y_size_floor(game))
		size_pix = (game->screen_height - game->y_minimap - 2 * h) / y_size_floor(game);
	y = -1;
	game->y_minimap += h;
	game->x_minimap += h;
	while (game->map[game->player->floor][++y])
	{
		x = -1;
		while (game->map[game->player->floor][y][++x])
			put_pixel_minimap(game, x, y, size_pix);
	}
	
}

