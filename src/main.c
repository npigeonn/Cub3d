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
/* 

mettre des textures sur les toit/ sol
gestion des sauts
gestion de la vie et des armes
gestion de la minimap
gerer le parsing, map trop grande /mal cerclee ...
gerer l'image sortie / ennemi / animation de sortie / hud
munitions
affichage des poings a 0 munitions
munitions au sol 
differentes armes?
ennemi qui se dirige vers toi un peu moins vite quand cest dans le champs de vision et degats


-des maps cools parce que cest le principal pour le correcteur
-un mode solo sortie / mode multi ensemble sortie/ mode melee generale





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
	la barre de vie
	viseur
	affichage des armes
	munitions
	gestion des armes / vie / munitions 

Y :	mettre les textures toit/ sol
	animations teleportation/ perte de vie / sortie

	gestion des attaques ennemies 

	serveur multijoueur

21-25
N : gerer les degats vision des ennemies
	gerer le parsing du multijoueur avec les spawns > nbre de player dans le serveur
	UN SEUL spawn si mode solo
	sortie visuelle (arceus)
	munitions visuelles? (cest la lettre M)


Y : gerer les mouvements en meme temps ex: droite et haut
	
	


	gerer les random textures?

28 - 29
	derniere modifs / leaks / normes
	-augmenter vitesse des projectiles
	- mieux gerer le champs de vision des bots
	- la touche B et espace (ne plus prendre en compte)
	- comprendre le freeze qui a lieu au bout d'1mn10 qd on joue
	/ si on reste spectateur sans trop bouger ca peut durer jusqu a 2mn50 avec un freewe couteux pour l'ordinateur
	/ le menu n'est pas pris en compte dans le freeze j'ai l'impression
	/
	- gerer les spawns pour le mode solo et multi
	- modif la couleur d'un partenaire ? dans le multijoueur


	- gere les leaks , PLUS AUCUN LEAK avant de commencer a  normer
	- couper les fonctions avec les bons parametres aussi
	-mettre dans les fichiers 
	-normer le contenu

*/

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
	if (!img->img)
	{
		printf("Erreur lors du chargement de la texture : %s\n", path);
		exit(1); // TODO free
	}
	img->data = mlx_get_data_addr(img->img, &img->bpp, &img->size_line, &img->endian);
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
	game->textures->bullet = gc_malloc(game->mem, sizeof(t_image));
	load_texture(game, game->textures->bullet, "./assets/sprites/bullet.xpm");
	game->textures->weapon = gc_malloc(game->mem, sizeof(t_image));
	load_texture(game, game->textures->weapon, "./assets/sprites/weapons/PISGE0.xpm");
	game->textures->fire = gc_malloc(game->mem, sizeof(t_image));
	load_texture(game, game->textures->fire, "assets/sprites/weapons/PISFA0.xpm");
	game->textures->ammo = gc_malloc(game->mem, sizeof(t_image));
	load_texture(game, game->textures->ammo, "./assets/sprites/ammos.xpm");
	game->textures->ammo->nb_sprite = 1;
	game->textures->ammo->sprite_height = game->textures->ammo->height;
	game->textures->ammo->sprite_width = game->textures->ammo->width;
}

void	init_floorcast(t_game *game)
{
	t_floorcast	floorcast;

	if (game->textures->color_f < 0)
	{
		floorcast.f_tex_width = game->textures->floor->width;
		floorcast.f_tex_height = game->textures->floor->height;
		floorcast.f_tex_data = game->textures->floor->data;
		floorcast.f_bpp = game->textures->floor->bpp / 8;
	}
	if (game->textures->color_c < 0)
	{
		floorcast.c_tex_width = game->textures->ceil->width;
		floorcast.c_tex_height = game->textures->ceil->height;
		floorcast.c_tex_data = game->textures->ceil->data;
		floorcast.c_bpp = game->textures->ceil->bpp / 8;
	}
	game->player->raycast->floorcast = floorcast;
}

void	load_game_texture(t_game *game)
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
	load_texture(game, game->textures->health, "./assets/sprites/heart.xpm");
	load_texture(game, game->textures->door, "./assets/sprites/ronflex.xpm");
	load_texture(game, game->textures->tp, "./assets/sprites/kadabra.xpm");
	load_texture(game, game->textures->enemy, "./assets/sprites/enemy_spritesheet.xpm");
	load_texture(game, game->textures->enemy_fire, "./assets/sprites/enemy_fire.xpm");
	load_texture(game, game->textures->enemy_death, "./assets/sprites/enemy_death.xpm");
	load_texture(game, game->textures->exit, "./assets/sprites/arceus.xpm");
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

void	reset_game(t_game *game)
{
	t_sprite	*current;
	t_sprite	*next;

	current = game->sprites;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	init_var(game, 0);
	init_player(game, 0);
	init_menu(game, 0);
	init_client(game, 0);
	free_map(game);
	parsing(game->av, game);
	set_direction(game, game->player->begin_dir);
	game->menu->status = MAIN_MENU;
}

void	hooks(t_game *game)
{
	mlx_mouse_move(game->mlx, game->win, game->screen_width >> 1 , game->screen_height >> 1 );
	mlx_hook(game->win, 6, 1L << 6, handle_mouse_move, game);
	mlx_hook(game->win, 4, 1L << 2, handle_mouse_key_press, game);
	mlx_hook(game->win, 5, 1L << 3, handle_mouse_key_release, game);
	mlx_hook(game->win, 33, 0, handle_close, game);
	mlx_hook(game->win, 2, 1L << 0, handle_key_press, game);
	mlx_hook(game->win, 3, 1L << 1, handle_key_release, game);
	mlx_loop_hook(game->mlx, game_loop, game);
	mlx_loop(game->mlx);
}

#include "raudio.h"
void	test_music(void)
{
	InitAudioDevice();

	Music music = LoadMusicStream("./assets/sound/sound.wav");
	PlayMusicStream(music);

	while (1)
		UpdateMusicStream(music);
	UnloadMusicStream(music);
	CloseAudioDevice();
}

# include <X11/X.h>
# include <X11/keysym.h>

int	main(int ac, char **av)
{
	t_game		game;
	(void)ac;

	// init structs pr cacher window
	// t_xvar *moha;
	// t_win_list *window;
	
	// test_music();
	game.mem = gc_init();
	game.mlx = mlx_init();
	game.av = av;
	mlx_get_screen_size(game.mlx, &game.screen_width, &game.screen_height);
	game.screen_height = 1080;
	game.screen_width = 1920;
	init_var(&game, 1);
	// //
	// moha = game.mlx;
	// window = game.win;
	// // hide
	// XFixesHideCursor(moha->display, window->window);
	// // show
	// XFixesShowCursor(moha->display, window->window);
	// //
	init_player(&game, 1);
	init_menu(&game, 1);
	init_client(&game, 1);
	load_game_texture(&game);
	parsing(av, &game);
	set_direction(&game, game.player->begin_dir);
	init_floorcast(&game);
	init_img(&game);
	set_width_all_letter(&game);
	game.win = mlx_new_window(game.mlx, game.screen_width, game.screen_height, "Cub3D");
	hooks(&game);
	return (0);
}
