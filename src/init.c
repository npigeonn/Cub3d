/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:18:40 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/02 20:14:03 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_var(t_game *game, int malloc)
{
	game->sprites = NULL;
	game->door = NULL;
	game->key_flags = 0;
	game->projectiles = NULL;
	game->cen_x = game->screen_width * .5;
	game->cen_y = game->screen_height * .5;
	game->half_cen_x = game->screen_width * .5 * .5;
	game->half_cen_y = game->screen_height * .5 * .5;
	game->fade_progress = 0;
	if (malloc)
		game->wall_distances = gc_malloc(game->mem, sizeof(float)
				* game->screen_width);
	ft_bzero(game->wall_distances, game->screen_width);
	gettimeofday(&game->last_time, NULL);
}

void	init_menu(t_game *game, int malloc)
{
	if (malloc)
	{
		game->menu = gc_malloc(game->mem, sizeof(t_menu));
		game->menu->music_volume = 30;
		game->menu->effects_volume = 30;
		game->menu->menu_music_volume = 30;
		game->menu->mouse_sensitivity = 20;
	}
	game->menu->message = NOTHING;
	game->menu->status = GET_PSEUDO;
	game->menu->button_selected = 0;
	game->menu->error_name = false;
	game->menu->error_pseudo = false;
	game->menu->text_field_selected = 0;
	game->menu->dragging = false;
	game->menu->scroll = 0;
}

void	init_client(t_game *game, int malloc)
{
	if (malloc)
	{
		game->client = gc_malloc(game->mem, sizeof(t_client));
		game->client->pseudo[0] = '\0';
	}
	game->client->name[0] = '\0';
	game->client->ip[0] = '\0';
	if (malloc)
		game->chatbox = gc_malloc(game->mem, sizeof(t_chatbox));
	game->chatbox->visible = false;
	game->chatbox->message[0] = '\0';
	game->chatbox->is_writting = false;
	game->chatbox->messages = NULL;
}

void	init_player_keycode(t_game *game, int malloc)
{
	if (malloc)
		game->player->key = gc_malloc(game->mem, sizeof(t_keycode));
	else
		return ;
	game->player->key->up = KEY_W;
	game->player->key->down = KEY_S;
	game->player->key->left = KEY_A;
	game->player->key->right = KEY_D;
	game->player->key->up2 = KEY_UP;
	game->player->key->down2 = KEY_DOWN;
	game->player->key->left2 = KEY_LEFT;
	game->player->key->right2 = KEY_RIGHT;
	game->player->key->use = KEY_F;
	game->player->key->use2 = NULL;
	game->player->key->escape = KEY_ESC;
	game->player->key->escape2 = NULL;
	game->player->key->pause = KEY_P;
	game->player->key->pause2 = NULL;
	game->player->key->chat = KEY_T;
	game->player->key->chat2 = NULL;
}

void	init_player2(t_game *game, int malloc)
{
	game->servers = NULL;
	game->player->stats->nb_kills = 0;
	game->player->stats->temps = gettimeofday(NULL, NULL);
	game->player->stats->nb_degats = 0;
	game->player->stats->nb_hit = 0;
	game->player->stats->nb_shoot = 0;
	game->player->stats->distanc_travel = 0;
	game->player->animation = 0;
	game->music_dif = 0;
	pthread_mutex_init(&game->mutex, NULL);
	pthread_mutex_init(&game->mutex_music, NULL);
	game->player->selected_anim = 0;
	init_player_keycode(game, malloc);
}
