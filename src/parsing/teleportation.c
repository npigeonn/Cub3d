/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleportation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:45:09 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/09 10:25:20 by npigeon          ###   ########.fr       */
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
				if (game->map[i][j][k] == c)
					count++;
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
	if (c == 'w')
		return (check_walls(game, x, y, floor));
	if (c == 'p')
		return (check_path(game, x, y, floor));
	return (0);
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
				if (game->map_cy[i][j][k] == to_find
					&& printf("game->map_cy[%d][%d][%d]\n", i, j, k))
						return (teleport_changment(game, k, j, i, c));
		}
	}
	return (1);
}
