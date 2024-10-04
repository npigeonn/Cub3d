/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floodfill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:17:49 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/04 16:20:42 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// int    check_path3(char **map, t_pos pos, t_pos pos2)
// {
// 	const int    x = pos.x;
// 	const int    y = pos.y;

// 	if (x < (int)ft_strlen(map[y]) - 2 && map[y + 1][x + 1] != '1'
// 		&& map[y][x + 1] != '1' && map[y][x + 2] != '1' && map[y - 1][x] != '1'
// 		&& map[y - 1][x + 1] != '1' && map[y - 1][x + 2] != '1'
// 		&& check_path(map, init_pos(x + 2, y), pos2))
// 		return (1);
// 	if (x > 0 && map[y + 1][x - 1] != '1' && map[y][x - 1] != '1'
// 		&& map[y][x - 2] != '1' && map[y - 1][x] != '1'
// 		&& map[y - 1][x - 1] != '1' && map[y - 1][x - 2] != '1'
// 		&& check_path(map, init_pos(x - 2, y), pos2))
// 		return (1);
// 	return (0);
// }

// int    check_path2(char **map, t_pos pos, t_pos pos2)
// {
// 	const int    x = pos.x;
// 	const int    y = pos.y;

// 	if ((map[y - 1][x + 1] != '1' && map[y - 1][x] != 'L')
// 		&& (map[y - 1][x] != '1' && map[y - 1][x + 1] != 'L')
// 		&& check_path(map, init_pos(x + 1, y - 1), pos2))
// 		return (1);
// 	if (y > 0 && x > 0 && map[y - 1][x - 1] != '1'
// 		&& map[y - 1][x] != '1'
// 		&& check_path(map, init_pos(x - 1, y - 1), pos2))
// 		return (1);
// 	if (x < (int)ft_strlen(map[y]) - 1 && map[y][x + 1] != '1'
// 		&& check_path(map, init_pos(x + 1, y), pos2))
// 		return (1);
// 	if (x > 0 && map[y][x - 1] != '1'
// 		&& check_path(map, init_pos(x - 1, y), pos2))
// 		return (1);
// 	return (check_path3(map, pos, pos2));
// }


// int    check_valid_path_collectable(t_game *data, t_pos pos,
// t_collectable *collectable)
// {
// 	char    **map;
// 	int        error;

// 	error = 0;
// 	map = copy_map(data);
// 	if (!check_path(map, pos, init_pos(collectable->x,
// 				collectable->y)))
// 		error = 1;
// 	pos.y = collectable->y;
// 	pos.x = collectable->x;
// 	free_map(data, map);
// 	return (error);
// }

// int    check_valid_path(t_game *data)
// {
	
// 	char            **map;
// 	t_pos            pos;
// 	t_collectable    *collectable;
// 	int                error;

// 	error = 0;
// 	pos.x = data->player->x / 100;
// 	pos.y = data->player->y / 100;
// 	get_collectable(data);
// 	collectable = data->collectables;
// 	while (collectable != NULL && !error)
// 	{
// 		error = check_valid_path_collectable(data, pos, collectable);
// 		collectable = collectable->next;
// 	}
// 	map = copy_map(data);
// 	if (!check_path(map, pos, init_pos(data->map->exit_x, data->map->exit_y)))
// 		error = 1;
// 	free_map(data, map);
// 	if (error)
// 		return (error_map(data, "Invalid path to collectable or exit"));
// 	return (1);
// }

int    check_path(t_game *game, int x, int y)
{
	if (game->map[0][y][x] == 'e')
		return (1);
	game->map[0][y][x] = 'X';
	if (game->map[0][y][x + 1] != 'X' && game->map[0][y][x + 1] != '1')
		return (check_path(game, x + 1, y));
	if (game->map[0][y][x - 1] != 'X' && game->map[0][y][x - 1] != '1')
		return (check_path(game, x - 1, y));
	if (game->map[0][y + 1][x] != 'X' && game->map[0][y + 1][x] != '1')
		return (check_path(game, x, y + 1));
	if (game->map[0][y - 1][x] != 'X' && game->map[0][y - 1][x] != '1')
		return (check_path(game, x, y - 1));
	return (0);
	
}

void	search_departure_position(t_game *game)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			k = 0;
			while (game->map[i][j][k])
			{
				if (ft_strchr("EWNS", game->map[i][j][k]))
				{
					game->player->x = k;
					game->player->y = j;
					return ;
				}
				k++;
			}
			j++;
		}
		i++;
	}
}

void	floodfill(t_game *game)
{
	search_departure_position(game);
	if (!check_path(game, game->player->x, game->player->y))
		exit(err("No exit...\n"));
	
	
}