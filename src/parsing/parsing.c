/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:44:55 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/24 16:42:10 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
	Quand tu parse il faut que tu ajoutes les portes a la liste chaine de game->door,
	pour sa tu peux utiliser la fonction
	add_door(t_game *game, int x, int y, bool lock);
	(lock veux dire qu'il faut une clef pour l'ouvrir)
	tu peux tester le resultat directement en jeux les portes sont fonctionnelle sauf
	les lock qui peuvent etre ouverte sans clef


	PLUSIEURS SORTIES...	
	des teleporteurs jumellees
	nb_keys >= nb_door_locks
	nb_n_s_e_w >= nb_players
*/

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

static int	op_in(char **av)
{
	int	fd;
	if (!av[1])
		exit(err("No map mentionned\n"));
	if (av[2])
		exit(err("Too many arguments\n"));
	if (!file_dot_cub(av[1]))
		exit(err("File without .cub\n"));
	fd = open(av[1], O_RDONLY);
	if (fd <= 0)
		exit(err("Impossible to open this file\n"));
	close(fd);
	fd = open(av[1], O_RDONLY | O_DIRECTORY);
	if (fd > -1)
		exit(err("Is a folder\n"));
	close(fd);
	return (0);
}

void	check_char_n_walls(t_game *game)
{
	int i[3];

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
					return (free_map_copy(game),
						exit(err("Need walls all around the playable map\n")));
			}
		}
	}
	free_map_copy(game);
}

void	parsing(char **av, t_game *game)
{
	op_in(av);
	init_data(game);
	textures(av[1], game);
	map_set_up(av, game);
	teleportation_check(game);
	check_char_n_walls(game);
	floodfill(game);
	door_ennemi_ammo_health_mngmt(game);
}
