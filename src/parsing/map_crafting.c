/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_crafting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:52:38 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/10 16:27:53 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char *go_to_the_map_line(int fd, int begin)
{
	int		i;
	char	*line;

	i = -1;
	line = get_next_line(fd);
	while (++i < begin)
		line = switch_line(line, fd);
	return (line);
}

char	*switch_line(char *line, int fd)
{
	free(line);
	return (get_next_line(fd));
}

int	nb_floors(char **av, t_game *game)
{
	int		fd;
	int		i;
	char	*line;

	i = 0;
	fd = open(av[1], O_RDONLY);
	line = go_to_the_map_line(fd, game->map_begin);
	if (!line)
		exit(err("Empty file\n"));
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
	return (free(line), close(fd), i);
}

void	size_floors(char **av, t_game *game, int floor)
{
	int		j;
	int		fd;
	int		i;
	char	*line;

	fd = open(av[1], O_RDONLY);
	line = go_to_the_map_line(fd, game->map_begin);
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
	free(line);
	close(fd);
}

void	map_ready(char **av, t_game *game, int floor)
{
	int		j;
	int		fd;
	int		i;
	char	*line;

	fd = open(av[1], O_RDONLY);
	line = go_to_the_map_line(fd, game->map_begin);
	j = -1;
	while (++j < floor)
	{
		i = 0;
		while (line && line[0] == '\n')
			line = switch_line(line, fd);
		while (line && line[0] != '\n')
		{
			game->map[j][i] = ft_strdup(line);
			if (!game->map[j][i])
				exit(err("error system\n")); // free line !free_tab(floor, game, j, i)
			i++;
			line = switch_line(line, fd);
		}
		game->map[j][i] = NULL;
	}
	game->map[j] = NULL;
	return ((void)free(line), (void)close(fd));
}

void	map_set_up(char **av, t_game *game)
{
	game->nb_floor = nb_floors(av, game);
	if (!game->nb_floor)
		exit(err("Empty\n"));
	game->map = malloc((game->nb_floor + 1) * sizeof(char **));
	if (!game->map)
		exit(err("error system\n"));
	size_floors(av, game, game->nb_floor);
	map_ready(av, game, game->nb_floor);
}
