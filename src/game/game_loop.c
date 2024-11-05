/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:35:59 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/05 13:04:40 by ybeaucou         ###   ########.fr       */
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
	if (game->menu->status == PLAYING)
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

void	draw_success(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;
	t_draw_info	info2;
	t_draw_info	info;

	info = init_draw_info(game->screen_height, "", 0, 0);
	info.width = game->screen_width;
	info.color = 13107400;
	draw_rectangle(game, info);
	mlx_mouse_show(game->mlx, game->win);
	info = init_draw_info(70, "MISSION SUCCEED", game->screen_width >> 1, game->screen_height * 0.4);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
	// info2 = init_draw_info(0, "", x - 4, y + 2 * (btn_height + spacing) - 4);
	// info2.width = btn_width + 8;
	// info2.height = btn_height + 8;
	// info2.color = MENU_BUTTON_SELECTED_COLOR;
	// info2.radius = 10;
	// if (game->menu->button_selected == 1)
	// 	draw_rounded_rectangle(game, info2);
	// info = init_draw_info(0, "", x, y + 2 * (btn_height + spacing));
	// info2 = init_draw_info(btn_height, "", x, y + 2 * (btn_height + spacing));
	// info2.color = MENU_BUTTON_COLOR;
	// info2.width = btn_width;
	// draw_rectangle(game, info2);
	// info.color = MENU_BUTTON_TEXT_COLOR;
	// ft_strcpy(info.str, "Main menu");
	// info.y = y + 2 * (btn_height + spacing) + btn_height * 0.33 - 5;
	// info.height = btn_height * 0.5;
	// info.x = x + btn_width / 2;
	// draw_text(game, info);
	// game->chatbox->visible = false;
	// game->chatbox->is_writting = false;
}

static void	victory_screen(t_game *game)
{
	t_player	*p;

	p = game->player;
	if (p->health <= 0)
		game->menu->status = GAME_OVER;
	if (game->map[p->floor][(int)p->y][(int)p->x] == 'e')
	{
		// ft_printf("VICTORY\n");
		game->menu->status = GAME_SUCCESS;
		// exit(EXIT_SUCCESS);
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
	else if (status == OPTIONS_KEYBOARD || status == OPTIONS_MOUSE || status == OPTIONS_SOUND)
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
			apply_fade_to(game, RED);
		else
			draw_game_over(game);
	}
	if (status == GAME_SUCCESS)
	{
		if (game->fade_progress < 1)
			apply_fade_to(game, WHITE);
		else
			draw_success(game);
	}
	else if (status == VALID_SERVER_CREATE || status == VALID_JOIN_SERVER)
		create_join_server(game);
	mlx_put_image_to_window(game->mlx, game->win, game->images->base->img,
		0, 0);
	victory_screen(game);
	return (0);
}
