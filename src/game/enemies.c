/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:20:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/11 13:43:41 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	add_enemies(t_game *game, int x, int y, int floor)
{
	t_enemies	*new;

	new = malloc(sizeof(t_enemies));
	new->x = x + 0.5;
	new->y = y + 0.5;
	new->floor = floor;
	new->health = 20;
	new->dirX = 0;
	new->dirY = -1;
	new->next = game->enemies;
	game->enemies = new;
}

bool	is_position_valid(t_game *game, float x, float y, int floor)
{
	if (!game->map[floor])
		return (false);
	if (!game->map[floor][(int)y])
		return (false);
	if (ft_strlen(game->map[floor][(int)y] < (size_t)x) || !game->map[floor][(int)y][(int)x]) //TODO: Segfault
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
	t_enemies	*current;

	current = game->enemies;
	while (current)
	{
		if (current->floor == game->player->floor)
			draw_sprite(game, game->textures->enemies, current->x, current->y);
		current = current->next;
	}
}

void	update_enemies(t_game *game)
{
	t_enemies	*current = game->enemies;

	while (current)
	{
		int direction = rand() % 4;
		float move_speed = 0.1f;
		if (direction == 0 && is_position_valid(game, current->x, current->y - move_speed, current->floor))
			current->y -= move_speed;
		else if (direction == 1 && is_position_valid(game, current->x, current->y + move_speed, current->floor))
			current->y += move_speed;
		else if (direction == 2 && is_position_valid(game, current->x - move_speed, current->y, current->floor))
			current->x -= move_speed;
		else if (direction == 3 && is_position_valid(game, current->x + move_speed, current->y, current->floor))
			current->x += move_speed;
	//
	current = current->next;
	}
}
