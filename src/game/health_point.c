/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_point.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 09:30:57 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/22 18:03:31 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	life(t_game *game)
{
	int	x;
	int	y;
	
	game->y_hp = game->screen_height * 0.855;
	game->x_hp = game->screen_width * 0.583;
	y = game->y_hp;

	while (++y < game->screen_height * 0.88)
	{
		x = game->x_hp;
		while (++x < game->screen_width * (.583 + .279 * game->player->health))
			pixel_put(game, x, y, 16711680);
	}

}


void	health_point_draw(t_game *game)
{

	int	x;
	int	y;
	float	alpha = 0.5; // Ajuster cette valeur entre 0 et 1 pour changer l'opacité

	game->y_hp = game->screen_height * 0.85;
	game->x_hp = game->screen_width * 0.58;
	y = game->y_hp - 1;
	while (++y <= game->screen_height * 0.885)
	{
		x = game->x_hp - 1;
		while (++x <= game->screen_width * 0.865)
		{
			pixel_put(game, x, y,
				blend_colors(get_pixel_color_from_image(game, x, y),
				0x000000, alpha));
			if ((game->screen_width * 0.862 <= x && x <= game->screen_width * 0.865)
				|| (game->screen_width * 0.58 <= x && x <= game->screen_width * 0.583)
				|| (game->screen_height * 0.85 <= y && y <= game->screen_height * 0.855)
				|| (game->screen_height * 0.88 <= y && y <= game->screen_height * 0.885))
				pixel_put(game, x, y, 0x000000);
		}
	}
	life(game);
}


// void	draw_anim_health(t_game *game, int x, int y, t_image *im_health)
// {
// 	t_image *to_send;
// 	int		x;
// 	int		y;
// 	int		x_size;
// 	int		y_size;
	
// 	x_size = 155;
// 	y_size = 154;
// 	to_send = malloc(sizeof(t_image));
// 	to_send->img = mlx_new_image(game->mlx, 776 / 5, 308 / 2);
// 	to_send->data = mlx_get_data_addr(to_send->img, to_send->bpp, to_send->size_line, to_send->endian);

// 	// mettre dans to_send la partie superieur gauche (1/10e de l'image) de l'image health.xpm pour pouvoir dessiner cette partie dans draw_sprite
// 	draw_sprite(game, to_send, x, y, 150, 0.4, 1); // 308 /2 et 1556/5
// }

void	draw_anim_health(t_game *game, int x, int y, t_image *im_health)
{
	t_image *to_send;
	int		x1, y1;
	int		x_size = 776 / 5;  // Largeur de la sous-image (1/5e de l'image)
	int		y_size = 308 / 2;  // Hauteur de la sous-image (1/2 de l'image)
	int		original_pixel_index;
	int		new_pixel_index;

	// Création de l'image temporaire to_send de la même taille que la portion à extraire
	to_send = malloc(sizeof(t_image));
	to_send->img = mlx_new_image(game->mlx, x_size, y_size);
	to_send->data = mlx_get_data_addr(to_send->img, &to_send->bpp, &to_send->size_line, &to_send->endian);

	// Copier les pixels de la portion supérieure gauche de im_health (1/10e de l'image totale)
	for (y1 = 0; y1 < y_size; y1++)
	{
		for (x1 = 0; x1 < x_size; x1++)
		{
			// Calcul des index des pixels dans im_health et to_send
			original_pixel_index = y1 * im_health->size_line + x1 * (im_health->bpp / 8); // Pixels de la partie supérieure gauche
			new_pixel_index = y1 * to_send->size_line + x1 * (to_send->bpp / 8);           // Pixels dans to_send

			// Copie des données de pixel
			to_send->data[new_pixel_index] = im_health->data[original_pixel_index];
		}
	}

	// Dessiner la portion extraite (to_send) à la position donnée
	draw_sprite(game, to_send, x, y, 150, 0.4, 1);

	// Libérer la mémoire allouée pour to_send
	mlx_destroy_image(game->mlx, to_send->img);
	free(to_send);
}


void	draw_collectible_life(t_game *game)
{
	t_image 	*im_health;
	t_health	*current;

	current = game->health;
	im_health = malloc(sizeof(t_image));

	load_texture(game, im_health, "./assets/sprites/health.xpm");
	im_health->nb_sprite = 1;
	im_health->sprite_height = im_health->height;
	im_health->sprite_width = im_health->width;
	while (current)
	{
		if (current->still_exist)
			draw_anim_health(game, current->x, current->y, im_health);
		current = current->next;
	}
}

void	on_life(t_game *game)
{
	t_health *current;

	current = game->health;
	if (game->map[game->player->floor][(int)game->player->y][(int)game->player->x] == 'H')
	{
		while (current)
		{
			if ((int)game->player->x == current->x && (int)game->player->y == current->y
				&& game->player->floor == current->floor)
			{
				if (current->still_exist == 1)
				{
					current->still_exist = 0;
					if (game->player->health < 0.75)
						game->player->health += 0.25;
					else
						game->player->health = 1;
				}
				return ;
			}
			current = current->next;
		}
	}
}
