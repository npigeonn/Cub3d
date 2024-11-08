/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floodfill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:17:49 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/08 09:37:38 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	check_walls(t_game *game, int x, int y, int floor)
{
	if (x < 0 || y < 0 || !game->map_cy[floor][y]
		|| x >= (int)ft_strlen(game->map_cy[floor][y]))
		return (0);
	if (!check_allowed_char(game, floor, x, y))
		return (0);
	if (game->map_cy[floor][y][x] == 'X' || game->map_cy[floor][y][x] == '1')
		return (1);
	if (is_a_teleporter(game->map_cy[floor][y][x])
		&& !teleportation(game, x, y, floor))
		return (0);
	game->map_cy[floor][y][x] = 'X';
	if (!check_walls(game, x + 1, y, floor))
		return (0);
	if (!check_walls(game, x - 1, y, floor))
		return (0);
	if (!check_walls(game, x, y + 1, floor))
		return (0);
	if (!check_walls(game, x, y - 1, floor))
		return (0);
	return (1);
}

int	check_path(t_game *game, int x, int y, int floor)
{
	if (x < 0 || y < 0 || !game->map_cy[floor][y]
		|| x >= (int)ft_strlen(game->map_cy[floor][y]))
		return (0);
	if (game->map_cy[floor][y][x] == 'e')
		return (1);
	if (game->map_cy[floor][y][x] == 'X' || game->map_cy[floor][y][x] == '1')
		return (0);
	if (is_a_teleporter(game->map_cy[floor][y][x])
		&& teleportation(game, x, y, floor))
		return (1);
	game->map_cy[floor][y][x] = 'X';
	if (check_path(game, x + 1, y, floor))
		return (1);
	if (check_path(game, x - 1, y, floor))
		return (1);
	if (check_path(game, x, y + 1, floor))
		return (1);
	if (check_path(game, x, y - 1, floor))
		return (1);
	return (0);
}

int	count_spawns(t_game *game)
{
	int	i;
	int	j;
	int	k;
	int	count;

	count = 0;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			k = -1;
			while (game->map[i][j][++k])
				if (ft_strchr("EWNS", game->map[i][j][k]))
					count++;
		}
	}
	return (count);
}

int	map_copy(t_game *game)
{
	int	i;
	int	j;

	i = -1;
	game->map_cy = gc_malloc(game->mem, (game->nb_floor + 1)
			* sizeof(char **));
	if (!game->map_cy)
		gc_exit(game->mem, err("error system\n"));
	while (game->map[++i])
	{
		j = 0;
		while (game->map[i][j])
			j++;
		game->map_cy[i] = gc_malloc(game->mem, (j + 1) * sizeof(char *));
		if (!game->map_cy[i])
			gc_exit(game->mem, err("error system\n"));
		game->map_cy[i][j] = NULL;
		while (--j >= 0)
		{
			game->map_cy[i][j] = gc_strdup(game->mem, game->map[i][j]);
			if (!game->map_cy[i][j])
				return (gc_exit(game->mem, err("error system\n")), 1);
		}
	}
	return (game->map_cy[i] = NULL, 1);
}

void	floodfill(t_game *game)
{
	search_departure_position(game);
	if (!map_copy(game)
		|| !check_path(game, game->player->x, game->player->y,
			game->player->floor))
		gc_exit(game->mem, err("No exit...\n"));
	free_map_copy(game);
}
