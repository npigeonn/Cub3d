/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:35:59 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/12/10 12:32:41 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	game_loop2(t_game *game, int status)
{
	if (status == PLAYING || status == MULTI_PLAYER || status == CHATING
		|| (status == GAME_OVER && game->fade_progress < 1))
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
	else if (status == OPTIONS_KEYBOARD || status == OPTIONS_MOUSE
		|| status == OPTIONS_SOUND)
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
	game_loop2(game, status);
	return (0);
}
