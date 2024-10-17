/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:46:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/17 14:36:22 by ybeaucou         ###   ########.fr       */
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
	t_image	*floor_texture = game->textures->floor;
	t_image	*ceiling_texture = game->textures->ceil;

	bool use_floor_texture = (game->textures->color_f  < 0);
	bool use_ceiling_texture = (game->textures->color_c < 0);

	int floor_color_default = game->textures->color_f;
	int ceiling_color_default = game->textures->color_c;

	float ray_dir_x0 = game->player->dirX - game->player->planeX;
	float ray_dir_x1 = game->player->dirX + game->player->planeX;
	float ray_dir_y0 = game->player->dirY - game->player->planeY;
	float ray_dir_y1 = game->player->dirY + game->player->planeY;
	float pos_z = game->screen_height * 0.5f;

	float diff_ray_dir_x = ray_dir_x1 - ray_dir_x0;
	float diff_ray_dir_y = ray_dir_y1 - ray_dir_y0;

	int floor_texture_width = use_floor_texture ? floor_texture->width : 0;
	int floor_texture_height = use_floor_texture ? floor_texture->height : 0;
	char *floor_texture_data = use_floor_texture ? floor_texture->data : NULL;
	int floor_bpp = use_floor_texture ? floor_texture->bpp / 8 : 0;

	int ceiling_texture_width = use_ceiling_texture ? ceiling_texture->width : 0;
	int ceiling_texture_height = use_ceiling_texture ? ceiling_texture->height : 0;
	char *ceiling_texture_data = use_ceiling_texture ? ceiling_texture->data : NULL;
	int ceiling_bpp = use_ceiling_texture ? ceiling_texture->bpp / 8 : 0;

	int screen_mid = game->screen_height >> 1;
	float screen_width_inv = 1.0f / game->screen_width;

	float diff_screen_x = diff_ray_dir_x * screen_width_inv;
	float diff_screen_y = diff_ray_dir_y * screen_width_inv;

	for (int y = 0; y < game->screen_height; y++)
	{
		int p = y - screen_mid;
		if (p == 0) p = 1;
		float row_distance = pos_z / fabsf(p);

		float floor_step_x = row_distance * diff_screen_x;
		float floor_step_y = row_distance * diff_screen_y;

		float floor_x = game->player->x + row_distance * ray_dir_x0;
		float floor_y = game->player->y + row_distance * ray_dir_y0;

		float ceiling_x = game->player->x + row_distance * ray_dir_x0;
		float ceiling_y = game->player->y + row_distance * ray_dir_y0;

		for (int x = 0; x < game->screen_width; x++)
		{
			int floor_color, ceiling_color;
			if (use_floor_texture)
			{
				float current_floor_x = floor_x + x * floor_step_x;
				float current_floor_y = floor_y + x * floor_step_y;
				int floor_tex_x = (int)(current_floor_x * floor_texture_width) % floor_texture_width;
				int floor_tex_y = (int)(current_floor_y * floor_texture_height) % floor_texture_height;
				floor_color = *((int *)(floor_texture_data + floor_tex_y * floor_texture->size_line + floor_tex_x * floor_bpp));
			}
			else
				floor_color = floor_color_default;
			if (use_ceiling_texture)
			{
				float current_ceiling_x = ceiling_x + x * floor_step_x;
				float current_ceiling_y = ceiling_y + x * floor_step_y;
				int ceiling_tex_x = (int)(current_ceiling_x * ceiling_texture_width) % ceiling_texture_width;
				int ceiling_tex_y = (int)(current_ceiling_y * ceiling_texture_height) % ceiling_texture_height;
				ceiling_color = *((int *)(ceiling_texture_data + ceiling_tex_y * ceiling_texture->size_line + ceiling_tex_x * ceiling_bpp));
			}
			else
				ceiling_color = ceiling_color_default;
			if (y < screen_mid)
				secure_pixel_put(game, x, y, ceiling_color);
			else
				secure_pixel_put(game, x, y, floor_color);
		}
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
		update_option_menu_click(game, x, y, keycode);
	else if (game->menu->status == SERVERS)
		update_multiplayer_click(game, x, y, keycode);
	else if (game->menu->status == SERVER_CREATE)
		update_create_server_menu_text(game, x, y, keycode);
	else if (game->menu->status == JOIN_SERVER)
		update_join_server_menu_text(game, x, y, keycode);
	else if (game->menu->status == MAIN_MENU)
		update_main_menu_click(game, x, y, keycode);
	else if (game->menu->status == SERVER_DISCONNECTED || game->menu->status == SERVER_FULL)
		update_server_error_click(game, x, y, keycode);
	else if (game->menu->status == CHATING)
		handle_mouse_chat(game, x, y, keycode);
	else if ((game->menu->status == PLAYING
		|| game->menu->status == MULTI_PLAYER)
		&& keycode == 1)
		game->player->anim_shoot = 1;
	return (0);
}

int	handle_mouse_move(int x, int y, t_game *game)
{
	if (game->menu->status == MAIN_MENU)
		update_main_menu_button(game, x, y);
	else if (game->menu->status == OPTIONS)
		update_option_menu_button(game, x, y);
	else if (game->menu->status == SERVERS)
		update_multiplayer_menu(game, x, y);
	else if (game->menu->status == SERVER_CREATE)
		update_create_server_menu_button(game, x, y);
	else if (game->menu->status == JOIN_SERVER)
		update_join_server_menu_button(game, x, y);
	else if (game->menu->status == SERVER_DISCONNECTED || game->menu->status == SERVER_FULL)
		update_server_error_button(game, x, y);
	if ((game->menu->status != PLAYING && game->menu->status != MULTI_PLAYER) || x == game->screen_width * 0.5)
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
	else if (game->menu->status == SERVER_CREATE || game->menu->status == JOIN_SERVER)
		handle_text_input(game, keycode);
	if (game->menu->status == CHATING)
		chat_input(game, keycode);
	if (keycode == 116 && (game->menu->status == MULTI_PLAYER || game->menu->status == CHATING) && !game->chatbox->is_writting)
	{
		if (game->menu->status == MULTI_PLAYER)
		{
			game->menu->status = CHATING;
			mlx_mouse_show(game->mlx, game->win);
		}
		else
		{
			game->menu->status = MULTI_PLAYER;
			mlx_mouse_hide(game->mlx, game->win);
		}
		game->chatbox->visible = !game->chatbox->visible;
	}
	if (game->menu->status != PLAYING && game->menu->status != MULTI_PLAYER)
		return (0);
	if (keycode == 65362 || keycode == 119) // W pour avancer
	{
		if (!can_move(game, p->x + p->dirX * 0.1, p->y + p->dirY * 0.1))
			return (0);
		p->x += p->dirX * 0.1;
		p->y += p->dirY * 0.1;
	}
	else if (keycode == 65364 || keycode == 115) // S pour reculer
	{
		if (!can_move(game, p->x - p->dirX * 0.1, p->y - p->dirY * 0.1))
			return (0);
		p->x -= p->dirX * 0.1;
		p->y -= p->dirY * 0.1;
	}
	else if (keycode == 65363 || keycode == 100) // D pour aller à droite
	{
		if (!can_move(game, p->x + p->planeX * 0.1, p->y + p->planeY * 0.1))
			return (0);
		p->x += p->planeX * 0.1;
		p->y += p->planeY * 0.1;
	}
	else if (keycode == 65361 || keycode == 97) // A pour aller à gauche
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
	if (game->menu->status == MULTI_PLAYER)
	{
		GameMessage message = {.type = MSG_MOVE, .player_id = game->server->player_id, .x = p->x, .y = p->y, .floor = p->floor, .height = p->height};
		strncpy(message.pseudo, game->server->pseudo, MAX_PSEUDO_LENGTH);
		send(game->server->sock, &message, sizeof(GameMessage), 0);
	}
	return (0);
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
		draw_rectangle(game, text_x - 60, text_y, width + 220, height, MENU_BUTTON_COLOR);
		draw_text(game, "Appuyer sur F pour vous teleportez", game->screen_width * 0.5, game->screen_height * 0.5 - 130, 30, MENU_BUTTON_TEXT_COLOR);
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
	draw_text_left(game, ft_itoa(fps), 63, 20, 30, 0xFFFFF);
}

int	game_loop(t_game *game)
{
	ft_memset(game->images->base->data, 0, game->screen_width * game->screen_height * 4);
	if (game->menu->status == MAIN_MENU)
		draw_main_menu(game);
	else if (game->menu->status == OPTIONS)
		draw_options_menu(game);
	else if (game->menu->status == SERVERS)
		draw_multiplayer_menu(game);
	else if (game->menu->status == SERVER_CREATE)
		draw_create_server_menu(game);
	else if (game->menu->status == VALID_SERVER_CREATE)
	{
		create_server(game);
		game->server->ip = "127.0.0.1";
		game->menu->status = VALID_JOIN_SERVER;
	}
	else if (game->menu->status == JOIN_SERVER)
		draw_join_server_menu(game);
	else if (game->menu->status == VALID_JOIN_SERVER)
	{
		if (join_server(game) == 1)
			game->menu->status = MULTI_PLAYER;
	}
	else if (game->menu->status == SERVER_DISCONNECTED || game->menu->status == SERVER_FULL)
		draw_server_error_menu(game);
	else if (game->menu->status == PLAYING || game->menu->status == MULTI_PLAYER || game->menu->status == CHATING)
	{
		game->menu->message = NOTHING;
		calculate_delta_time(game);
		update_door_animation(game);
		cast_rays(game);
		cast_floor(game);
		draw_players(game);
		update_enemies(game);
		draw_sprites(game);
<<<<<<< HEAD
		head_up_display(game);
=======
		mini_map(game);
		crosshair(game);
		gun_draw(game);
		chat_draw(game);
>>>>>>> d44b37d (update on chat)
		 // game delta time = nbre de s depuis la dernier image
	
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
