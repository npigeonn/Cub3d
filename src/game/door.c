/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:16:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/27 16:00:58 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_door	*get_door(t_door *door, int x, int y, int floor)
{
	t_door	*current;

	if (!door)
		return (NULL);
	current = door;
	while (current)
	{
		if ((int)current->x == x && (int)current->y == y
			&& current->floor == floor)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	send_door_msg(t_game *game, t_door *door)
{
	t_game_message	msg_door;

	ft_bzero(&msg_door, sizeof(t_game_message));
	msg_door.type = MSG_DOOR;
	msg_door.x = door->x;
	msg_door.y = door->y;
	msg_door.floor = door->floor;
	msg_door.open = door->open;
	msg_door.player_id = game->client->player_id;
	send(game->client->sock, &msg_door, sizeof(t_game_message), 0);
}

void	use_door_in_view(t_game *game)
{
	const t_player	*player = game->player;
	t_door			*door;
	int				x;
	int				y;

	if (game->menu->message != CLOSE_DOOR && game->menu->message != OPEN_DOOR)
		return ;
	x = (int)(player->x + player->dir_x * 1);
	y = (int)(player->y + player->dir_y * 1);
	door = get_door(game->door, x, y, player->floor);
	if (door)
	{
		door->open = !door->open;
		send_door_msg(game, door);
	}
}

void	add_door(t_game *game, int x, int y, int floor)
{
	t_door	*new_door;

	new_door = gc_malloc(game->mem, sizeof(t_door));
	new_door->x = x;
	new_door->y = y;
	new_door->floor = floor;
	new_door->open = false;
	new_door->animation = 0;
	new_door->next = game->door;
	game->door = new_door;
}

static float	calculate_perp_wall_dist(t_raycast *r, t_game *game)
{
	if (r->side == SIDE_EAST || r->side == SIDE_WEST)
		return ((r->map_x - game->player->x + (1 - r->step_x) * 0.5)
			/ r->ray_dir_x);
	return ((r->map_y - game->player->y + (1 - r->step_y) * 0.5)
		/ r->ray_dir_y);
}

static void	calculate_draw_limits(t_raycast *doorcast, t_game *game,
		float perp_wall_dist)
{
	doorcast->line_height = (int)(game->screen_height / perp_wall_dist);
	doorcast->draw_start = game->screen_height * 0.5
		- doorcast->line_height * 0.5
		- (int)(game->player->height * doorcast->line_height);
	doorcast->draw_end = doorcast->draw_start + doorcast->line_height - 1;
	if (doorcast->draw_start < 0)
		doorcast->draw_start = 0;
	if (doorcast->draw_end >= game->screen_height)
		doorcast->draw_end = game->screen_height - 1;
}

static float	calculate_wall_x(t_raycast *r, t_game *game, float perp_dist)
{
	float	wall_x;

	if (r->side == SIDE_EAST || r->side == SIDE_WEST)
		wall_x = game->player->y + perp_dist * r->ray_dir_y;
	else
		wall_x = game->player->x + perp_dist * r->ray_dir_x;
	return (wall_x - floor(wall_x));
}

void	draw_door(t_game *game, t_raycast *r)
{
	t_image		*texture;
	t_raycast	doorcast;
	float		perp_wall_dist;
	float		wall_x;

	texture = game->textures->door;
	perp_wall_dist = calculate_perp_wall_dist(r, game);
	calculate_draw_limits(&doorcast, game, perp_wall_dist);
	wall_x = calculate_wall_x(r, game, perp_wall_dist);
	game->wall_distances[r->x] = perp_wall_dist;
	doorcast.x = r->x;
	draw_vertical_line_with_texture(game, &doorcast, texture, wall_x);
}

void	update_door_animation(t_game *game)
{
	t_door	*current;

	current = game->door;
	if (!current)
		return ;
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
	const float	anim = door->animation;

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

void	handle_door2(t_game *game, t_door *door, float distance,
t_raycast *raycast)
{
	if (visible_door(door))
		draw_door(game, raycast);
	if (distance < 0.4)
	{
		if (door->open)
			game->menu->message = CLOSE_DOOR;
		else
			game->menu->message = OPEN_DOOR;
	}
}

int	handle_door(t_game *game, t_raycast *raycast)
{
	t_door		*door;
	float		distance;

	distance = raycast->perp_wall_dist;
	if (raycast->side == SIDE_EAST || raycast->side == SIDE_WEST)
		distance = (raycast->map_x - game->player->x + (1 - raycast->step_x)
				* 0.5) / raycast->ray_dir_x;
	else
		distance = (raycast->map_y - game->player->y + (1 - raycast->step_y)
				* 0.5) / raycast->ray_dir_y;
	if (game->map[game->player->floor][raycast->map_y][raycast->map_x] == 'D')
	{
		door = get_door(game->door, raycast->map_x, raycast->map_y,
				game->player->floor);
		if (!door)
			return (0);
		handle_door2(game, door, distance, raycast);
		if (visible_door(door))
			return (1);
	}
	return (0);
}
