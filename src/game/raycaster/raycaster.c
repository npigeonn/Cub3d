/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:46:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/10 09:24:44 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	handle_close(t_game *game)
{
	mlx_destroy_image(game->mlx, game->images->base->img);
	mlx_destroy_window(game->mlx, game->win);
	pthread_mutex_lock(&game->mutex_music);
	game->is_running = false;
	pthread_mutex_unlock(&game->mutex_music);
	pthread_mutex_destroy(&game->mutex_music);
	destroy_thread_pool(game->pool);
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
	const int	width = 350 + 160;
	t_draw_info	info;

	if (game->menu->message == OPEN_DOOR)
		draw_menu_message(game, " to open");
	else if (game->menu->message == CLOSE_DOOR)
		draw_menu_message(game, " to close");
	else if (game->menu->message == TELEPORT)
	{
		info = init_draw_info(40, "", (game->screen_width - width) * 0.5 - 80,
				(game->screen_height - 40) * 0.5 - 120);
		info.width = width;
		info.height = 40;
		info.color = MENU_BUTTON_COLOR;
		draw_rectangle(game, info);
		draw_menu_message(game, " to teleport");
	}
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
