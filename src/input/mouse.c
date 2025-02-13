/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 02:35:22 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 16:15:24 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	handle_mouse_key_release(int keycode, int x, int y, t_game *game)
{
	if (keycode == 1)
	{
		set_key_flag(game, keycode, 0);
		game->menu->dragging = false;
	}
	(void)x;
	(void)y;
	return (0);
}

int	handle_mouse_key_press2(int keycode, int x, int y, t_game *game)
{
	const int		status = game->menu->status;
	t_game_message	msg;

	if ((status == PLAYING || status == MULTI_PLAYER)
		&& keycode == 1 && game->player->ammo > 0 && game->player->health > 0)
	{
		game->player->anim_shoot = 1;
		game->player->is_shooting = 1;
		game->player->ammo--;
		game->player->stats->nb_shoot++;
		if (game->menu->status == MULTI_PLAYER)
		{
			msg.type = MSG_PLAYER_SHOOT;
			msg.player_id = game->client->player_id;
			msg.x = game->player->x;
			msg.y = game->player->y;
			msg.dir_x = game->player->dir_x;
			msg.dir_y = game->player->dir_y;
			msg.floor = game->player->floor;
			send(game->client->sock, &msg, sizeof(t_game_message), 0);
		}
	}
	else if (status == STATS)
		update_stats_menu_click(game, x, y, keycode);
	return (0);
}

int	handle_mouse_key_press(int keycode, int x, int y, t_game *game)
{
	const int	status = game->menu->status;

	if (status == OPTIONS_KEYBOARD || status == OPTIONS_MOUSE
		|| status == OPTIONS_SOUND)
		update_option_menu_click(game, x, y, keycode);
	else if (status == SERVERS)
		update_multiplayer_click(game, keycode);
	else if (status == SERVER_CREATE)
		update_create_server_menu_text(game, x, y, keycode);
	else if (status == JOIN_SERVER && keycode == 1)
		update_join_server_menu_text(game, x, y);
	else if (status == MAIN_MENU)
		update_main_menu_click(game, keycode);
	else if (status == SERVER_DISCONNECTED || status == SERVER_FULL)
		update_server_error_click(game, keycode);
	else if ((status == GAME_OVER || status == GAME_SUCCESS)
		&& game->fade_progress >= 1)
		update_game_over_click(game, keycode);
	else if (status == CHATING)
		handle_mouse_chat(game, x, y, keycode);
	else if (status == GET_PSEUDO)
		update_get_pseudo_click(game, x, y, keycode);
	return (handle_mouse_key_press2(keycode, x, y, game));
}

static void	handle_mouse_game(t_game *game, int x)
{
	float		rotation;
	const float	old_dir_x = game->player->dir_x;
	const float	old_plane_x = game->player->plane_x;
	t_player	*p;

	rotation = (x - game->cen_x) * ((game->menu->mouse_sensitivity / 20.0f)
			* ROTATION_SPEED);
	if (x == game->cen_x || game->player->health <= 0)
		return ;
	p = game->player;
	if (p->invert_mouse_x)
		rotation *= -1;
	p->dir_x = old_dir_x * cos(rotation) - p->dir_y * sin(rotation);
	p->dir_y = old_dir_x * sin(rotation) + p->dir_y * cos(rotation);
	p->plane_x = old_plane_x * cos(rotation) - p->plane_y * sin(rotation);
	p->plane_y = old_plane_x * sin(rotation) + p->plane_y * cos(rotation);
	if (game->menu->status == MULTI_PLAYER)
		send_update_position(game);
	mlx_mouse_move(game->mlx, game->win, game->cen_x,
		game->screen_height * 0.5);
}

int	handle_mouse_move(int x, int y, t_game *game)
{
	if (game->menu->status == MAIN_MENU)
		update_main_menu_button(game, x, y);
	else if (game->menu->status == OPTIONS_KEYBOARD
		|| game->menu->status == OPTIONS_MOUSE
		|| game->menu->status == OPTIONS_SOUND)
		update_option_menu_button(game, x, y);
	else if (game->menu->status == SERVERS)
		update_multiplayer_menu(game, x, y);
	else if (game->menu->status == SERVER_CREATE)
		update_create_server_menu_button(game, x, y);
	else if (game->menu->status == JOIN_SERVER)
		update_join_server_menu_button(game, x, y);
	else if (game->menu->status == SERVER_DISCONNECTED
		|| game->menu->status == SERVER_FULL)
		update_server_error_button(game, x, y);
	else if ((game->menu->status == GAME_OVER
			|| game->menu->status == GAME_SUCCESS) && game->fade_progress >= 1)
		update_game_over_button(game, x, y);
	else if (game->menu->status == GET_PSEUDO)
		update_get_pseudo_button(game, x, y);
	if (game->menu->status == PLAYING || game->menu->status == MULTI_PLAYER)
		handle_mouse_game(game, x);
	if (game->menu->status == STATS)
		update_stats_menu(game, x, y);
	return (0);
}
