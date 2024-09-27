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






// int	handle_close(t_data *img)
// {
// 	mlx_destroy_image(img->mlx, img->img);
// 	mlx_destroy_window(img->mlx, img->window);
// 	mlx_destroy_display(img->mlx);
// 	free(img->mlx);
// 	exit(0);
// }

// int	mouse_move(int mousecode, int x, int y, t_data *img)
// {
// 	// if (mousecode == 1)
// 	//	//tirer (3 = tire speciaux?)
	
// 	// if (mousecode == 4)
// 	// 	img->zoom *= 0.9;
// 	// if (mousecode == 5)
// 	// 	img->zoom *= 1.11111;
// 	// if (mousecode == 4 || mousecode == 5)
// 	// {
// 	// 	img->x_offset += img->zoom * 4 * (x - WIDTH * 0.5) / WIDTH;
// 	// 	img->y_offset += img->zoom * 4 * (y - HEIGHT * 0.5) / HEIGHT;
// 	// }
// 	mlx_clear_window(img->mlx, img->window);
// 	crafting_image(img);
// 	mlx_put_image_to_window(img->mlx, img->window, img->img, 0, 0);
// 	return (0);
// }

// int	key_move(int keycode, t_data *img)
// {
// 	// printf("%d\n", keycode);
// 	if (keycode == 65361 || keycode == 97)
// 		img->x_offset -= img->zoom;
// 	if (keycode == 65362 || keycode == 119)
// 		img->y_offset -= img->zoom;
// 	if (keycode == 65363 || keycode == 100)
// 		img->x_offset += img->zoom;
// 	if (keycode == 65364 || keycode == 115)
// 		img->y_offset += img->zoom;
// 	if (keycode == 65307)
// 	{
// 		mlx_destroy_image(img->mlx, img->img);
// 		mlx_destroy_window(img->mlx, img->window);
// 		mlx_destroy_display(img->mlx);
// 		free(img->mlx);
// 		exit(0);
// 	}
// 	mlx_clear_window(img->mlx, img->window);
// 	// crafting_image(img);
// 	mlx_put_image_to_window(img->mlx, img->window, img->img, 0, 0);
// 	return (0);
// }

// int	main(int argc, char **argv)
// {
// 	t_data	img;

// 	img.color = 0;
// 	img.fct = 0;
// 	if (!verifglobal(argc, argv, &img))
// 		return (0);
// 	img.mlx = mlx_init();
// 	img.window = mlx_new_window(img.mlx, WIDTH, HEIGHT, "Cub3D");
// 	img.img = mlx_new_image(img.mlx, WIDTH, HEIGHT);
// 	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, \
// 			&img.line_length, &img.endian);
// 	img.x_offset = 0;
// 	img.y_offset = 0;
// 	img.zoom = 1;
// 	// crafting_image(&img);
// 	mlx_hook(img.window, 33, 0, handle_close, &img);
// 	mlx_hook(img.window, 2, 1L << 0, key_move, &img);
// 	mlx_hook(img.window, 4, 1L << 2, mouse_move, &img);
// 	mlx_put_image_to_window(img.mlx, img.window, img.img, 0, 0);
// 	mlx_loop(img.mlx);
// 	return (0);
// }



#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/cub3d.h"



#define TILE_SIZE 50
#define MAP_WIDTH 640
#define MAP_HEIGHT 480

typedef struct {
    void *img;
    int width;
    int height;
} t_texture;

typedef struct {
    void *mlx;
    void *win;
	t_player	*player;
    t_texture wall_texture;
    t_texture floor_texture;
    t_texture player_texture;
    char map[MAP_HEIGHT][MAP_WIDTH + 1];
} t_game;

void draw_texture(t_game *game, t_texture *texture, int x, int y) {
    mlx_put_image_to_window(game->mlx, game->win, texture->img, x * TILE_SIZE, y * TILE_SIZE);
}

void draw_map(t_game *game) {
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (game->map[y][x] == '1') {
                draw_texture(game, &game->wall_texture, x, y); // Texture du mur
            } else if (game->map[y][x] == '0' || game->map[y][x] == 'D') {
                draw_texture(game, &game->floor_texture, x, y); // Texture du sol
            }
        }
    }
    // Afficher le joueur
    draw_texture(game, &game->player_texture, game->player->x, game->player->y); // Texture du joueur
}

int can_move(t_game *game, int new_x, int new_y) {
    if (new_x >= 0 && new_x < MAP_WIDTH && new_y >= 0 && new_y < MAP_HEIGHT &&
        (game->map[new_y][new_x] == '0' || game->map[new_y][new_x] == 'D')) {
        return 1;
    }
    return 0;
}

int key_hook(int keycode, t_game *game) {
	if (keycode == 65307) // Touche ESC pour quitter
		exit(0);
	if (keycode == 100 && can_move(game, game->player->x, game->player->y - 1)) // W - Haut
		game->player->y--;
	if (keycode == 115 && can_move(game, game->player->x, game->player->y + 1)) // S - Bas
		game->player->y++;
	if (keycode == 119 && can_move(game, game->player->x - 1, game->player->y)) // A - Gauche
		game->player->x--;
	if (keycode == 97 && can_move(game, game->player->x + 1, game->player->y)) // D - Droite
		game->player->x++;
		
	mlx_clear_window(game->mlx, game->win);
	// draw_map(game);
	raycast(game->player, game->mlx, game->win);
	return 0;
}

void load_texture(t_game *game, t_texture *texture, char *path) {
    texture->img = mlx_xpm_file_to_image(game->mlx, path, &texture->width, &texture->height);
    if (!texture->img) {
        printf("Erreur lors du chargement de la texture : %s\n", path);
        exit(1);
    }
}

int main() {
    t_game game;

    // Initialisation de la carte
    // char map[MAP_HEIGHT][MAP_WIDTH + 1] = {
    //     "11111111111111",
    //     "10000000011111",
    //     "10000000010001",
    //     "10000N000D0001",
    //     "10000000010001",
    //     "11111111111111"
    // };
	game.player = malloc(sizeof(t_player));
    ft_bzero(game.player, sizeof(t_player));
	// for (int i = 0; i < MAP_HEIGHT; i++)
    //     for (int j = 0; j < MAP_WIDTH; j++)
    //         game.map[i][j] = map[i][j];
    // // Initialiser la position du joueur (trouve la position N)
    // for (int y = 0; y < MAP_HEIGHT; y++) {
    //     for (int x = 0; x < MAP_WIDTH; x++) {
    //         if (game.map[y][x] == 'N') {
    //             game.player->x = x;
    //             game.player->y = y;
    //         }
    //     }	
    // }

	
	game.player->x = 22;
	game.player->y = 12;
	game.player->px = 0.66;

	game.mlx = mlx_init();
    game.win = mlx_new_window(game.mlx, 640, 480, "Mini Map with Textures");

    // Charger les textures (mettre les chemins vers vos fichiers XPM)
    // load_texture(&game, &game.player_texture, "../assets/player.xpm"); // Texture du joueur
    // load_texture(&game, &game.floor_texture, "../assets/floor.xpm"); // Texture du sol
    // load_texture(&game, &game.wall_texture, "../assets/wall.xpm"); // Texture du mur
    
    // draw_map(&game);

    mlx_key_hook(game.win, key_hook, &game);
    mlx_loop(game.mlx);
    return 0;
}