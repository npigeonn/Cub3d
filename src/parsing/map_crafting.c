/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_crafting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:52:38 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/07 16:35:18 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char	*go_to_the_map_line(t_memory_table *mem, int fd, int begin)
{
	int		i;
	char	*line;

	i = -1;
	line = gc_get_next_line(mem, fd);
	while (++i < begin)
		line = switch_line(mem, line, fd);
	return (line);
}

int	nb_floors(char **av, t_game *game)
{
	int		fd;
	int		i;
	char	*line;

	i = 0;
	fd = open(av[1], O_RDONLY);
	line = go_to_the_map_line(game->mem, fd, game->map_begin);
	if (!line)
		gc_exit(game->mem, err("Empty file\n"));
	while (line)
	{
		while (line && line[0] == '\n')
			line = switch_line(game->mem, line, fd);
		if (line && line[0] != '\n')
		{
			i++;
			while (line && line[0] != '\n')
				line = switch_line(game->mem, line, fd);
		}
	}
	return (gc_free(game->mem, line), close(fd), i);
}

void	size_floors(char **av, t_game *game, int floor)
{
	int		j;
	int		fd;
	int		i;
	char	*line;

	fd = open(av[1], O_RDONLY);
	line = go_to_the_map_line(game->mem, fd, game->map_begin);
	j = 0;
	while (j < floor)
	{
		i = 0;
		while (line && line[0] == '\n')
			line = switch_line(game->mem, line, fd);
		while (line && line[0] != '\n')
		{
			i++;
			line = switch_line(game->mem, line, fd);
		}
		game->map[j] = gc_malloc(game->mem, (i + 1) * sizeof(char *));
		if (!game->map[j])
			gc_exit(game->mem, 1);
		j++;
	}
	gc_free(game->mem, line);
	close(fd);
}

void	map_ready(char **av, t_game *game, int floor)
{
	int		j;
	int		fd;
	int		i;
	char	*line;

	fd = open(av[1], O_RDONLY);
	line = go_to_the_map_line(game->mem, fd, game->map_begin);
	j = -1;
	while (++j < floor)
	{
		i = 0;
		while (line && line[0] == '\n')
			line = switch_line(game->mem, line, fd);
		while (line && line[0] != '\n')
		{
			game->map[j][i] = gc_strdup(game->mem, line);
			if (!game->map[j][i])
				gc_exit(game->mem, err("error system\n"));
			i++;
			line = switch_line(game->mem, line, fd);
		}
		game->map[j][i] = NULL;
	}
	game->map[j] = NULL;
	return ((void)gc_free(game->mem, line), (void)close(fd));
}

void	map_set_up(char **av, t_game *game)
{
	game->nb_floor = nb_floors(av, game);
	if (!game->nb_floor)
		gc_exit(game->mem, err("Empty\n"));
	game->map = gc_malloc(game->mem, (game->nb_floor + 1)
			* sizeof(char **));
	if (!game->map)
		gc_exit(game->mem, err("error system\n"));
	size_floors(av, game, game->nb_floor);
	map_ready(av, game, game->nb_floor);
}
