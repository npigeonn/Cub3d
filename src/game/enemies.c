/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:20:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/10 13:53:44 by ybeaucou         ###   ########.fr       */
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
			if (game->map[floor][y][x] != '1' && game->map[floor][y][x] != 'D' && game->map[floor][y][x] != 'L')
				nb_pos++;
		}
	}
	return (nb_pos);
}

int **get_valid_position(t_game *game, int floor, int total_pos)
{
	int x;
	int y;
	int nb_pos;
	int **pos;

	pos = malloc((total_pos + 1) * sizeof(int *));
	y = -1;
	nb_pos = 0;

	while (game->map[floor][++y])
	{
		x = -1;
		while (game->map[floor][y][++x])
		{
			if (game->map[floor][y][x] != '1' && game->map[floor][y][x] != 'D' && game->map[floor][y][x] != 'L')
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
		int	**pos;
		int	nb_enemies = ft_rand(2, 6);
		int	j;
		int	nb_pos = get_number_pos(game, i);

		pos = get_valid_position(game, i, nb_pos);
		j = -1;
		while (++j < nb_enemies)
		{
			int *pos_e = pos[ft_rand(0, nb_pos - 1)];
			add_enemies(game, pos_e[1], pos_e[0], i);
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
			draw_sprite(game, game->textures->tp, current->x, current->y);
		current = current->next;
	}
}