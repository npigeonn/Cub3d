/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleportation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:45:09 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/08 20:12:18 by npigeon          ###   ########.fr       */
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
