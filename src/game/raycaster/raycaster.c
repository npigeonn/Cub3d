/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:46:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/14 11:14:47 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	handle_close(t_game *game)
{
	if (game->client->sock > 0)
	{
		close(game->client->sock);
		game->client->sock = -1;
		pthread_join(game->client->thread, NULL);
	}
	if (game->server)
	{
		game->server->stop = true;
		pthread_join(game->server->logic_game_thread, NULL);
		pthread_join(game->server->main_server_thread, NULL);
		pthread_join(game->server->broadcast_thread, NULL);
	}
	destroy_thread_pool(game->pool);
	mlx_destroy_image(game->mlx, game->images->base->img);
	mlx_destroy_window(game->mlx, game->win);
	pthread_mutex_lock(&game->mutex_music);
	game->is_running = false;
	pthread_mutex_unlock(&game->mutex_music);
	pthread_mutex_destroy(&game->mutex_music);
	gc_exit(game->mem, 0);
	return (0);
}

void	draw_menu_message(t_game *game, const char *action)
{
	t_draw_info	info;
	const int	width = 350;
	const int	height = 40;
	const int	text_x = (game->screen_width - width) * 0.5;
	const int	text_y = (game->screen_height - height) * 0.5 - 120;

	info = init_draw_info(40, "", text_x, text_y);
	info.width = width;
	info.height = height;
	info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, info);
	ft_strcpy(info.str, "Press ");
	ft_strlcat(info.str, get_key_name(game, game->player->key->use), 256);
	ft_strlcat(info.str, action, 256);
	info.height = 30;
	info.y = game->cen_y - 129;
	info.x = game->cen_x;
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	show_menu_message(t_game *game)
{
	if (game->menu->message == OPEN_DOOR)
		draw_menu_message(game, " to open");
	else if (game->menu->message == CLOSE_DOOR)
		draw_menu_message(game, " to close");
	else if (game->menu->message == TELEPORT)
		draw_menu_message(game, " to teleport");
}

void	calculate_delta_time(t_game *game)
{
	struct timeval	current_time;
	float			seconds;

	gettimeofday(&current_time, NULL);
	seconds = (current_time.tv_sec - game->last_time.tv_sec)
		+ (current_time.tv_usec - game->last_time.tv_usec) / 1000000.0f;
	game->delta_time = seconds;
	game->last_time = current_time;
}
