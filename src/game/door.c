/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:16:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/10 12:45:26 by ybeaucou         ###   ########.fr       */
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
	new_door->animation = 0;
    new_door->next = game->door;
    game->door = new_door;
}

void	draw_door(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side, t_door *door)
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
	{
		game->wall_distances[x] = perp_wall_dist;
		draw_vertical_line_with_texture(game, x, animated_draw_start, draw_end, texture, wall_x, line_height);
	}
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

int	handle_door(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side, float distance)
{
	t_door	*door;

	if (game->map[game->player->floor][map_y][map_x] == 'D')
	{
		door = get_door(game, map_x, map_y, game->player->floor);
		if (!door)
			return (0);
		draw_door(game, x, map_x, map_y, step_x, step_y, ray_dir_x, ray_dir_y, side, door);
		if (distance < 0.4f)
		{
			if (door->open)
				game->menu->message = CLOSE_DOOR;	
			else
				game->menu->message = OPEN_DOOR;
		}
	}
	return (0);
}

void	door_mngmt(t_game *game)
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
				if (game->map[i][j][k] == 'L')
					add_door(game, k, j, i, true);
				else if (game->map[i][j][k] == 'D')
					add_door(game, k, j, i, false);
			}
		}
	}
}
