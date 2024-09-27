/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:59:56 by npigeon           #+#    #+#             */
/*   Updated: 2024/09/27 15:02:23 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/cub3d.h"




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
