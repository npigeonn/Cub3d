// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/09/27 09:54:03 by npigeon           #+#    #+#             */
// /*   Updated: 2024/09/27 11:44:59 by npigeon          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// # include "include/game.h"

// mettre des textures sur les toit/ sol
// gestion des sauts
// gestion de la vie et des armes
// gestion de la minimap
// gerer le parsing, map trop grande /mal cerclee ...
// gerer l'image sortie / ennemi / animation de sortie / hud
// munitions / cle


/* 
MODES DE JEU

sortir solo multi
melee generale multi infini ou avec une lim de kill
melee fornite

9-11  
N : parsing gestion des cles
Y : mob ennemi/teleporteur/sortie/munition /cles / affichage pero


14-19 
N : gestion mode de jeu
	preparer des maps
	gere la minimap
	gestion des armes / vie / munitions 

Y :	mettre les textures toit/ sol
	animations teleportation/ perte de vie / sortie

	gestion des attaques ennemies 

	serveur multijoueur
*/


#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include "../includes/cub3d.h"

# define KEY_W 119
# define KEY_Z 122
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_Q 113

void	load_texture(t_game *game, t_image *img, char *path)
{
	img->img = mlx_xpm_file_to_image(game->mlx, path, &img->width, &img->height);
	img->data = mlx_get_data_addr(img->img, &img->bpp, &img->size_line, &img->endian);
	if (!img->img)
	{
		printf("Erreur lors du chargement de la texture : %s\n", path);
		exit(1);
	}
}

void	init_img(t_game *game)
{
	t_image	*base;

	game->images = gc_malloc(game->mem, sizeof(t_images));
	game->images->alphanum_sprite = gc_malloc(game->mem, sizeof(t_image));
	base = gc_malloc(game->mem, sizeof(t_image));
	base->img = mlx_new_image(game->mlx, game->screen_width, game->screen_height);
	base->data = mlx_get_data_addr(base->img, &base->bpp, &base->size_line, &base->endian);
	game->images->base = base;
	load_texture(game, game->images->alphanum_sprite, "./assets/sprites/alphanum_sprite.xpm");
}

void	init_player(t_game	*game)
{
	game->player = gc_malloc(game->mem, sizeof(t_player));
	game->player->x = 0;
	game->player->y = 0;
	game->player->floor = 0;
	game->player->height = 0;
	game->player->dirX = 1;
	game->player->dirY = 0;
	game->player->planeX = 0;
	game->menu = malloc(sizeof(t_menu));
	game->menu->volume = 20;
	game->menu->mouse_sensitivity = 2;
	game->menu->message = NOTHING;
	game->servers = NULL;
}

void	load_game_texture(t_game *game)
{
	game->textures = malloc(sizeof(t_textures));
	game->textures->east = malloc(sizeof(t_image));
	game->textures->north = malloc(sizeof(t_image));
	game->textures->sud = malloc(sizeof(t_image));
	game->textures->west = malloc(sizeof(t_image));
	game->textures->door = malloc(sizeof(t_image));
	game->textures->tp = malloc(sizeof(t_image));
	game->textures->floor = malloc(sizeof(t_image));
	game->textures->ceil = malloc(sizeof(t_image));
	game->textures->enemies = malloc(sizeof(t_image));
	load_texture(game, game->textures->door, "./assets/sprites/ronflex.xpm");
	load_texture(game, game->textures->tp, "./assets/sprites/kadabra.xpm");
	load_texture(game, game->textures->enemies, "./assets/sprites/enemies.xpm");
}

int	main(int ac, char **av)
{
	t_game		game;
	(void)ac;
	
	game.mem = gc_init();
	game.mlx = mlx_init();
	gettimeofday(&game.last_time, NULL);
	mlx_get_screen_size(game.mlx, &game.screen_width, &game.screen_height);
	game.screen_height = 1080;
	game.screen_width = 1920;
	init_player(&game);
	game.menu->status = MAIN_MENU;
	game.menu->button_selected = 0;
	game.wall_distances = malloc(sizeof(float) * game.screen_width);
	ft_bzero(game.wall_distances, game.screen_width);
	game.door = NULL;
	game.tp = NULL;
	game.enemies = NULL;
	load_game_texture(&game);
	parsing(av, &game);
	init_enemies(&game);
	game.player->x += 0.5;
	game.player->y += 0.5;
	set_direction(&game, 0);
	game.win = mlx_new_window(game.mlx, game.screen_width, game.screen_height, "Raycasting 3D");
	init_img(&game);
	mlx_mouse_move(game.mlx, game.win, game.screen_width >> 1 , game.screen_height >> 1 );
	mlx_hook(game.win, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.win, 6, 1L << 6, handle_mouse_move, &game);
	mlx_hook(game.win, 4, 1L << 2, handle_mouse_key, &game);
	mlx_hook(game.win, 33, 0, handle_close, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	printf("player pos %f and %f\n", game.player->x, game.player->y);
	mlx_loop(game.mlx);
	free_map(&game);
	return 0;
}
