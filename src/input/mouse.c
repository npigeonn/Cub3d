/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 02:35:22 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/22 08:52:49 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	handle_mouse_key(int keycode, int x, int y, t_game *game)
{
	if (game->menu->status == OPTIONS)
		update_option_menu_click(game, x, y, keycode);
	else if (game->menu->status == SERVERS)
		update_multiplayer_click(game, x, y, keycode);
	else if (game->menu->status == SERVER_CREATE)
		update_create_server_menu_text(game, x, y, keycode);
	else if (game->menu->status == JOIN_SERVER)
		update_join_server_menu_text(game, x, y, keycode);
	else if (game->menu->status == MAIN_MENU)
		update_main_menu_click(game, x, y, keycode);
	else if (game->menu->status == SERVER_DISCONNECTED
		|| game->menu->status == SERVER_FULL)
		update_server_error_click(game, x, y, keycode);
	else if (game->menu->status == CHATING)
		handle_mouse_chat(game, x, y, keycode);
	else if ((game->menu->status == PLAYING
			|| game->menu->status == MULTI_PLAYER)
		&& keycode == 1 && game->player->ammo > 0)
		return (game->player->anim_shoot = 1, game->player->ammo--, 0);
	return (0);
}

static void	handle_mouse_game(t_game *game, int x, int y)
{
	const int	center_x = game->screen_width * 0.5;
	const float	rotation = (x - center_x) * ROTATION_SPEED;
	const float	old_dir_x = game->player->dirX;
	const float	old_plane_x = game->player->planeX;
	t_player	*p;

	p = game->player;
	p->dirX = old_dir_x * cos(rotation) - p->dirY * sin(rotation);
	p->dirY = old_dir_x * sin(rotation) + p->dirY * cos(rotation);
	p->planeX = old_plane_x * cos(rotation) - p->planeY * sin(rotation);
	p->planeY = old_plane_x * sin(rotation) + p->planeY * cos(rotation);
	mlx_mouse_move(game->mlx, game->win, center_x, game->screen_height * 0.5);
}

int	handle_mouse_move(int x, int y, t_game *game)
{
	if (game->menu->status == MAIN_MENU)
		update_main_menu_button(game, x, y);
	else if (game->menu->status == OPTIONS)
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
	if (game->menu->status == PLAYING || game->menu->status == MULTI_PLAYER)
		handle_mouse_game(game, x, y);
	return (0);
}
