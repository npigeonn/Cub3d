/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:12:31 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 11:12:31 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
