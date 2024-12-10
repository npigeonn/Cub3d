/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:47:02 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 12:47:02 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
