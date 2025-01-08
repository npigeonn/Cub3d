/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:44:55 by npigeon           #+#    #+#             */
/*   Updated: 2025/01/08 03:36:33 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	err(char *str)
{
	write(2, "Error\n", 6);
	while (*str)
		write(2, str++, 1);
	return (1);
}

int	file_dot_cub(char *file_map)
{
	int	i;

	i = 0;
	while (file_map[i])
		i++;
	if (i < 5)
		return (0);
	return (!ft_strcmp(".cub", file_map + i - 4));
}

static int	op_in(char **av, t_game *game)
{
	int	fd;
	int	i;

	if (!av[1] || !av[2] && (!ft_strcmp(av[1], "--volume=true")
			|| !ft_strcmp(av[1], "--volume=false")))
		gc_exit(game->mem, err("No map mentionned\n"));
	if (av[2] && ft_strcmp(av[1], "--volume=true")
		&& ft_strcmp(av[1], "--volume=false"))
		gc_exit(game->mem, err("Too many arguments\n"));
	if (!ft_strcmp(av[1], "--volume=true"))
		game->music_dif = 1;
	game->arg_map = 1;
	if (av[2])
		game->arg_map = 2;
	if (!file_dot_cub(av[game->arg_map]))
		gc_exit(game->mem, err("File without .cub\n"));
	fd = open(av[game->arg_map], O_RDONLY);
	if (fd <= 0)
		gc_exit(game->mem, err("Impossible to open this file\n"));
	close(fd);
	fd = open(av[game->arg_map], O_RDONLY | O_DIRECTORY);
	if (fd > -1)
		return (close(fd), gc_exit(game->mem, err("Is a folder\n")), 1);
	return (0);
}

void	check_char_n_walls(t_game *game)
{
	int	i[3];

	i[0] = -1;
	map_copy(game);
	while (game->map_cy[++i[0]])
	{
		i[1] = -1;
		while (game->map_cy[i[0]][++i[1]])
		{
			i[2] = -1;
			while (game->map_cy[i[0]][i[1]][++i[2]])
			{
				if (game->map_cy[i[0]][i[1]][i[2]] == '1'
					|| game->map_cy[i[0]][i[1]][i[2]] == 'X'
					|| game->map_cy[i[0]][i[1]][i[2]] == ' '
					|| game->map_cy[i[0]][i[1]][i[2]] == '	'
					|| game->map_cy[i[0]][i[1]][i[2]] == '\n')
					continue ;
				if (!check_walls(game, i[2], i[1], i[0]))
					return (free_map_copy(game), gc_exit(game->mem,
							err("Need walls all around the playable map\n")));
			}
		}
	}
	free_map_copy(game);
}

void	parsing(char **av, t_game *game)
{
	op_in(av, game);
	init_data(game);
	textures(av[game->arg_map], game);
	map_set_up(av, game);
	if (count_spawns(game) != 1)
		gc_exit(game->mem, err("Need ONE spawn\n"));
	teleportation_check(game);
	game->check_w = 1;
	check_char_n_walls(game);
	game->check_w = 0;
	floodfill(game);
	door_ennemi_ammo_health_mngmt(game);
	printf("color floor: %d\n", game->textures->color_c);
}
