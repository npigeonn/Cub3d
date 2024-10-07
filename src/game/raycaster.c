/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 23:37:48 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/07 09:08:03 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
	if (x > game->screen_width || x < 0)
		return;
	for (int y = start; y < end; y++)
	{
		if (y >= 0 && y < game->screen_height && *((int *)(game->images->base->data + y * game->images->base->size_line + x * (int)(game->images->base->bpp * 0.125))) == 0)
			pixel_put(game, x, y, color);
	}
}

void draw_ceilling(t_game *game)
{
	int horizon = (game->screen_height * 0.5) + game->player->height;
	if (horizon < 0)
		horizon = 0;
	else if (horizon > game->screen_height)
		horizon = game->screen_height;
	for (int y = horizon; y > 0; y--)
	{
		int color = 0x896542;
		int shadow_factor = (y + horizon) * 255 / (game->screen_height + horizon);
		shadow_factor = fmin(1.0f, shadow_factor);
		int shadowed_color = ((int)((color & 0xFF0000) * shadow_factor) & 0xFF0000) |
						((int)((color & 0x00FF00) * shadow_factor) & 0x00FF00) |
						((int)((color & 0x0000FF) * shadow_factor) & 0x0000FF);
	   for (int x = 0; x < game->screen_width; x++)
		{
			if (y >= 0 && y < game->screen_height && *(int *)(game->images->base->data + y * game->images->base->size_line + x * (int)(game->images->base->bpp * 0.125)) == 0)
				pixel_put(game, x, y, shadowed_color);
		}
	}
}

void draw_floor(t_game *game)
{
	int horizon = (game->screen_height * 0.5) + game->player->height;
	if (horizon < 0)
		horizon = 0;
	else if (horizon > game->screen_height)
		horizon = game->screen_height;
	for (int y = horizon; y < game->screen_height; y++)
	{
		int color = 0xaba89f;
		int shadow_factor = (y - horizon) * 255 / (game->screen_height - horizon);
		shadow_factor = fmin(1.0f, shadow_factor);
		int shadowed_color = ((int)((color & 0xFF0000) * shadow_factor) & 0xFF0000) |
						((int)((color & 0x00FF00) * shadow_factor) & 0x00FF00) |
						((int)((color & 0x0000FF) * shadow_factor) & 0x0000FF);
	   for (int x = 0; x < game->screen_width; x++)
		{
			if (y >= 0 && y < game->screen_height && *(int *)(game->images->base->data + y * game->images->base->size_line + x * (int)(game->images->base->bpp * 0.125)) == 0)
				pixel_put(game, x, y, shadowed_color);
		}
	}
}

void draw_wall(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side)
{
	float perp_wall_dist = (side == SIDE_EAST) || (side == SIDE_WEST)
		? (map_x - game->player->x + (1 - step_x) * 0.5) / ray_dir_x
		: (map_y - game->player->y + (1 - step_y) * 0.5) / ray_dir_y;
		
	int line_height = (int)(game->screen_height / perp_wall_dist);
	int draw_start = -line_height * 0.5 + game->screen_height * 0.5;
	int draw_end = line_height * 0.5 + game->screen_height * 0.5;
	draw_start -= (int)(game->player->height * line_height);
	draw_end -= (int)(game->player->height * line_height);

	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;

	int color;
	if (side == SIDE_EAST)
		color = 0x00FFFF; // Cyan
	else if (side == SIDE_NORTH)
		color = 0xFF0000; // Rouge
	else if (side == SIDE_SOUTH)
		color = 0x0000FF; // Bleu
	else
		color = 0xFFFF00; // Jaune

	float shadow_factor = fmax(0.1f, 1.0f - (perp_wall_dist / 100.0f));
	int shadowed_color = ((int)((color & 0xFF0000) * shadow_factor) & 0xFF0000) |
						((int)((color & 0x00FF00) * shadow_factor) & 0x00FF00) |
						((int)((color & 0x0000FF) * shadow_factor) & 0x0000FF);
	draw_vertical_line(game, x, draw_start - 1, draw_end, shadowed_color);
}

void	cast_rays(t_game *game)
{
	for (int x = 0; x < game->screen_width; x++)
	{
		float camera_x = 2 * x / (float)game->screen_width - 1;
		float ray_dir_x = game->player->dirX + game->player->planeX * camera_x;
		float ray_dir_y = game->player->dirY + game->player->planeY * camera_x;

		int map_x = (int)game->player->x;
		int map_y = (int)game->player->y;

		float delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
		float delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);

		int step_x, step_y;
		float side_dist_x, side_dist_y;

		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (game->player->x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - game->player->x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (game->player->y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - game->player->y) * delta_dist_y;
		}
		int side;

		while (1)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				if (step_x > 0) 
					side = SIDE_EAST;
				else 
					side = SIDE_WEST;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;

				if (step_y > 0)
					side = SIDE_SOUTH;
				else
					side = SIDE_NORTH;
			}
			// if (map_x < 0 || map_y < 0 || map_x >= game->map_width || map_y >= game->map_height)
			// 	break;
			if (game->map[game->player->floor][map_x][map_y] == '1')
			{
				draw_wall(game, x, map_x, map_y, step_x, step_y, ray_dir_x, ray_dir_y, side);
				break;
			}
		}
	}
}
int	handle_close(t_game *game)
{
	mlx_destroy_image(game->mlx, game->images->base->img);
	mlx_destroy_image(game->mlx, game->images->alphanum_sprite->img);
	mlx_destroy_window(game->mlx, game->win);
	mlx_destroy_display(game->mlx);
	gc_free(game->mem, game->images->base);
	gc_free(game->mem, game->images->alphanum_sprite);
	free(game->mlx);
	exit(0);
}

int	handle_mouse_key(int keycode, int x, int y, t_game *game)
{
	if (game->status == OPTIONS)
	{
		if (game->button_selected == 3)
		{
			game->status = MAIN_MENU;
			return (0);
		}
		update_option_menu_slider(game, x, y, keycode);
		return (0);
	}
	if (game->status == SERVEURS)
	{
		if (game->button_selected == 3)
			game->status = MAIN_MENU;
		return (0);
	}
	if (keycode == 1)
	{
		if (game->button_selected == 1)
		{
			game->status = PLAYING;
			// mlx_mouse_hide(game->mlx, game->win);
		}
		else if (game->button_selected == 2)
			game->status = SERVEURS;
		else if (game->button_selected == 3)
			game->status = OPTIONS;
		else if (game->button_selected == 4)
			handle_close(game);
	}
	return (0);
}

int	handle_mouse_move(int x, int y, t_game *game)
{
	if (game->status == MAIN_MENU)
		update_main_menu_button(game, x, y);
	else if (game->status == OPTIONS)
		update_option_menu_button(game, x, y);
	else if (game->status == SERVEURS)
		update_multiplayer_menu(game, x, y);
	if (game->status != PLAYING)
		return (0);
	float deltaX = -(x - game->screen_width * 0.5) * ROTATION_SPEED;

	float oldDirX = game->player->dirX;
	game->player->dirX = oldDirX * cos(-deltaX) - game->player->dirY * sin(-deltaX);
	game->player->dirY = oldDirX * sin(-deltaX) + game->player->dirY * cos(-deltaX);
	float oldPlaneX = game->player->planeX;
	game->player->planeX = oldPlaneX * cos(-deltaX) - game->player->planeY * sin(-deltaX);
	game->player->planeY = oldPlaneX * sin(-deltaX) + game->player->planeY * cos(-deltaX);
	mlx_mouse_move(game->mlx, game->win, game->screen_width * 0.5, game->screen_height * 0.5);
	(void)y;
	return (0);
}


int	handle_keypress(int keycode, t_game *game)
{
	float x, y;
	
	if (keycode == 65307) // TODO a proteger pour l'instant ca segfault
		handle_close(game);
	if (game->status != PLAYING)
		return (0);
	if (keycode == 65362 || keycode == 119) // W pour avancer
	{
		x = game->player->x + game->player->dirX * 0.1;
		y = game->player->y + game->player->dirY * 0.1;
		if (game->map[game->player->floor][(int)x][(int)y] == '1')
			return (0);
		game->player->x = x;
		game->player->y = y;
	}
	if (keycode == 65364 || keycode == 115) // S pour reculer
	{
		game->player->x -= game->player->dirX * 0.1;
		game->player->y -= game->player->dirY * 0.1;
	}
	if (keycode == 65363 || keycode == 100) // D pour aller à droite
	{
		game->player->x += game->player->planeX * 0.1;
		game->player->y += game->player->planeY * 0.1;
	}

	if (keycode == 65361 || keycode == 97) // A pour aller à gauche
	{
		game->player->x -= game->player->planeX * 0.1;
		game->player->y -= game->player->planeY * 0.1;
	}
	if (keycode == 32) // Espace pour sauter
		game->player->height -= 0.1;
	if (keycode == 98) // b pour s'accroupir
		game->player->height += 0.1;
	return (0);
}

void clear_image(t_game *game, int color)
{
	for (int y = 0; y < game->screen_height; y++)
	{
		for (int x = 0; x < game->screen_width; x++)
		{
			int *pixel = (int *)(game->images->base->data + y * game->images->base->size_line + x * (int)(game->images->base->bpp * 0.125));
			*pixel = color;
		}
	}	
}

int	game_loop(t_game *game)
{
	mlx_clear_window(game->mlx, game->win);
	clear_image(game, 0x000000);
	if (game->status == MAIN_MENU)
		draw_main_menu(game);
	else if (game->status == OPTIONS)
		draw_options_menu(game);
	else if (game->status == SERVEURS)
		draw_multiplayer_menu(game);
	else if (game->status == PLAYING)
	{
		cast_rays(game);
		draw_ceilling(game);
		draw_floor(game);
	}
	mlx_put_image_to_window(game->mlx, game->win, game->images->base->img, 0, 0);
	if (game->map[0][(int)game->player->x][(int)game->player->y] == 'e')
	{
		ft_printf("VICTORY\n");
		exit(EXIT_SUCCESS);
	}
	return (0);
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
