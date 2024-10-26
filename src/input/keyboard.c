/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 02:43:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/25 23:45:39 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	check_mouvement(t_game *game, t_player *p, double new_x,
	double new_y)
{
	if (!can_move(game, new_x, new_y, game->player->floor))
		return ;
	p->stats->distanc_travel += sqrt(pow(new_x - p->x, 2) + pow(new_y - p->y, 2));
	p->x = new_x;
	p->y = new_y;
}

static void	mouvement(t_game *game, t_player *p)
{
	double	new_x;
	double	new_y;

	new_x = p->x;
	new_y = p->y;
	if (is_key_pressed(game, KEY_UP) || is_key_pressed(game, KEY_W))
	{
		new_x += p->dirX * 0.1;
		new_y += p->dirY * 0.1;
	}
	if (is_key_pressed(game, KEY_DOWN) || is_key_pressed(game, KEY_S))
	{
		new_x -= p->dirX * 0.1;
		new_y -= p->dirY * 0.1;
	}
	if (is_key_pressed(game, KEY_RIGHT) || is_key_pressed(game, KEY_D))
	{
		new_x += p->planeX * 0.1;
		new_y += p->planeY * 0.1;
	}
	if (is_key_pressed(game, KEY_LEFT) || is_key_pressed(game, KEY_A))
	{
		new_x -= p->planeX * 0.1;
		new_y -= p->planeY * 0.1;
	}
	check_mouvement(game, p, new_x, new_y);
}

void	send_update_position(t_game *game)
{
	t_game_message	msg;

	msg.type = MSG_MOVE;
	msg.player_id = game->client->player_id;
	msg.x = game->player->x;
	msg.y = game->player->y;
	msg.floor = game->player->floor;
	msg.height = game->player->height;
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

int	handle_key_press(int keycode, t_game *game)
{
	t_player	*p;
	const int	status = game->menu->status;
	
	p = game->player;
	if (keycode == 65307)
		handle_close(game);
	else if (status == SERVER_CREATE || status == JOIN_SERVER)
		handle_text_input(game, keycode);
	if (status == CHATING)
		chat_input(game, keycode);
	if (keycode == 116 && (status == MULTI_PLAYER || status == CHATING)
		&& !game->chatbox->is_writting)
		chatting_mode(game);
	if (is_keyflag(keycode))
		set_key_flag(game, keycode, 1);
	if (status == GET_PSEUDO)
		handle_pseudo_input(game, keycode);
	if (status != PLAYING && status != MULTI_PLAYER)
		return (0);
	if (keycode == 32)
		p->height -= 0.1;
	if (keycode == 98)
		p->height += 0.1;
	if (keycode == 102)
		use_item(game);
	return (0);
}

int	handle_key_release(int keycode, t_game *game)
{
	if (is_keyflag(keycode))
		set_key_flag(game, keycode, 0);
	return (0);
}
