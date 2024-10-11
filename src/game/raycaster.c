/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:46:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/11 13:45:01 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
	Si ray_dir_x = 1 et ray_dir_y = 0, le joueur regarde vers l'Est.
	Si ray_dir_x = -1 et ray_dir_y = 0, le joueur regarde vers l'Ouest.
	Si ray_dir_x = 0 et ray_dir_y = 1, il regarde vers le Sud.
	Si ray_dir_x = 0 et ray_dir_y = -1, il regarde vers le Nord.
*/

#include "../../includes/cub3d.h"

void cast_rays(t_game *game)
{

	for (int x = 0; x < game->screen_width; x++)
	{
		int map_x = (int)game->player->x;
		int map_y = (int)game->player->y;

		float camera_x = 2 * x / (float)game->screen_width - 1;
		float ray_dir_x = game->player->dirX + game->player->planeX * camera_x;
		float ray_dir_y = game->player->dirY + game->player->planeY * camera_x;
		float delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
		float delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);

		int step_x = (ray_dir_x < 0) ? -1 : 1;
		int step_y = (ray_dir_y < 0) ? -1 : 1;
		
		float side_dist_x = (ray_dir_x < 0) ? (game->player->x - map_x) * delta_dist_x : (map_x + 1.0 - game->player->x) * delta_dist_x;
		float side_dist_y = (ray_dir_y < 0) ? (game->player->y - map_y) * delta_dist_y : (map_y + 1.0 - game->player->y) * delta_dist_y;

		int side;
		while (1)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = (step_x > 0) ? SIDE_EAST : SIDE_WEST;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = (step_y > 0) ? SIDE_SOUTH : SIDE_NORTH;
			}
			if (game->map[game->player->floor][map_y][map_x] == '1')
			{
				draw_wall(game, x, map_x, map_y, step_x, step_y, ray_dir_x, ray_dir_y, side);
				break;
			}
			float distance = (side == SIDE_EAST || side == SIDE_WEST) ? side_dist_x - delta_dist_x : side_dist_y - delta_dist_y;
			if (handle_door(game, x, map_x, map_y, step_x, step_y, ray_dir_x, ray_dir_y, side, distance))
				 break;
		}
	}
}


void cast_floor(t_game *game)
{
	t_image *texture = game->textures->floor;

	float ray_dir_x0 = game->player->dirX - game->player->planeX;
	float ray_dir_x1 = game->player->dirX + game->player->planeX;
	float ray_dir_y0 = game->player->dirY - game->player->planeY;
	float ray_dir_y1 = game->player->dirY + game->player->planeY;
	float pos_z = game->screen_height >> 1;

	float diff_ray_dir_x = ray_dir_x1 - ray_dir_x0;
	float diff_ray_dir_y = ray_dir_y1 - ray_dir_y0;

	int texture_width = texture->width;
	int texture_height = texture->height;
	char *texture_data = texture->data;
	int	bpp = texture->bpp / 8;

	int y = game->screen_height >> 1;
	float screen_width_inv = 1.0f / game->screen_width;

	while (y < game->screen_height)
	{
		int p = y - (game->screen_height >> 1);
		if (p <= 0)
			p = 1;
		float row_distance = pos_z / p;

		float floor_step_x = row_distance * diff_ray_dir_x * screen_width_inv;
		float floor_step_y = row_distance * diff_ray_dir_y * screen_width_inv;

		float floor_x = game->player->x + row_distance * ray_dir_x0; 
		float floor_y = game->player->y + row_distance * ray_dir_y0;

		for (int x = 0; x < game->screen_width; x++)
		{
			float current_floor_x = floor_x + x * floor_step_x;
			float current_floor_y = floor_y + x * floor_step_y;
			int tex_x = (int)(current_floor_x * texture_width) % texture_width;
			int tex_y = (int)(current_floor_y * texture_height) % texture_height;
			int floor_color = *((int *)(texture_data + tex_y * texture->size_line + tex_x * bpp));
			if (x >= 0 && x < game->screen_width && y >= 0 && y < game->screen_height)
				secure_pixel_put(game, x, y, floor_color);
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
	if (game->menu->status == OPTIONS)
	{
		if (game->menu->button_selected == 3)
		{
			game->menu->status = MAIN_MENU;
			return (0);
		}
		update_option_menu_slider(game, x, y, keycode);
		return (0);
	}
	if (game->menu->status == SERVEURS)
	{
		if (game->menu->button_selected == 3)
			game->menu->status = MAIN_MENU;
		return (0);
	}
	if (keycode == 1)
	{
		if (game->menu->button_selected == 1)
		{
			game->menu->status = PLAYING;
			mlx_mouse_hide(game->mlx, game->win);
		}
		else if (game->menu->button_selected == 2)
			game->menu->status = SERVEURS;
		else if (game->menu->button_selected == 3)
			game->menu->status = OPTIONS;
		else if (game->menu->button_selected == 4)
			handle_close(game);
	}
	return (0);
}

int	handle_mouse_move(int x, int y, t_game *game)
{
	if (game->menu->status == MAIN_MENU)
		update_main_menu_button(game, x, y);
	else if (game->menu->status == OPTIONS)
		update_option_menu_button(game, x, y);
	else if (game->menu->status == SERVEURS)
		update_multiplayer_menu(game, x, y);
	if (game->menu->status != PLAYING || x == game->screen_width * 0.5)
		return (0);
	int centerX = game->screen_width * 0.5;
	int centerY = game->screen_height * 0.5;
	float rotation = (x - centerX) * ROTATION_SPEED;
	float oldDirX = game->player->dirX;
	game->player->dirX = oldDirX * cos(rotation) - game->player->dirY * sin(rotation);
	game->player->dirY = oldDirX * sin(rotation) + game->player->dirY * cos(rotation);
	float oldPlaneX = game->player->planeX;
	game->player->planeX = oldPlaneX * cos(rotation) - game->player->planeY * sin(rotation);
	game->player->planeY = oldPlaneX * sin(rotation) + game->player->planeY * cos(rotation);
	mlx_mouse_move(game->mlx, game->win, centerX, centerY);
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
	if (game->menu->status != PLAYING)
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

void	show_menu_message(t_game *game)
{
	int width = 350;
	int height = 40;
	int text_x = (game->screen_width - width) * 0.5;
	int text_y = (game->screen_height - height) * 0.5 - 120;

	if (game->menu->message == OPEN_DOOR)
	{
		draw_rectangle(game, text_x, text_y, width, height, MENU_BUTTON_COLOR);
		draw_text(game, "Appuyer sur F pour ouvrir", game->screen_width * 0.5, game->screen_height * 0.5 - 135, 30, MENU_BUTTON_TEXT_COLOR);
	}
	else if (game->menu->message == CLOSE_DOOR)
	{
		draw_rectangle(game, text_x, text_y, width, height, MENU_BUTTON_COLOR);
		draw_text(game, "Appuyer sur F pour fermer", game->screen_width * 0.5, game->screen_height * 0.5 - 135, 30, MENU_BUTTON_TEXT_COLOR);
	}
	else if (game->menu->message == TELEPORT)
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
	draw_text_left(game, "FPS: ", 5, 20, 30, 0xFFFFF);
	draw_text_left(game, ft_itoa(fps), 50, 20, 30, 0xFFFFF);
}

int	game_loop(t_game *game)
{
	clear_image(game, 0x000000);
	if (game->menu->status == MAIN_MENU)
		draw_main_menu(game);
	else if (game->menu->status == OPTIONS)
		draw_options_menu(game);
	else if (game->menu->status == SERVEURS)
		draw_multiplayer_menu(game);
	else if (game->menu->status == PLAYING)
	{
		game->menu->message = NOTHING;
		calculate_delta_time(game);
		update_door_animation(game);
		cast_rays(game);
		cast_floor(game);
		update_enemies(game);
		draw_sprites(game);
		if (is_a_teleporter(game->map[game->player->floor][(int)game->player->y][(int)game->player->x]))
			game->menu->message = TELEPORT;
		if (game->menu->message != NOTHING)
			show_menu_message(game);
	}
	calculate_fps(game);
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
