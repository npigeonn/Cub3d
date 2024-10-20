/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 02:43:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/19 14:32:59 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	check_mouvement(t_game *game, t_player *p, double new_x,
	double new_y)
{
	if (!can_move(game, new_x, new_y))
		return ;
	p->x = new_x;
	p->y = new_y;
}

static void	mouvement(t_game *game, t_player *p, int keycode)
{
	double	new_x;
	double	new_y;

	if (keycode == KEY_UP || keycode == KEY_W)
	{
		new_x = p->x + p->dirX * 0.1;
		new_y = p->y + p->dirY * 0.1;
	}
	else if (keycode == KEY_DOWN || keycode == KEY_S)
	{
		new_x = p->x - p->dirX * 0.1;
		new_y = p->y - p->dirY * 0.1;
	}
	else if (keycode == KEY_RIGHT || keycode == KEY_D)
	{
		new_x = p->x + p->planeX * 0.1;
		new_y = p->y + p->planeY * 0.1;
	}
	else if (keycode == KEY_LEFT || keycode == KEY_A)
	{
		new_x = p->x - p->planeX * 0.1;
		new_y = p->y - p->planeY * 0.1;
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
	strncpy(msg.pseudo, game->client->pseudo, MAX_PSEUDO_LENGTH);
	send(game->client->sock, &msg, sizeof(t_game_message), 0);
}

int	handle_keypress(int keycode, t_game *game)
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
	if (status != PLAYING && status != MULTI_PLAYER)
		return (0);
	mouvement(game, p, keycode);
	if (keycode == 32)
		p->height -= 0.1;
	if (keycode == 98)
		p->height += 0.1;
	if (keycode == 102)
		use_item(game);
	if (status == MULTI_PLAYER)
		send_update_position(game);
	return (0);
}
