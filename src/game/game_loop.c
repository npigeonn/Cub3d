/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:35:59 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/24 17:08:13 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../includes/cub3d.h"

static void	draw_hud(t_game *game)
{
	head_up_display(game);
	mini_map(game);
	crosshair(game);
	gun_draw(game);
	if (game->player->being_tpted)
		animation_teleportation(game);
	calculate_fps(game);
}

static void	game_engine(t_game *game)
{
	t_player	*p;

	p = game->player;
	game->menu->message = NOTHING;
	handle_key(game);
	calculate_delta_time(game);
	cast_rays(game);
	cast_floor(game);
	draw_players(game);
	if (game->menu->status != GAME_OVER)
	{
		update_door_animation(game);
		update_enemies(game);
		on_ammo(game);
		update_projectiles(game);
		on_life(game);
	}
	draw_sprites(game);
	chat_draw(game);
	show_message(game);
	draw_hud(game);
	if (is_a_teleporter(game->map[p->floor][(int)p->y][(int)p->x]))
		game->menu->message = TELEPORT;
	if (game->menu->message != NOTHING)
		show_menu_message(game);
}

static void	create_join_server(t_game *game)
{
	if (game->menu->status == VALID_SERVER_CREATE)
	{
		create_server(game);
		ft_strcpy(game->client->ip, "127.0.0.1");
		game->menu->status = VALID_JOIN_SERVER;
	}
	else if (game->menu->status == VALID_JOIN_SERVER)
	{
		if (join_server(game) == 1)
			game->menu->status = MULTI_PLAYER;
	}
}

static void	victory_screen(t_game *game)
{
	t_player	*p;

	p = game->player;
	if (p->health <= 0)
		game->menu->status = GAME_OVER;
	if (game->map[p->floor][(int)p->y][(int)p->x] == 'e')
	{
		ft_printf("VICTORY\n");
		exit(EXIT_SUCCESS);
	}
}

int	game_loop(t_game *game)
{
	int	status;

	status = game->menu->status;
	ft_memset(game->images->base->data, 0, game->screen_width
		* game->screen_height * 4);
	if (status == GET_PSEUDO)
		draw_get_pseudo_menu(game);
	else if (status == MAIN_MENU)
		draw_main_menu(game);
	else if (status == OPTIONS)
		draw_options_menu(game);
	else if (status == SERVERS)
		draw_multiplayer_menu(game);
	else if (status == SERVER_CREATE)
		draw_create_server_menu(game);
	else if (status == JOIN_SERVER)
		draw_join_server_menu(game);
	else if (status == SERVER_DISCONNECTED || status == SERVER_FULL)
		draw_server_error_menu(game);
	else if (status == STATS)
		draw_stats_menu(game);
	else if (status == PLAYING || status == MULTI_PLAYER || status == CHATING || (status == GAME_OVER && game->fade_progress < 1))
		game_engine(game);
	if (status == GAME_OVER)
	{
		if (game->fade_progress < 1)
			apply_fade_to_red(game);
		else
			draw_game_over(game);
	}
	else if (status == VALID_SERVER_CREATE || status == VALID_JOIN_SERVER)
		create_join_server(game);
	mlx_put_image_to_window(game->mlx, game->win, game->images->base->img,
		0, 0);
	victory_screen(game);
	return (0);
}
