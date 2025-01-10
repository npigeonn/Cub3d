/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:13:19 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/10 11:25:16 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	load_texture(t_game *game, t_image *img, char *path)
{
	img->img = mlx_xpm_file_to_image(game->mlx, path, &img->width,
			&img->height);
	if (!img->img)
	{
		printf("%s\n", path);
		game->is_running = false;
		gc_exit(game->mem, err("Erreur lors du chargement de la texture\n"));
	}
	img->data = mlx_get_data_addr(img->img, &img->bpp, &img->size_line,
			&img->endian);
}

static void	load_game_texture_malloc(t_game *game)
{
	game->textures = gc_malloc(game->mem, sizeof(t_textures));
	game->textures->east = gc_malloc(game->mem, sizeof(t_image));
	game->textures->north = gc_malloc(game->mem, sizeof(t_image));
	game->textures->south = gc_malloc(game->mem, sizeof(t_image));
	game->textures->west = gc_malloc(game->mem, sizeof(t_image));
	game->textures->door = gc_malloc(game->mem, sizeof(t_image));
	game->textures->tp = gc_malloc(game->mem, sizeof(t_image));
	game->textures->floor = gc_malloc(game->mem, sizeof(t_image));
	game->textures->ceil = gc_malloc(game->mem, sizeof(t_image));
	game->textures->enemy = gc_malloc(game->mem, sizeof(t_image));
	game->textures->enemy_fire = gc_malloc(game->mem, sizeof(t_image));
	game->textures->enemy_death = gc_malloc(game->mem, sizeof(t_image));
	game->textures->health = gc_malloc(game->mem, sizeof(t_image));
	game->textures->exit = gc_malloc(game->mem, sizeof(t_image));
}

static void	load_game_texture2(t_game *game)
{
	game->textures->enemy->nb_sprite = 8;
	game->textures->enemy->sprite_width = 64;
	game->textures->enemy->sprite_height = 64;
	game->textures->enemy_fire->nb_sprite = 1;
	game->textures->enemy_fire->sprite_width = 64;
	game->textures->enemy_fire->sprite_height = 64;
	game->textures->enemy_death->nb_sprite = 1;
	game->textures->enemy_death->sprite_width = 64;
	game->textures->enemy_death->sprite_height = 64;
	game->textures->tp->nb_sprite = 1;
	game->textures->tp->sprite_width = game->textures->tp->width;
	game->textures->tp->sprite_height = game->textures->tp->height;
	game->textures->exit->nb_sprite = 1;
	game->textures->exit->sprite_width = game->textures->exit->width;
	game->textures->exit->sprite_height = game->textures->exit->height;
	game->textures->health->nb_sprite = 1;
	game->textures->health->sprite_height = 160;
	game->textures->health->sprite_width = game->textures->health->width;
}

void	load_game_texture(t_game *game)
{
	load_game_texture_malloc(game);
	load_texture(game, game->textures->health, "./assets/sprites/heart.xpm");
	load_texture(game, game->textures->door, "./assets/sprites/ronflex.xpm");
	load_texture(game, game->textures->tp, "./assets/sprites/kadabra.xpm");
	load_texture(game, game->textures->enemy,
		"./assets/sprites/enemy_spritesheet.xpm");
	load_texture(game, game->textures->enemy_fire,
		"./assets/sprites/enemy_fire.xpm");
	load_texture(game, game->textures->enemy_death,
		"./assets/sprites/enemy_death.xpm");
	load_texture(game, game->textures->exit, "./assets/sprites/arceus.xpm");
	load_game_texture2(game);
}
