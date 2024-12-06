/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 09:54:03 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/27 15:09:30 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	destroy_mlx_img(t_block_info *params)
{
	mlx_destroy_image(params->ptr, params->ptr2);
}

void	destroy_mlx_display(t_block_info *params)
{
	mlx_destroy_display(params->ptr);
	free(params->ptr);
}

void	load_texture(t_game *game, t_image *img, char *path)
{
	t_block_info	*param;

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
	param = gc_malloc(game->mem, sizeof(t_block_info));
	param->ptr = game->mlx;
	param->ptr2 = img->img;
	gc_add_memory_block(game->mem, img->img, destroy_mlx_img, param);
	gc_free(game->mem, param);
}

void	init_img(t_game *game)
{
	t_image	*base;

	game->images = gc_malloc(game->mem, sizeof(t_images));
	game->images->alphanum_sprite = gc_malloc(game->mem, sizeof(t_image));
	base = gc_malloc(game->mem, sizeof(t_image));
	base->img = mlx_new_image(game->mlx, game->screen_width,
			game->screen_height);
	base->data = mlx_get_data_addr(base->img, &base->bpp, &base->size_line,
			&base->endian);
	game->images->base = base;
	load_texture(game, game->images->alphanum_sprite,
		"./assets/sprites/alphanum_sprite.xpm");
	game->textures->bullet = gc_malloc(game->mem, sizeof(t_image));
	load_texture(game, game->textures->bullet, "./assets/sprites/bullet.xpm");
	game->textures->weapon = gc_malloc(game->mem, sizeof(t_image));
	load_texture(game, game->textures->weapon,
		"./assets/sprites/weapons/PISGE0.xpm");
	game->textures->fire = gc_malloc(game->mem, sizeof(t_image));
	load_texture(game, game->textures->fire,
		"assets/sprites/weapons/PISFA0.xpm");
	game->textures->ammo = gc_malloc(game->mem, sizeof(t_image));
	load_texture(game, game->textures->ammo, "./assets/sprites/ammos.xpm");
	game->textures->ammo->nb_sprite = 1;
	game->textures->ammo->sprite_height = game->textures->ammo->height;
	game->textures->ammo->sprite_width = game->textures->ammo->width;
}

void	init_floorcast(t_game *game, t_floorcast *f)
{
	if (game->textures->color_f < 0)
	{
		f->f_tex_width = game->textures->floor->width;
		f->f_tex_height = game->textures->floor->height;
		f->f_tex_data = game->textures->floor->data;
		f->f_bpp = game->textures->floor->bpp / 8;
	}
	if (game->textures->color_c < 0)
	{
		f->c_tex_width = game->textures->ceil->width;
		f->c_tex_height = game->textures->ceil->height;
		f->c_tex_data = game->textures->ceil->data;
		f->c_bpp = game->textures->ceil->bpp / 8;
	}
}

void	load_game_texture_malloc(t_game *game)
{
	t_block_info	*param;

	param = gc_malloc(game->mem, sizeof(t_block_info));
	param->ptr = game->mlx;
	param->ptr2 = NULL;
	gc_add_memory_block(game->mem, game->mlx, destroy_mlx_display, param);
	gc_free(game->mem, param);
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

void	load_game_texture2(t_game *game)
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
	set_direction(game, game->player->begin_dir);
	game->menu->status = MAIN_MENU;
}

void	hooks(t_game *game)
{
	mlx_mouse_move(game->mlx, game->win, game->screen_width >> 1,
		game->screen_height >> 1);
	mlx_hook(game->win, 6, 1L << 6, handle_mouse_move, game);
	mlx_hook(game->win, 4, 1L << 2, handle_mouse_key_press, game);
	mlx_hook(game->win, 5, 1L << 3, handle_mouse_key_release, game);
	mlx_hook(game->win, 33, 0, handle_close, game);
	mlx_hook(game->win, 2, 1L << 0, handle_key_press, game);
	mlx_hook(game->win, 3, 1L << 1, handle_key_release, game);
	mlx_loop_hook(game->mlx, game_loop, game);
	mlx_loop(game->mlx);
}

void	x_fixes_cursor(t_game *game, char to_do)
{
	if (to_do == 'h' && game->mouse)
	{
		XFixesHideCursor(((t_xvar *)(game->mlx))->display,
			((t_win_list *)(game->win))->window);
		game->mouse = 0;
		return ;
	}
	else if ((to_do == 'h' && !game->mouse)
		|| (to_do == 's' && game->mouse))
		return ;
	else if (to_do == 's' && !game->mouse)
	{
		XFixesShowCursor(((t_xvar *)(game->mlx))->display,
			((t_win_list *)(game->win))->window);
		game->mouse = 1;
		return ;
	}
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
