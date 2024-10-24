/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:46:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/24 17:00:58 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	handle_close(t_game *game)
{
	mlx_destroy_image(game->mlx, game->images->base->img);
	mlx_destroy_image(game->mlx, game->images->alphanum_sprite->img);
	mlx_destroy_window(game->mlx, game->win);
	mlx_destroy_display(game->mlx);
	free_map(game);
	gc_free(game->mem, game->images->base);
	gc_free(game->mem, game->images->alphanum_sprite);
	free(game->mlx);
	exit(0);
}

void	show_menu_message(t_game *game)
{
	const int	width = 350;
	const int	height = 40;
	const int	text_x = (game->screen_width - width) * 0.5;
	const int	text_y = (game->screen_height - height) * 0.5 - 120;
	t_draw_info	info;

	info = init_draw_info(40, "", text_x, text_y);
	info.width = width;
	info.height = height;
	info.color = MENU_BUTTON_COLOR;
	if (game->menu->message == OPEN_DOOR)
	{
		draw_rectangle(game, info);
		ft_strcpy(info.str, "Appuyer sur F pour ouvrir");
		info.height = 30;
		info.y = game->screen_height * 0.5 - 129;
		info.x = game->screen_width * 0.5;
		info.color = MENU_BUTTON_TEXT_COLOR;
		draw_text(game, info);
	}
	else if (game->menu->message == CLOSE_DOOR)
	{
		draw_rectangle(game, info);
		ft_strcpy(info.str, "Appuyer sur F pour fermer");
		info.height = 30;
		info.y = game->screen_height * 0.5 - 129;
		info.x = game->screen_width * 0.5;
		info.color = MENU_BUTTON_TEXT_COLOR;
		draw_text(game, info);
	}
	else if (game->menu->message == TELEPORT)
	{
		info.x -= 80;
		info.width += 160;
		draw_rectangle(game, info);
		ft_strcpy(info.str, "Appuyer sur F pour vous teleportez");
		info.height = 30;
		info.y = game->screen_height * 0.5 - 129;
		info.x = game->screen_width * 0.5;
		info.color = MENU_BUTTON_TEXT_COLOR;
		draw_text(game, info);
	}
}

void	calculate_delta_time(t_game *game)
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	float seconds = (current_time.tv_sec - game->last_time.tv_sec) +
					(current_time.tv_usec - game->last_time.tv_usec) / 1000000.0f;
	game->delta_time = seconds;
	game->last_time = current_time;
}

static double last_time = 0.0;
static int frame_count = 0;
static double fps = 0.0;

void	calculate_fps(t_game *game)
{
	struct	timeval current_time;
	gettimeofday(&current_time, NULL);
	double	elapsed_time = (current_time.tv_sec + current_time.tv_usec / 1e6) - last_time;

	frame_count++;

	if (elapsed_time >= 1.0)
	{
		fps = frame_count / elapsed_time;
		frame_count = 0;
		last_time = current_time.tv_sec + current_time.tv_usec / 1e6;
	}
	t_draw_info	info = init_draw_info(30, "FPS: ", 5, 20);
	info.color = 0xFFFFF;
	draw_text_left(game, info);
	ft_strcpy(info.str, ft_itoa(fps));
	info.x = 60;
	draw_text_left(game, info);
}

void	set_direction(t_game *game, int dir)
{
	if (dir == 0) // Nord
	{
		game->player->dirX = 0;
		game->player->dirY = -1;
		game->player->planeX = 0.66;
		game->player->planeY = 0;
	}
	else if (dir == 1) // Sud
	{
		game->player->dirX = 0;
		game->player->dirY = 1;
		game->player->planeX = -0.66;
		game->player->planeY = 0;
	}
	else if (dir == 2) // Est
	{
		game->player->dirX = 1;
		game->player->dirY = 0;
		game->player->planeX = 0;
		game->player->planeY = 0.66;
	}
	else if (dir == 3) // Ouest
	{
		game->player->dirX = -1;
		game->player->dirY = 0;
		game->player->planeX = 0;
		game->player->planeY = -0.66;
	}
}
