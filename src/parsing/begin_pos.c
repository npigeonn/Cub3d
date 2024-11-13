/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   begin_pos.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:46:03 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/07 16:46:03 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	begin_player_position(t_game *game, int i, int j, int k)
{
	if (game->map[i][j][k] == 'N')
		game->player->begin_dir = 0;
	else if (game->map[i][j][k] == 'S')
		game->player->begin_dir = 1;
	else if (game->map[i][j][k] == 'E')
		game->player->begin_dir = 2;
	else if (game->map[i][j][k] == 'W')
		game->player->begin_dir = 3;
	game->player->x = k + 0.5;
	game->player->y = j + 0.5;
	game->player->floor = i;
}

void	search_departure_position(t_game *game)
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
				if (ft_strchr("EWNS", game->map[i][j][k]))
				{
					begin_player_position(game, i, j, k);
					return ;
				}
			}
		}
	}
	game->is_running = false;
	gc_exit(game->mem, err("No player found\n"));
}

int	check_allowed_char(t_game *game, int floor, int x, int y)
{
	const int	c = game->map_cy[floor][y][x];

	if (!c || !(ft_strchr("01NSWEDMBHX", c) || ('a' <= c && c <= 'z')))
	{
		printf("%c\n", c);
		game->is_running = false;
		gc_exit(game->mem, err("Not allowed character\n"));
		return (0);
	}
	return (1);
}
