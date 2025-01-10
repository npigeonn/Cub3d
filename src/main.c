/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 09:54:03 by npigeon           #+#    #+#             */
/*   Updated: 2025/01/10 09:47:40 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
	init_var(game, 0);
	init_player(game, 0);
	init_menu(game, 0);
	init_client(game, 0);
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
