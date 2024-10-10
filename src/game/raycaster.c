/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:46:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/10 10:49:28 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
	Si ray_dir_x = 1 et ray_dir_y = 0, le joueur regarde vers l'Est.
	Si ray_dir_x = -1 et ray_dir_y = 0, le joueur regarde vers l'Ouest.
	Si ray_dir_x = 0 et ray_dir_y = 1, il regarde vers le Sud.
	Si ray_dir_x = 0 et ray_dir_y = -1, il regarde vers le Nord.
*/

#include "../../includes/cub3d.h"

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
			if (game->map[game->player->floor][map_y][map_x] == '1')
			{
				draw_wall(game, x, map_x, map_y, step_x, step_y, ray_dir_x, ray_dir_y, side);
				break ;
			}
			float distance = (side == SIDE_EAST || side == SIDE_WEST) ? side_dist_x - delta_dist_x : side_dist_y - delta_dist_y;
			if (handle_door(game, x, map_x, map_y, step_x, step_y, ray_dir_x, ray_dir_y, side, distance))
				break ;
		}
	}
}

void cast_floor(t_game *game)
{
	t_image	*texture = game->textures->floor;


	float ray_dir_x0 = game->player->dirX - game->player->planeX;
	float ray_dir_x1 = game->player->dirX + game->player->planeX;
	float ray_dir_y0 = game->player->dirY - game->player->planeY;
	float ray_dir_y1 = game->player->dirY + game->player->planeY;

	int y = game->screen_height * 0.5;
	while (y < game->screen_height)
	{
		int p = y - game->screen_height * 0.5;
		float pos_z = 0.5 * game->screen_height;
		float row_distance = pos_z / p;

		float floor_step_x = row_distance * (ray_dir_x1 - ray_dir_x0) / game->screen_width;
		float floor_step_y = row_distance * (ray_dir_y1 - ray_dir_y0) / game->screen_width;

		float floor_x = game->player->x + row_distance * ray_dir_x0;
		float floor_y = game->player->y + row_distance * ray_dir_y0;

		for (int x = 0; x < game->screen_width; x++)
		{
			float current_floor_x = floor_x + x * floor_step_x;
			float current_floor_y = floor_y + x * floor_step_y;
			int tex_x = (int)(texture->width * (current_floor_x - (int)current_floor_x)) % texture->width;
			int tex_y = (int)(texture->height * (current_floor_y - (int)current_floor_y)) % texture->height;
			int floor_color = *((int *)(texture->data + tex_y * texture->size_line + tex_x * (texture->bpp / 8)));
			secure_pixel_put(game, x, y, floor_color);
			// secure_pixel_put(game, x, y + game->screen_height, floor_color);
		}
		y++;
	}
}


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
			mlx_mouse_hide(game->mlx, game->win);
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
	if (game->status != PLAYING || x == game->screen_width * 0.5)
		return (0);
	int centerX = game->screen_width * 0.5;
	int centerY = game->screen_height * 0.5;
	int deltaX = x - centerX;
	int deltaY = y - centerY;
	float rotation = deltaX * ROTATION_SPEED;
	float oldDirX = game->player->dirX;
	game->player->dirX = oldDirX * cos(rotation) - game->player->dirY * sin(rotation);
	game->player->dirY = oldDirX * sin(rotation) + game->player->dirY * cos(rotation);
	float oldPlaneX = game->player->planeX;
	game->player->planeX = oldPlaneX * cos(rotation) - game->player->planeY * sin(rotation);
	game->player->planeY = oldPlaneX * sin(rotation) + game->player->planeY * cos(rotation);
	mlx_mouse_move(game->mlx, game->win, centerX, centerY);
	(void)y;
	return (0);
}

bool	can_move(t_game *game, float x, float y)
{
	t_door	*door;

	if (game->map[game->player->floor][(int)(y)][(int)(x)] == '1')
		return (false);
	if (game->map[game->player->floor][(int)(y)][(int)(x)] == 'D')
	{
		door = get_door(game, (int)x, (int)y, game->player->floor);
		if (!door || !door->open)
			return (false);
	}
	return (true);
}

void	use_item(t_game *game)
{
	use_door_in_view(game);
	use_teleporter(game);
}

int	handle_keypress(int keycode, t_game *game)
{
	t_player *p = game->player;
	
	if (keycode == 65307)
		handle_close(game);
	if (game->status != PLAYING)
		return (0);
	if (keycode == 65362 || keycode == 119) // W pour avancer
	{
		if (!can_move(game, p->x + p->dirX * 0.1, p->y + p->dirY * 0.1))
			return (0);
		p->x += p->dirX * 0.1;
		p->y += p->dirY * 0.1;
	}
	if (keycode == 65364 || keycode == 115) // S pour reculer
	{
		if (!can_move(game, p->x - p->dirX * 0.1, p->y - p->dirY * 0.1))
			return (0);
		p->x -= p->dirX * 0.1;
		p->y -= p->dirY * 0.1;
	}
	if (keycode == 65363 || keycode == 100) // D pour aller à droite
	{
		if (!can_move(game, p->x + p->planeX * 0.1, p->y + p->planeY * 0.1))
			return (0);
		p->x += p->planeX * 0.1;
		p->y += p->planeY * 0.1;
	}
	if (keycode == 65361 || keycode == 97) // A pour aller à gauche
	{
		if (!can_move(game, p->x - p->planeX * 0.1, p->y - p->planeY * 0.1))
			return (0);
		p->x -= p->planeX * 0.1;
		p->y -= p->planeY * 0.1;
	}
	if (keycode == 32) // Espace pour sauter
		p->height -= 0.1;
	if (keycode == 98) // b pour s'accroupir
		p->height += 0.1;
	if (keycode == 102)
		use_item(game);
	return (0);
}

void	clear_image(t_game *game, int color)
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

void	show_message(t_game *game)
{
	int width = 350;
	int height = 40;
	int text_x = (game->screen_width - width) * 0.5;
	int text_y = (game->screen_height - height) * 0.5 - 120;

	draw_rectangle(game, text_x, text_y, width, height, MENU_BUTTON_COLOR);
	if (game->message == OPEN_DOOR)
		draw_text(game, "Appuyer sur F pour ouvrir", game->screen_width * 0.5, game->screen_height * 0.5 - 135, 30, MENU_BUTTON_TEXT_COLOR);
	else if (game->message == CLOSE_DOOR)
		draw_text(game, "Appuyer sur F pour fermer", game->screen_width * 0.5, game->screen_height * 0.5 - 135, 30, MENU_BUTTON_TEXT_COLOR);
	else if (game->message == TELEPORT)
	{
		draw_rectangle(game, text_x - 40, text_y, width + 80, height, MENU_BUTTON_COLOR);
		draw_text(game, "Appuyer sur F pour vous teleportez", game->screen_width * 0.5, game->screen_height * 0.5 - 135, 30, MENU_BUTTON_TEXT_COLOR);
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

int	game_loop(t_game *game)
{
	clear_image(game, 0x000000);
	if (game->status == MAIN_MENU)
		draw_main_menu(game);
	else if (game->status == OPTIONS)
		draw_options_menu(game);
	else if (game->status == SERVEURS)
		draw_multiplayer_menu(game);
	else if (game->status == PLAYING)
	{
		game->message = NOTHING;
		calculate_delta_time(game);
		update_door_animation(game);
		cast_rays(game);
		cast_floor(game);
		draw_sprites(game);
		if (is_a_teleporter(game->map[game->player->floor][(int)game->player->y][(int)game->player->x]))
			game->message = TELEPORT;
		if (game->message != NOTHING)
			show_message(game);
	}
	mlx_put_image_to_window(game->mlx, game->win, game->images->base->img, 0, 0);
	if (game->map[game->player->floor][(int)game->player->y][(int)game->player->x] == 'e')
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
