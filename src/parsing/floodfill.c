/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floodfill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:17:49 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/24 09:37:44 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	check_allowed_char(t_game *game, int floor, int x, int y)
{
	const int	c = game->map_cy[floor][y][x];
	if (!c || !(ft_strchr("01NSWEDMBHX", c) || ('a' <= c && c <= 'z')))
	{
		printf("%c\n", c);
		free_map_copy(game);
		exit(err("Not allowed character\n"));
		return (0);
	}
	return (1);
}

int	check_walls(t_game *game, int x, int y, int floor)
{
	if (x < 0 || y < 0 || !game->map_cy[floor][y]
		|| x >= (int)ft_strlen(game->map_cy[floor][y]))
		return (0); //
	if (!check_allowed_char(game, floor, x, y))
		return (0);//
	if (game->map_cy[floor][y][x] == 'X' || game->map_cy[floor][y][x] == '1')
		return (1); // Already visited or wall
	if (is_a_teleporter(game->map_cy[floor][y][x])
		&& !teleportation(game, x, y, floor, 'w'))
		return (0);// TODO peutetre modif a 0; game->map_cy[floor][y][x] = '0';
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

int	check_path(t_game *game, int x, int y, int floor) // si exit dans un autre etage ca ne marche pas 
{
	if (x < 0 || y < 0 || !game->map_cy[floor][y]
		|| x >= (int)ft_strlen(game->map_cy[floor][y]))
		return (0); // 
	if (game->map_cy[floor][y][x] == 'e') 
		return (1); //   // Exit found
	if (game->map_cy[floor][y][x] == 'X' || game->map_cy[floor][y][x] == '1')
		return (0); // Already visited or wall
	if (is_a_teleporter(game->map_cy[floor][y][x])
		&& teleportation(game, x, y, floor, 'p'))
		return (1);
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

int	count_spoons(t_game *game)
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
				if (ft_strchr("EWNS", game->map[i][j][k]))
					return (begin_player_position(game, i, j, k));
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


void floodfill(t_game *game)
{
	search_departure_position(game);
	if (!map_copy(game) || !check_path(game, game->player->x, game->player->y, game->player->floor))
	{
		free_map_copy(game);
		exit(err("No exit...\n"));
	}
	free_map_copy(game);
}
