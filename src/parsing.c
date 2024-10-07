/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:44:55 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/07 10:47:09 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	err(char *str)
{
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

// void	verif_only_one(char *line)
// {
// 	while(*line)
// 	{
// 		if (ft_strcmp(*line, "1"))
// 			exit(err("Need walls all around\n"));
// 		line++;
// 	}
// }

// void	verif_component_line(char *line, int *n_w_s_e)
// {
// 	if (ft_strcmp(*line, "1"))
// 		exit(err("Need walls all around\n"));
// 	while (*line)
// 	{
// 		if (!ft_strcmp(*line, "N") || !ft_strcmp(*line, "S")\
// 			|| !ft_strcmp(*line, "W") || !ft_strcmp(*line, "E"))
// 		{
// 			(*n_w_s_e)++;
// 			line++;
// 			continue;
// 		}
// 		if (!(ft_strcmp(*line, "0") || ft_strcmp(*line, "1")\
// 			|| ft_strcmp(*line, "2") || ft_strcmp(*line, "u")\
// 			|| ft_strcmp(*line, "D") || ft_strcmp(*line, "d")))
// 			exit(err("Unkown character\n"));
// 		line++;
// 	}
// 	line--;
// 	if (ft_strcmp(*line, "1"))
// 		exit(err("Need walls all around\n"));
// }

char	*switch_line(char *line, int fd)
{
	free(line);
	return (get_next_line(fd));
}

int	nb_floors(char **av)
{
	int		fd;
	int		i;
	char	*line;

	i = 0;
	fd = open(av[1], O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		while (line && line[0] == '\n')
			line = switch_line(line, fd);
		if (line && line[0] != '\n')
		{
			i++;
			while (line && line[0] != '\n')
				line = switch_line(line, fd);
		}
	}
	free(line);
	close(fd);
	return i;
}

void	size_floors(char **av, t_game *game, int floor)
{
	int		j;
	int		fd;
	int		i;
	char	*line;

	fd = open(av[1], O_RDONLY);
	line = get_next_line(fd);
	j = 0;
	while (j < floor)
	{
		i = 0;
		while (line && line[0] == '\n')
			line = switch_line(line, fd);
		while (line && line[0] != '\n')
		{
			i++;
			line = switch_line(line, fd);
		}
		game->map[j] = malloc((i + 1) * sizeof(char *));
		if (!game->map[j])
			exit(1); // free line !free_tab(floor, game, j, 0)
		j++;
	}
	game->map[j] = NULL;
	close(fd);
}

void	map_ready(char **av, t_game *game, int floor)
{
	int		j;
	int		fd;
	int		i;
	char	*line;

	fd = open(av[1], O_RDONLY);
	line = get_next_line(fd);
	j = 0;
	while (j < floor)
	{
		i = 0;
		while (line && line[0] == '\n')
			line = switch_line(line, fd);
		while (line && line[0] != '\n')
		{
			game->map[j][i] = ft_strdup(line);
			if (!game->map[j][i])
				exit(1); // free line !free_tab(floor, game, j, i)
			i++;
			line = switch_line(line, fd);
		}
		j++;
	}
	close(fd);
}

void	map_set_up(char **av, t_game *game)
{
	int	floor;
	int	k;

	floor = nb_floors(av);
	if (!floor)
		exit(err("Empty\n"));
	printf("nbre d'etage = %d\n", floor);
	k = 0;
	
	game->map = malloc((floor + 1) * sizeof(char **));
	if (!game->map)
		exit(err("error system\n"));
	size_floors(av, game, floor);
	map_ready(av, game, floor);
	floodfill(game);
	
}

void	parsing(char **av, t_game *game)
{
	// int	fd;
	char *line;
	int n_w_s_e;

	n_w_s_e = 0;
	op_in(av);
	map_set_up(av, game);

	// access to /regarder les permissions
}
