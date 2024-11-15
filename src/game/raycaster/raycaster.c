/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:46:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/15 12:13:31 by npigeon          ###   ########.fr       */
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
	gc_exit(game->mem, 0);
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

void	draw_fps(t_game *game, double fps)
{
	t_draw_info	info;

	info = init_draw_info(30, "FPS: ", 5, 20);
	info.color = 0xFFFFF;
	draw_text_left(game, info);
	ft_strcpy(info.str, gc_itoa(game->mem, (int)fps));
	info.x = 60;
	draw_text_left(game, info);
}

void	calculate_fps(t_game *game)
{
	struct timeval	c_time;
	static double	last_time = 0.0;
	static int		frame_count = 0;
	static double	fps = 0.0;
	double			elapsed_time;

	gettimeofday(&c_time, NULL);
	elapsed_time = (c_time.tv_sec + c_time.tv_usec / 1e6) - last_time;
	frame_count++;
	if (elapsed_time >= 1.0)
	{
		fps = frame_count / elapsed_time;
		frame_count = 0;
		last_time = c_time.tv_sec + c_time.tv_usec / 1e6;
	}
	draw_fps(game, fps);
}

void	set_direction2(t_game *game, int dir)
{
	if (dir == 2)
	{
		game->player->dir_x = 1;
		game->player->dir_y = 0;
		game->player->plane_x = 0;
		game->player->plane_y = 0.66;
	}
	else if (dir == 3)
	{
		game->player->dir_x = -1;
		game->player->dir_y = 0;
		game->player->plane_x = 0;
		game->player->plane_y = -0.66;
	}
}

void	set_direction(t_game *game, int dir)
{
	if (dir == 0)
	{
		game->player->dir_x = 0;
		game->player->dir_y = -1;
		game->player->plane_x = 0.66;
		game->player->plane_y = 0;
	}
	else if (dir == 1)
	{
		game->player->dir_x = 0;
		game->player->dir_y = 1;
		game->player->plane_x = -0.66;
		game->player->plane_y = 0;
	}
	else
		set_direction2(game, dir);
}
