/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:14:08 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/10 11:01:04 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_player2(t_game *game, int malloc)
{
	game->servers = NULL;
	game->player->stats->nb_kills = 0;
	gettimeofday(&game->player->stats->temps, NULL);
	game->player->stats->nb_degats = 0;
	game->player->stats->nb_hit = 0;
	game->player->stats->nb_shoot = 0;
	game->player->stats->distanc_travel = 0;
	game->player->animation = 0;
	game->music_dif = 0;
	game->player->selected_anim = 0;
	if (malloc)
	{
		pthread_mutex_init(&game->mutex, NULL);
		pthread_mutex_init(&game->mutex_music, NULL);
		init_player_keycode(game, malloc);
	}
}

void	init_player(t_game	*game, int malloc)
{
	if (malloc)
	{
		game->player = gc_malloc(game->mem, sizeof(t_player));
		game->player->raycast = gc_malloc(game->mem, sizeof(t_raycast));
		game->player->stats = gc_malloc(game->mem, sizeof(t_stats));
		game->player->invert_mouse_x = 0;
	}
	game->player->x = 0;
	game->player->y = 0;
	game->player->floor = 0;
	game->player->height = 0;
	game->player->dir_x = 1;
	game->player->dir_y = 0;
	game->player->plane_x = 0;
	game->player->anim_shoot = 0;
	game->player->is_shooting = 0;
	game->player->health = 1;
	game->player->life_up = 0;
	game->player->injured = 0;
	game->player->ammo = 30;
	game->player->picking_up_ammo = 0;
	game->player->being_tpted = 0;
	game->player->telep_signal = 0;
	game->time_regen = 0;
	init_player2(game, malloc);
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
