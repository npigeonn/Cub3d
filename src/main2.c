// // /* ************************************************************************** */
// // /*                                                                            */
// // /*                                                        :::      ::::::::   */
// // /*   main.c                                             :+:      :+:    :+:   */
// // /*                                                    +:+ +:+         +:+     */
// // /*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
// // /*                                                +#+#+#+#+#+   +#+           */
// // /*   Created: 2024/09/27 09:54:03 by npigeon           #+#    #+#             */
// // /*   Updated: 2024/09/27 11:44:59 by npigeon          ###   ########.fr       */
// // /*                                                                            */
// // /* ************************************************************************** */

// // # include "include/game.h"
// /* 

// mettre des textures sur les toit/ sol
// gestion des sauts
// gestion de la vie et des armes
// gestion de la minimap
// gerer le parsing, map trop grande /mal cerclee ...
// gerer l'image sortie / ennemi / animation de sortie / hud
// munitions
// affichage des poings a 0 munitions
// munitions au sol 
// differentes armes?
// ennemi qui se dirige vers toi un peu moins vite quand cest dans le champs de vision et degats


// -des maps cools parce que cest le principal pour le correcteur
// -un mode solo sortie / mode multi ensemble sortie/ mode melee generale





// MODES DE JEU

// sortir solo multi
// melee generale multi infini ou avec une lim de kill
// melee fornite

// 9-11  
// N : parsing gestion des cles
// Y : mob ennemi/teleporteur/sortie/munition /cles / affichage pero


// 14-19 
// N : gestion mode de jeu
// 	preparer des maps
// 	gere la minimap
// 	la barre de vie
// 	viseur
// 	affichage des armes
// 	munitions
// 	gestion des armes / vie / munitions 

// Y :	mettre les textures toit/ sol
// 	animations teleportation/ perte de vie / sortie

// 	gestion des attaques ennemies 

// 	serveur multijoueur

// 21-25
// N : gerer les degats vision des ennemies
// 	gerer le parsing du multijoueur avec les spawns > nbre de player dans le serveur
// 	UN SEUL spawn si mode solo


// Y : gerer les mouvements en meme temps ex: droite et haut
// 	munitions visuelles? (cest la lettre M)
// 	sortie visuelle (arceus)
	
	


// 	gerer les random textures?

// 28 - 29
// 	derniere modifs / leaks / normes


// */

// #include "../includes/cub3d.h"

// void	load_texture(t_game *game, t_image *img, char *path)
// {
// 	img->img = mlx_xpm_file_to_image(game->mlx, path, &img->width, &img->height);
// 	if (!img->img)
// 	{
// 		printf("Erreur lors du chargement de la texture : %s\n", path);
// 		exit(1);
// 	}
// 	img->data = mlx_get_data_addr(img->img, &img->bpp, &img->size_line, &img->endian);
// }

// void	load_game_texture(t_game *game)
// {
// 	game->textures = gc_malloc(game->mem, sizeof(t_textures));
// 	game->textures->east = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->north = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->south = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->west = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->door = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->tp = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->floor = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->ceil = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->enemy = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->enemy_fire = gc_malloc(game->mem, sizeof(t_image));
// 	game->textures->enemy_death = gc_malloc(game->mem, sizeof(t_image));
// 	load_texture(game, game->textures->door, "./assets/sprites/ronflex.xpm");
// 	load_texture(game, game->textures->tp, "./assets/sprites/kadabra.xpm");
// 	load_texture(game, game->textures->enemy, "./assets/sprites/enemy_spritesheet.xpm");
// 	load_texture(game, game->textures->enemy_fire, "./assets/sprites/enemy_fire.xpm");
// 	load_texture(game, game->textures->enemy_death, "./assets/sprites/enemy_death.xpm");
// 	game->textures->enemy->nb_sprite = 8;
// 	game->textures->enemy->sprite_width = 64;
// 	game->textures->enemy->sprite_height = 64;
// 	game->textures->enemy_fire->nb_sprite = 1;
// 	game->textures->enemy_fire->sprite_width = 64;
// 	game->textures->enemy_fire->sprite_height = 64;
// 	game->textures->enemy_death->nb_sprite = 1;
// 	game->textures->enemy_death->sprite_width = 64;
// 	game->textures->enemy_death->sprite_height = 64;
// 	game->textures->tp->nb_sprite = 1;
// 	game->textures->tp->sprite_width = game->textures->tp->width;
// 	game->textures->tp->sprite_height = game->textures->tp->height;
// }

// int	main(int ac, char **av)
// {
// 	t_game	game;
// 	(void)ac;
	
// 	game.mem = gc_init();

// 	game.mlx = mlx_init();
// 	gettimeofday(&game.last_time, NULL);
// 	mlx_get_screen_size(game.mlx, &game.screen_width, &game.screen_height);
// 	game.screen_height = 1080;
// 	game.screen_width = 1920;
// 	game.win = mlx_new_window(game.mlx, game.screen_width, game.screen_height, "Cub3D");

// 	init_player(&game);
// 	init_chatbox(&game);
// 	init_menu(&game);
// 	init_client(&game);
// 	load_game_texture(&game);
// 	init_img(&game);
// 	set_width_all_letter(&game);

// 	game.sprites = NULL;
// 	game.door = NULL;
// 	game.fade_progress = 0;
// 	game.key_flags = 0;
// 	game.projectiles = NULL;
// 	game.wall_distances = gc_malloc(game.mem, sizeof(float) * game.screen_width);
// 	ft_bzero(game.wall_distances, game.screen_width);

// 	parsing(av, &game);
// 	init_enemies(&game);
// 	set_direction(&game, game.player->begin_dir);
// 	init_floorcast(&game);

// 	mlx_mouse_move(game.mlx, game.win, game.screen_width >> 1 , game.screen_height >> 1 );
// 	mlx_hook(game.win, 6, 1L << 6, handle_mouse_move, &game);
// 	mlx_hook(game.win, 4, 1L << 2, handle_mouse_key, &game);
// 	mlx_hook(game.win, 33, 0, handle_close, &game);
// 	mlx_hook(game.win, 2, 1L << 0, handle_keypress, &game);
// 	mlx_hook(game.win, 3, 1L << 1, handle_keyrelease, &game);
// 	mlx_loop_hook(game.mlx, game_loop, &game);
// 	mlx_loop(game.mlx);
// 	free_map(&game);
// 	return (0);
// }
