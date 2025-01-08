/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 02:43:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/02 20:28:14 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	send_update_position(t_game *game)
{
	t_game_message	msg;

	ft_bzero(&msg, sizeof(t_game_message));
	msg.type = MSG_MOVE;
	msg.player_id = game->client->player_id;
	msg.x = game->player->x;
	msg.y = game->player->y;
	msg.floor = game->player->floor;
	msg.health = game->player->health;
	msg.plane_x = game->player->plane_x;
	msg.plane_y = game->player->plane_y;
	msg.dir_x = game->player->dir_x;
	msg.dir_y = game->player->dir_y;
	msg.selected_anim = game->player->selected_anim;
	ft_strcpy(msg.pseudo, game->client->pseudo);
	send(game->client->sock, &msg, sizeof(t_game_message), 0);
}

void	handle_key(t_game *game)
{
	const int	status = game->menu->status;

	if (status == PLAYING || status == MULTI_PLAYER)
		mouvement(game, game->player);
	if (status == MULTI_PLAYER)
		send_update_position(game);
}

void	check_use_item(t_game *game, int keycode, t_player *p)
{
	if ((keycode == game->player->key->use
			|| keycode == game->player->key->use2) && p->health > 0)
		use_item(game);
}

int	handle_key_press(int keycode, t_game *game)
{
	t_player	*p;
	const int	status = game->menu->status;

	p = game->player;
	if (keycode == game->player->key->escape
		|| keycode == game->player->key->escape2)
		handle_close(game);
	else if (status == OPTIONS_KEYBOARD)
		update_option_menu_key_keyboard(game, keycode);
	else if (status == SERVER_CREATE || status == JOIN_SERVER)
		handle_text_input(game, keycode);
	if (status == CHATING)
		chat_input(game, keycode);
	if ((keycode == game->player->key->chat
			|| keycode == game->player->key->chat2) && (status == MULTI_PLAYER
			|| status == CHATING) && !game->chatbox->is_writting)
		chatting_mode(game);
	if (is_keyflag(game, keycode))
		set_key_flag(game, keycode, 1);
	if (status == GET_PSEUDO)
		handle_pseudo_input(game, keycode);
	if (status != PLAYING && status != MULTI_PLAYER)
		return (0);
	check_use_item(game, keycode, p);
	return (0);
}

int	handle_key_release(int keycode, t_game *game)
{
	if (is_keyflag(game, keycode))
		set_key_flag(game, keycode, 0);
	return (0);
}
