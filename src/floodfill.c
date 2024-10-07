/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floodfill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:17:49 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/07 18:27:38 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	check_allowed_char(t_game *game, int floor, int x, int y)
{
	const int	c = game->map_cy[floor][y][x];
	if (!c || !((ft_strchr("01NSWEDLKMHX", c) || ('a' <= c && c <= 'z'))))
		return (printf("map[0][%d][%d]\n", x, y), 0);
	return (1);
}

int	check_walls(t_game *game, int x, int y)
{
	if (!check_allowed_char(game, 0, x, y))
		return (free_map_copy(game), 0);
	game->map_cy[0][y][x] = 'X';
	if (game->map_cy[0][y][x + 1] != 'X' && game->map_cy[0][y][x + 1] != '1')
		return (check_walls(game, x + 1, y));
	if (game->map_cy[0][y][x - 1] != 'X' && game->map_cy[0][y][x - 1] != '1')
		return (check_walls(game, x - 1, y));
	if (game->map_cy[0][y + 1][x] != 'X' && game->map_cy[0][y + 1][x] != '1')
		return (check_walls(game, x, y + 1));
	if (game->map_cy[0][y - 1][x] != 'X' && game->map_cy[0][y - 1][x] != '1')
		return (check_walls(game, x, y - 1));
	return (free_map_copy(game), 1);
}

int    check_path(t_game *game, int x, int y)
{
	if (game->map_cy[0][y][x] == 'e')
		return (free_map_copy(game), 1);
	game->map_cy[0][y][x] = 'X';
	if (game->map_cy[0][y][x + 1] != 'X' && game->map_cy[0][y][x + 1] != '1')
		return (check_path(game, x + 1, y));
	if (game->map_cy[0][y][x - 1] != 'X' && game->map_cy[0][y][x - 1] != '1')
		return (check_path(game, x - 1, y));
	if (game->map_cy[0][y + 1][x] != 'X' && game->map_cy[0][y + 1][x] != '1')
		return (check_path(game, x, y + 1));
	if (game->map_cy[0][y - 1][x] != 'X' && game->map_cy[0][y - 1][x] != '1')
		return (check_path(game, x, y - 1));
	return (free_map_copy(game), 0);
	
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
					game->player->x = k;
					game->player->y = j;
					return ;
				}
			}
		}
	}
	exit(err("No player found\n"));
}
// verif que tous les caracteres sont possibles
// transporteurs jumelles 
// verif si ya qu'un seul player (NSWE)
// clefs >= portes fermees
// une seule sortie



int	map_copy(t_game *game)
{
	int	i;
	int	j;

	i = -1;
	game->map_cy = malloc((game->nb_floor + 1) * sizeof(char **));
	if (!game->map_cy)
		exit(err("error system\n"));
	while (game->map[++i])
	{
		j = 0;
		while (game->map[i][j])
			j++;
		game->map_cy[i] = malloc((j + 1) * sizeof(char *));
		if (!game->map_cy[i])
			exit(err("error system\n"));
		game->map_cy[i][j] = NULL;
		while (--j >= 0)
		{
			game->map_cy[i][j] = ft_strdup(game->map[i][j]);
			if (!game->map_cy[i][j])
				exit(err("error system\n")); // free line !free_tab(floor, game, j, i)
		}
	}
	return (1);
}

int free_map_copy(t_game *game)
{
	int	i;
	int	floor;
	
	floor = -1;
	while (game->map_cy[++floor])
	{
		i = -1;
		while (game->map_cy[floor][++i])
			free(game->map_cy[floor][i]);
		free(game->map_cy[floor][i]);
	}
	free(game->map_cy);
	return (0);
}


void	floodfill(t_game *game)
{
	
	search_departure_position(game);
	if (!map_copy(game) || !check_path(game, game->player->x, game->player->y))
		exit(err("No exit...\n"));
	if (!map_copy(game) || !check_walls(game, game->player->x, game->player->y))
		exit(err("Need walls all around the playable map\n"));
	
	
}