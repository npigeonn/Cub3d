/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:16:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/23 13:08:39 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_door	*get_door(t_game *game, int x, int y, int floor)
{
	t_door	*current;

	current = game->door;
	if (!game->door)
		return (NULL);
	while (current)
	{
		// printf("%d, %d\n", (int)x, (int)y);
		if ((int)current->x == x && (int)current->y == y && current->floor == floor)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	use_door_in_view(t_game *game)
{
	const t_player	*player = game->player;
	t_door			*door;
	int				x;
	int				y;
	float			distance = 1;

	if (game->menu->message != CLOSE_DOOR && game->menu->message != OPEN_DOOR)
		return ;

	x = (int)(player->x + player->dirX * distance);
	y = (int)(player->y + player->dirY * distance);

	door = get_door(game, x, y, player->floor);
	if (door)
	{
		door->open = !door->open;
		t_game_message	msg_door;
		msg_door.type = MSG_DOOR;
		msg_door.x = door->x;
		msg_door.y = door->y;
		msg_door.floor = door->floor;
		msg_door.open = door->open;
		msg_door.player_id = game->client->player_id;
		send(game->client->sock, &msg_door, sizeof(t_game_message), 0);
	}
}

void	add_door(t_game *game, int x, int y, int floor)
{
	t_door *new_door;

	new_door = malloc(sizeof(t_door));
	new_door->x = x;
	new_door->y = y;
	new_door->floor = floor;
	new_door->open = false;
	new_door->animation = 0;
    new_door->next = game->door;
    game->door = new_door;
}

void draw_door(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side, t_door *door)
{
	t_image *texture = game->textures->door;

	float perp_wall_dist = (side == SIDE_EAST || side == SIDE_WEST)
		? (map_x - game->player->x + (1 - step_x) * 0.5) / ray_dir_x
		: (map_y - game->player->y + (1 - step_y) * 0.5) / ray_dir_y;

	int line_height = (int)(game->screen_height / perp_wall_dist);
	int draw_start = game->screen_height * 0.5 - line_height * 0.5 - (int)(game->player->height * line_height);
	int draw_end = draw_start + line_height - 1;

	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;

	float wall_x = (side == SIDE_EAST || side == SIDE_WEST)
		? (game->player->y + perp_wall_dist * ray_dir_y)
		: (game->player->x + perp_wall_dist * ray_dir_x);
	wall_x -= floor(wall_x);
	game->wall_distances[x] = perp_wall_dist;
	draw_vertical_line_with_texture(game, x, draw_start, draw_end, texture, wall_x, line_height);
}

void	update_door_animation(t_game *game)
{
	t_door	*current;

	current = game->door;
	if (current)
	while (current)
	{
		if (current->open)
		{
			current->animation += 0.2 * game->delta_time;
			if (current->animation > 1.0)
				current->animation = 1.0;
		}
		else
		{
			current->animation -= 0.2 * game->delta_time;
			if (current->animation < 0.0)
				current->animation = 0.0;
		}
		current = current->next;
	}
}

bool	visible_door(t_door *door)
{
	const float anim = door->animation; 

	if (anim == 0)
		return (true);
	if (anim >= 0.2 && anim <= 0.3)
		return (true);
	if (anim >= 0.5 && anim <= 0.6)
		return (true);
	if (anim >= 0.8 && anim <= 0.9)
		return (true);
	return (false);
}

int	handle_door(t_game *game)
{
	t_door		*door;
	t_raycast	*raycast = game->player->raycast;
	float		distance = raycast->perp_wall_dist;

	if (raycast->side == SIDE_EAST || raycast->side == SIDE_WEST)
		distance = (raycast->map_x - game->player->x + (1 - raycast->step_x) * 0.5) / raycast->ray_dir_x;
	else
		distance = (raycast->map_y - game->player->y + (1 - raycast->step_y) * 0.5) / raycast->ray_dir_y;
	if (game->map[game->player->floor][raycast->map_y][raycast->map_x] == 'D')
	{
		door = get_door(game, raycast->map_x, raycast->map_y, game->player->floor);
		if (!door)
			return (0);
		if (visible_door(door))
			draw_door(game, raycast->x, raycast->map_x, raycast->map_y, raycast->step_x, raycast->step_y, raycast->ray_dir_x, raycast->ray_dir_y, raycast->side, door);
		if (distance < 0.4)
		{
			if (door->open)
				game->menu->message = CLOSE_DOOR;	
			else
				game->menu->message = OPEN_DOOR;
		}
		if (visible_door(door))
			return (1);
	}
	return (0);
}


void	add_ammo(t_game *game, int x, int y, int floor)
{
	t_sprite *new_ammo;

	new_ammo = malloc(sizeof(t_sprite));
	new_ammo->x = x;
	new_ammo->y = y;
	new_ammo->floor = floor;
	new_ammo->still_exist = 1;
	new_ammo->next = game->sprites;
	new_ammo->type = SPRITE_AMMO;
	game->sprites = new_ammo;
}

void	add_health(t_game *game, int x, int y, int floor)
{
	t_sprite *new_health;

	new_health = malloc(sizeof(t_sprite));
	new_health->x = x;
	new_health->y = y;
	new_health->floor = floor;
	new_health->still_exist = 1;
	new_health->next = game->sprites;
	new_health->type = SPRITE_HEALTH;
	game->sprites = new_health;
}

void	door_ennemi_ammo_health_mngmt(t_game *game)
{
	int	i;
	int	j;
	int	k;
	
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			k = -1;
			while (game->map[i][j][++k])
			{
				if (game->map[i][j][k] == 'D')
					add_door(game, k, j, i);
				if (game->map[i][j][k] == 'M')
					add_ammo(game, k + 0.5, j + 0.5, i);
				if (game->map[i][j][k] == 'B')
					add_enemies(game, k, j, i);
				if (game->map[i][j][k] == 'H')
					add_health(game, k, j + 0.5, i + 0.5);
			}
		}
	}
}
