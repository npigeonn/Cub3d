/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:16:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/08 15:54:15 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_door	*get_door(t_game *game, int x, int y)
{
	t_door	*current;

	current = game->door;
	if (!current)
		return (NULL);
	while (current)
	{
		if (current->x == x && current->y == y)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	use_door_in_view(t_game *game)
{
	const t_player *player = game->player;
	
	float dirX = player->dirX;
	float dirY = player->dirY;

	float length = sqrt(dirX * dirX + dirY * dirY);
	if (length != 0)
	{
		dirX /= length;
		dirY /= length;
	}
	float door_check_distance = 0.3f;
	float checkX = player->x + dirX * door_check_distance;
	float checkY = player->y + dirY * door_check_distance;
	
	t_door *door = get_door(game, 1, 2);
	if (door)
		door->open = !door->open;
}

void	add_door(t_game *game, int x, int y, int floor, bool lock)
{
	t_door *new_door;

	new_door = malloc(sizeof(t_door));
	new_door->x = x;
	new_door->y = y;
	new_door->floor = floor;
	new_door->open = false;
	new_door->lock = lock;
	if (game->door)
		new_door->next = game->door;
	game->door = new_door;
}

void draw_door(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side, t_door *door)
{
	t_image	*texture = game->textures->door;

	float perp_wall_dist = (side == SIDE_EAST || side == SIDE_WEST)
		? (map_x - game->player->x + (1 - step_x) * 0.5) / ray_dir_x
		: (map_y - game->player->y + (1 - step_y) * 0.5) / ray_dir_y;

	int line_height = (int)(game->screen_height / perp_wall_dist);
	int draw_start = -line_height * 0.5 + game->screen_height * 0.5;
	int draw_end = line_height * 0.5 + game->screen_height * 0.5;

	draw_start -= (int)(game->player->height * line_height);
	draw_end -= (int)(game->player->height * line_height);

	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;

	float wall_x;
	if (side == SIDE_EAST || side == SIDE_WEST) {
		wall_x = game->player->y + perp_wall_dist * ray_dir_y;
	} else {
		wall_x = game->player->x + perp_wall_dist * ray_dir_x;
	}
	wall_x -= floor(wall_x);
		
	int texture_width = texture->width;
	wall_x *= texture_width;
	int animated_height = (int)((1.0f - door->animation) * line_height);
	int animated_draw_start = draw_end - animated_height;
	if (animated_draw_start < draw_start)
		animated_draw_start = draw_start;
	if (door->animation < 1.0f)
		draw_vertical_line_with_texture(game, x, animated_draw_start, draw_end, texture, wall_x, line_height);
}


void update_door_animation(t_door *door, float delta_time)
{
	if (door->open)
	{
		door->animation += 0.0005 * delta_time;
		if (door->animation > 1.0f)
			door->animation = 1.0f;
	}
	else
	{
		door->animation -= 0.0005 * delta_time;
		if (door->animation < 0.0f)
			door->animation = 0.0f;
	}
}


int	handle_door(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side, float distance)
{
	t_door	*door;

	if (game->map[game->player->floor][map_x][map_y] == 'D')
	{
		door = get_door(game, map_x, map_y);
		if (!door)
			return (0);
		update_door_animation(door, game->delta_time);
		draw_door(game, x, map_x, map_y, step_x, step_y, ray_dir_x, ray_dir_y, side, door);
		if (distance < 0.4f)
		{
			if (door->open)
				game->message = CLOSE_DOOR;	
			else
				game->message = OPEN_DOOR;
		}
		// if (!door->open && door->animation <= 1)
		// 	return (0);
		// if (!door->open)
		// 	return (1);
		return (!door->open);
	}
	return (0);
}
