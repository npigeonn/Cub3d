/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 09:54:03 by npigeon           #+#    #+#             */
/*   Updated: 2025/01/10 11:59:39 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	free_last_game_texture(t_game *game)
{
	mlx_destroy_image(game->mlx, game->textures->north->img);
	mlx_destroy_image(game->mlx, game->textures->south->img);
	mlx_destroy_image(game->mlx, game->textures->west->img);
	mlx_destroy_image(game->mlx, game->textures->east->img);
	mlx_destroy_image(game->mlx, game->textures->door->img);
	mlx_destroy_image(game->mlx, game->textures->tp->img);
	if (game->textures->color_f == -1)
		mlx_destroy_image(game->mlx, game->textures->floor->img);
	if (game->textures->color_c == -1)
		mlx_destroy_image(game->mlx, game->textures->ceil->img);
	// mlx_destroy_image(game->mlx, game->textures->enemy->img);
	// mlx_destroy_image(game->mlx, game->textures->enemy_fire->img);
	// mlx_destroy_image(game->mlx, game->textures->enemy_death->img);
	gc_free(game->mem, game->textures->north);
	gc_free(game->mem, game->textures->south);
	gc_free(game->mem, game->textures->west);
	gc_free(game->mem, game->textures->east);
	gc_free(game->mem, game->textures->door);
	gc_free(game->mem, game->textures->tp);
	gc_free(game->mem, game->textures->floor);
	gc_free(game->mem, game->textures->ceil);
	// gc_free(game->mem, game->textures->enemy);
	// gc_free(game->mem, game->textures->enemy_fire);
	// gc_free(game->mem, game->textures->enemy_death);
}

void	reset_game(t_game *game)
{
	t_sprite	*current;
	t_sprite	*next;

	current = game->sprites;
	while (current)
	{
		next = current->next;
		gc_free(game->mem, current);
		current = next;
	}
	free_last_game_texture(game);
	init_var(game, 0);
	init_player(game, 0);
	init_menu(game, 0);
	init_client(game, 0);
	// load_game_texture(game);
	free_map(game);
	parsing(game->av, game);
	init_img(game);
	set_direction(game, game->player->begin_dir);
	game->menu->status = MAIN_MENU;
}

int	main(int ac, char **av)
{
	t_game		game;

	(void)ac;
	game.mem = gc_init();
	game.mlx = mlx_init();
	game.av = av;
	mlx_get_screen_size(game.mlx, &game.screen_width, &game.screen_height);
	game.screen_height = 1080;
	game.screen_width = 1920;
	init_var(&game, 1);
	init_player(&game, 1);
	init_menu(&game, 1);
	init_client(&game, 1);
	load_game_texture(&game);
	parsing(av, &game);
	set_direction(&game, game.player->begin_dir);
	init_img(&game);
	set_width_all_letter(&game);
	game.win = mlx_new_window(game.mlx, game.screen_width,
			game.screen_height, "Cub3D");
	game.is_running = true;
	init_thread_pool(&game, 4);
	if (game.music_dif)
		music_launch(&game);
	return (hooks(&game), 0);
}
