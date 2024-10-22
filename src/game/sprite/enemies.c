/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:20:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/22 08:36:22 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_enemies(t_game *game, int x, int y, int floor)
{
	t_enemy	*new;

	new = malloc(sizeof(t_enemy));
	new->x = x + 0.5;
	new->y = y + 0.5;
	new->floor = floor;
	new->health = 20;
	new->dirX = 0;
	new->dirY = 1;
	new->next = game->enemies;
	new->state = PATROL;
	new->direction = 200;
	new->frame_count = 0;
	new->fov = 60;
	game->enemies = new;
}

bool	is_position_valid(t_game *game, float x, float y, int floor)
{
	if (!game->map[floor])
		return (false);
	if (!game->map[floor][(int)y])
		return (false);
	if (ft_strlen(game->map[floor][(int)y]) < (size_t)x || !game->map[floor][(int)y][(int)x]) //TODO: Segfault
		return (false);
	if (game->map[floor][(int)y][(int)x] != '1' && game->map[floor][(int)y][(int)x] != 'D' && game->map[floor][(int)y][(int)x] != 'L')
		return (true);
	return (false);
}

int	get_number_pos(t_game *game, int floor)
{
	int	x;
	int	y;
	int	nb_pos;

	y = -1;
	nb_pos = 0;
	while(game->map[floor][++y])
	{
		x = -1;
		while(game->map[floor][y][++x])
		{
			if (is_position_valid(game, x, y, floor) && !ft_strchr("ENSW", game->map[floor][y][x]))
				nb_pos++;
		}
	}
	return (nb_pos);
}

int	**get_valid_position(t_game *game, int floor, int total_pos)
{
	int	x;
	int	y;
	int	nb_pos;
	int	**pos;

	pos = malloc((total_pos + 1) * sizeof(int *));
	y = -1;
	nb_pos = 0;

	while (game->map[floor][++y])
	{
		x = -1;
		while (game->map[floor][y][++x])
		{
			if (is_position_valid(game, x, y, floor) && !ft_strchr("ENSW", game->map[floor][y][x]))
			{
				pos[nb_pos] = malloc(2 * sizeof(int));
				pos[nb_pos][0] = x;
				pos[nb_pos][1] = y;
				nb_pos++;
			}
		}
	}
	pos[nb_pos] = NULL;
	return (pos);
}

void	init_enemies(t_game *game)
{
	int	i;

	i = -1;
	while (game->map[++i])
	{
		rand();
		int	**pos;
		int	nb_enemies = ft_rand(2, 6);
		int	j;
		int	nb_pos = get_number_pos(game, i);

		pos = get_valid_position(game, i, nb_pos);
		j = -1;
		while (++j < nb_enemies)
		{
			int *pos_e = pos[ft_rand(0, nb_pos - 1)];
			add_enemies(game, pos_e[0], pos_e[1], i);
		}
	}
}

void	draw_enemies(t_game *game)
{
	t_enemy	*current;

	current = game->enemies;
	while (current)
	{
		if (current->floor == game->player->floor)
			draw_sprite(game, game->textures->enemies, current->x, current->y, atan2(current->dirY, current->dirX), 1, 0);
		current = current->next;
	}
}

void	draw_players(t_game *game)
{
	t_player_info	*current;

	current = game->client->players;
	while (current)
	{
		if (current->floor == game->player->floor)
			draw_sprite(game, game->textures->enemies, current->x, current->y, 150, 1, 0);
		current = current->next;
	}
}
bool	is_position_passable(t_game *game, float x, float y, int floor)
{
	int tile_x = (int)(x);
	int tile_y = (int)(y);

	if (tile_x < 0 || tile_y < 0 || tile_x >= (int)ft_strlen(game->map[floor][tile_y]))
		return (false);
	if (game->map[floor][tile_y][tile_x] == 'D' || game->map[floor][tile_y][tile_x] == '1')
		return (false);
	return (true);
}

bool	has_line_of_sight(t_game *game, t_point enemy_pos, t_point player_pos, float enemy_facing_angle, float fov_angle)
{
	float dx = player_pos.x - enemy_pos.x;
	float dy = player_pos.y - enemy_pos.y;
	float distance = sqrt(dx * dx + dy * dy);

	float angle_to_player = atan2(dy, dx) * (180.0f / M_PI);
	float angle_diff = fabsf(angle_to_player - enemy_facing_angle);

	if (angle_diff <= fov_angle * 0.5)
	{
		float step_x = dx / distance;
		float step_y = dy / distance;
		for (float t = 0; t < distance; t += 0.1f)
		{
			enemy_pos.x += step_x * 0.1f;
			enemy_pos.y += step_y * 0.1f;
			if (!is_position_passable(game, enemy_pos.x, enemy_pos.y, enemy_pos.floor))
				return (false);
		}
		return (true);
	}
	return (false);
}

void	update_enemies(t_game *game)
{
	t_enemy *current = game->enemies;
	t_point player_pos = {game->player->x, game->player->y, game->player->floor};

	while (current)
	{
		t_point enemy_pos = {current->x, current->y, current->floor};
		
		float dx = player_pos.x - current->x;
		float dy = player_pos.y - current->y;
		float distance_squared = dx * dx + dy * dy;
		
		if (current->state == PATROL)
		{
			if (current->frame_count % 220 == 0)
				current->direction = rand() % 360;
			float angle_in_radians = current->direction * (M_PI / 180.0f);
			current->dirX = cos(angle_in_radians);
			current->dirY = sin(angle_in_radians);
			
			float new_x = current->x + current->dirX * MOVEMENT_SPEED;
			float new_y = current->y + current->dirY * MOVEMENT_SPEED;
			if (is_position_passable(game, new_x, new_y, current->floor))
			{
				current->x = new_x;
				current->y = new_y;
			}
			else
			{
				current->direction = rand() % 360;
				current->frame_count = 0;
			}
			if (distance_squared < 25.0f && has_line_of_sight(game, enemy_pos, player_pos, current->direction, current->fov))
				current->state = CHASE;
		}
		else if (current->state == CHASE)
		{
			float angle_to_player = atan2(dy, dx);
			current->dirX = cos(angle_to_player);
			current->dirY = sin(angle_to_player);
			
			float new_x = current->x + current->dirX * MOVEMENT_SPEED;
			float new_y = current->y + current->dirY * MOVEMENT_SPEED;
			if (is_position_passable(game, new_x, new_y, current->floor))
			{
				current->x = new_x;
				current->y = new_y;
			}
			else
			{
				current->direction = rand() % 360;
				current->frame_count = 0;
			}
			if (distance_squared > 64.0f || !has_line_of_sight(game, enemy_pos, player_pos, current->direction, current->fov))
				current->state = PATROL;
		}
		current->frame_count++;
		current = current->next;
	}
}
