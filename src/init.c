/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:18:40 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/24 10:11:36 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// void	init_player(t_game	*game)
// {
// 	game->player = gc_malloc(game->mem, sizeof(t_player));
// 	game->player->x = 0;
// 	game->player->y = 0;
// 	game->player->floor = 0;
// 	game->player->height = 0;
// 	game->player->dirX = 1;
// 	game->player->dirY = 0;
// 	game->player->planeX = 0;
// 	game->player->anim_shoot = 0;
// 	game->player->health = 1;
// 	game->player->ammo = 30;
// 	game->timehealth = 0;
// 	game->servers = NULL;
// 	game->player->raycast = gc_malloc(game->mem, sizeof(t_raycast));
// }

// void	init_client(t_game *game)
// {
// 	game->client = gc_malloc(game->mem, sizeof(t_client));
// 	game->client->pseudo[0] = '\0';
// 	game->client->players = NULL;
// 	game->client->name[0] = '\0';
// 	game->client->pseudo[0] = '\0';
// 	game->client->ip[0] = '\0';
// }

// void	init_menu(t_game *game)
// {
// 	game->menu = gc_malloc(game->mem, sizeof(t_menu));
// 	game->menu->volume = 20;
// 	game->menu->mouse_sensitivity = 2;
// 	game->menu->message = NOTHING;
// 	game->menu->status = MAIN_MENU;
// 	game->menu->button_selected = 0;
// 	game->menu->error_name = false;
// 	game->menu->error_pseudo = false;
// 	game->menu->text_field_selected = 0;
// }

// void	init_chatbox(t_game *game)
// {
// 	game->chatbox = gc_malloc(game->mem, sizeof(t_chatbox));
// 	game->chatbox->visible = false;
// 	game->chatbox->message[0] = '\0';
// 	game->chatbox->is_writting = false;
// 	game->chatbox->messages = NULL;
// }

// void	init_img(t_game *game)
// {
// 	t_image	*base;

// 	game->images = gc_malloc(game->mem, sizeof(t_images));
// 	game->images->alphanum_sprite = gc_malloc(game->mem, sizeof(t_image));
// 	base = gc_malloc(game->mem, sizeof(t_image));
// 	base->img = mlx_new_image(game->mlx, game->screen_width, game->screen_height);
// 	base->data = mlx_get_data_addr(base->img, &base->bpp, &base->size_line, &base->endian);
// 	game->images->base = base;
// 	load_texture(game, game->images->alphanum_sprite, "./assets/sprites/alphanum_sprite.xpm");
// 	game->textures->bullet = gc_malloc(game->mem, sizeof(t_image));
// 	load_texture(game, game->textures->bullet, "./assets/sprites/bullet.xpm");
// 	game->textures->weapon = gc_malloc(game->mem, sizeof(t_image));
// 	load_texture(game, game->textures->weapon, "./assets/sprites/weapons/PISGE0.xpm");
// 	game->textures->fire = gc_malloc(game->mem, sizeof(t_image));
// 	load_texture(game, game->textures->fire, "assets/sprites/weapons/PISFA0.xpm");
// 	game->textures->ammo = gc_malloc(game->mem, sizeof(t_image));
// 	load_texture(game, game->textures->ammo, "./assets/sprites/ammos.xpm");
// 	game->textures->ammo->nb_sprite = 1;
// 	game->textures->ammo->sprite_height = game->textures->ammo->height;
// 	game->textures->ammo->sprite_width = game->textures->ammo->width;
// }

// void	init_floorcast(t_game *game)
// {
// 	t_floorcast	floorcast;

// 	if (game->textures->color_f < 0)
// 	{
// 		floorcast.f_tex_width = game->textures->floor->width;
// 		floorcast.f_tex_height = game->textures->floor->height;
// 		floorcast.f_tex_data = game->textures->floor->data;
// 		floorcast.f_bpp = game->textures->floor->bpp / 8;
// 	}
// 	if (game->textures->color_c < 0)
// 	{
// 		floorcast.c_tex_width = game->textures->ceil->width;
// 		floorcast.c_tex_height = game->textures->ceil->height;
// 		floorcast.c_tex_data = game->textures->ceil->data;
// 		floorcast.c_bpp = game->textures->ceil->bpp / 8;
// 	}
// 	game->player->raycast->floorcast = floorcast;
// }
