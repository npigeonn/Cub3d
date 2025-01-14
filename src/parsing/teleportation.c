/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleportation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:45:09 by npigeon           #+#    #+#             */
/*   Updated: 2025/01/14 10:48:11 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_a_teleporter(char c)
{
	if (c != 'e' && 'a' <= c && c <= 'z')
		return (1);
	return (0);
}

int	is_a_valid_teleporter(char c, t_game *game)
{
	int	i;
	int	j;
	int	k;
	int	count;

	i = -1;
	count = 0;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			k = -1;
			while (game->map[i][j][++k])
			{
				if (game->map[i][j][k] == c && count == 0)
					add_teleporter(game, k, j, i);
				else if (game->map[i][j][k] == c && count == 1)
					set_output_teleporter(game, k, j, i);
				if (game->map[i][j][k] == c)
					count++;
			}
		}
	}
	return (count);
}

void	teleportation_check(t_game *game)
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
				if (is_a_teleporter(game->map[i][j][k])
					&& is_a_valid_teleporter(game->map[i][j][k], game) != 2)
					gc_exit(game->mem, err("teleporter's issue\n"));
		}
	}
}

int	teleport_changment(t_game *game, int x, int y, int floor)
{
	if (game->check_w && !check_walls(game, x, y, floor))
		return (0);
	if (!game->check_w && check_path(game, x, y, floor))
		return (1);
	if (!game->check_w)
		return (0);
	else
		return (1);
}

int	teleportation(t_game *game, int x, int y, int floor)
{
	int		i;
	int		j;
	int		k;
	char	to_find;

	to_find = game->map_cy[floor][y][x];
	game->map_cy[floor][y][x] = '0';
	i = -1;
	while (game->map_cy[++i])
	{
		j = -1;
		while (game->map_cy[i][++j])
		{
			k = -1;
			while (game->map_cy[i][j][++k])
				if (game->map_cy[i][j][k] == to_find)
					return (game->map_cy[i][j][k] = '0',
							teleport_changment(game, k, j, i));
		}
	}
	return (1);
}
