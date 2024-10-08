/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleportation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:45:09 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/10 11:01:20 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_a_teleporter(char c)
{
	if (c != 'e' && 'a' <= c && c <= 'z')
		return (1);
	return (0);
}

void is_a_valid_teleporter(char c, t_game *game)
{
	int	i;
	int	j;
	int	k;
	int count;

	if (!is_a_teleporter(c))
		return ;
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
				if (game->map[i][j][k] == c)
				{
					if (count == 0)
						add_teleporter(game, k, j, i);
					if (count == 1)
						set_output_teleporter(game, k, j, i);
					count++;
				}
			}
		}
	}
	if (count != 2)
		exit(err("teleporter's issue\n"));
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
				is_a_valid_teleporter(game->map[i][j][k], game);
		}
	}
}

int teleport_changment(t_game *game, int x, int y, int floor, char c)
{
	if (c == 'w' && !check_walls(game, x, y, floor))
		return (0);
	if (c == 'p' && check_path(game, x, y, floor))
		return (1);
	if (c == 'p')
		return (0);
	else
		return (1);
}
int	teleportation(t_game *game, int x, int y, int floor, char c)
{
	int	i;
	int	j;
	int	k;
	char to_find;

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
					return (game->map_cy[i][j][k] = '0', teleport_changment(game, k, j, i, c));
		}
	}
	return (1);
}
