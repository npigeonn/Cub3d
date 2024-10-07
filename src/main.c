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

// // mettre des textures sur les murs / toit/ sol
// // crafting d'image
// // pouvoir se deplacer seulement avec les fleches
// // gestion de la souris
// // gestion des sauts
// // gestion de la vie et des armes
// // gestion de la minimap
// // gerer le parsing, map trop grande /mal cerclee ...

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
	game->player->height = 0;
	game->player->dirX = 1;
	game->player->dirY = 0;
	game->player->planeX = 0;
	game->volume = 20;
	game->mouse_sensitivity = 2;
}

void	load_game_texture(t_game *game)
{
	load_texture(game, game->textures->zekrom, "./assets/sprites/zekrom.xpm");
}

int	main(int ac, char **av)
{
	t_game		game;
	(void)ac;
	
	game.mem = gc_init();
	game.mlx = mlx_init();
	mlx_get_screen_size(game.mlx, &game.screen_width, &game.screen_height);
	//remove this
	game.screen_width = 1920;
	game.screen_height = 1080;
	//////////////////////////
	game.status = MAIN_MENU;
	game.button_selected = 0;
	game.map = malloc(sizeof(t_map));

	load_game_texture(&game);

	init_player(&game);
	parsing(av, &game);
	game.player->x += 0.5;
	game.player->y += 0.5;
	set_direction(&game, 0);
	game.win = mlx_new_window(game.mlx, game.screen_width, game.screen_height, "Raycasting 3D");
	init_img(&game);
	mlx_mouse_move(game.mlx, game.win, game.screen_width / 2, game.screen_height / 2);
	// mlx_mouse_hide(game.mlx, game.win);
	mlx_hook(game.win, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.win, 6, 1L << 6, handle_mouse_move, &game);
	mlx_hook(game.win, 4, 1L << 2, handle_mouse_key, &game);
	mlx_hook(game.win, 33, 0, handle_close, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	return 0;
}
