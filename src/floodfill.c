/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floodfill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:17:49 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/08 16:59:50 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/cub3d.h"

int	check_allowed_char(t_game *game, int floor, int x, int y)
{
	const int	c = game->map_cy[floor][y][x];
	if (!c || !(ft_strchr("01NSWEDLKMHX", c) || ('a' <= c && c <= 'z')))
		return (printf("map[0][%d][%d]\n", x, y), 0);
	return (1);
}

int	check_walls(t_game *game, int x, int y, int floor)
{
	if (x < 0 || y < 0 || !game->map_cy[floor][y]
		|| x >= (int)ft_strlen(game->map_cy[floor][y]))
		return (free_map_copy(game), 0);
	if (!check_allowed_char(game, 0, x, y))
		return (free_map_copy(game), 0);
	if (game->map_cy[floor][y][x] == 'X' || game->map_cy[floor][y][x] == '1')
		return (1); // Already visited or wall
	// if (game->map_cy[floor][y][x])
	game->map_cy[floor][y][x] = 'X'; // Mark visited
	if (!check_walls(game, x + 1, y, floor))
		return (0); // Right
	if (!check_walls(game, x - 1, y, floor))
		return (0); // Left
	if (!check_walls(game, x, y + 1, floor))
		return (0); // Down
	if (!check_walls(game, x, y - 1, floor))
		return (0); // Up
	return (1);
}

int	check_path(t_game *game, int x, int y, int floor)
{
	if (x < 0 || y < 0 || !game->map_cy[floor][y]
		|| x >= (int)ft_strlen(game->map_cy[floor][y]))
		return (free_map_copy(game), 0);
	if (game->map_cy[floor][y][x] == 'e') 
		return (free_map_copy(game), 1); // Exit found
	if (game->map_cy[floor][y][x] == 'X' || game->map_cy[floor][y][x] == '1')
		return (0); // Already visited or wall
	game->map_cy[floor][y][x] = 'X'; // Mark visited
	if (check_path(game, x + 1, y, floor))
		return (1); // Right
	if (check_path(game, x - 1, y, floor))
		return (1); // Left
	if (check_path(game, x, y + 1, floor))
		return (1); // Down
	if (check_path(game, x, y - 1, floor))
		return (1); // Up
	return (0); // No path found
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
					game->player->floor = i;
					return;
				}
			}
		}
	}
	exit(err("No player found\n"));
}

int map_copy(t_game *game)
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
	game->map_cy[i] = NULL;
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

void	door_mngmt(t_game *game)
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
				if (game->map[i][j][k] == 'L')
					add_door(game, k, j, i, true);
				else if (game->map[i][j][k] == 'D')
					add_door(game, k, j, i, false);
			}
		}
	}
}

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

void floodfill(t_game *game)
{
	search_departure_position(game);
	if (!map_copy(game) || !check_walls(game, game->player->x, game->player->y, game->player->floor))
		exit(err("Need walls all around the playable map\n"));
	if (!map_copy(game) || !check_path(game, game->player->x, game->player->y, game->player->floor))
		exit(err("No exit...\n"));
	door_mngmt(game);
	teleportation_check(game);
		
}
