/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:44:55 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/03 17:33:00 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	err(char *str)
{
	while (*str)
		write(2, str++, 1);
	return (1);
}

static int	op_in(char **av)
{
	if (!av[1])
		exit(err("No map mentionned\n"));
	if (av[2])
		exit(err("Too many arguments\n"));
	if (access(av[1], F_OK) != 0)
		exit(err("This map doesn't exist\n"));
	if (access(av[1], R_OK) != 0)
		exit(err("Not allowed to open this file\n"));
	return (0);
}

void	verif_only_one(char *line)
{
	while(*line)
	{
		if (ft_strcmp(*line, "1"))
			exit(err("Need walls all around\n"));
		line++;
	}
}

void	verif_component_line(char *line, int *n_w_s_e)
{
	if (ft_strcmp(*line, "1"))
		exit(err("Need walls all around\n"));
	while (*line)
	{
		if (!ft_strcmp(*line, "N") || !ft_strcmp(*line, "S")\
			|| !ft_strcmp(*line, "W") || !ft_strcmp(*line, "E"))
		{
			(*n_w_s_e)++;
			line++;
			continue;
		}
		if (!(ft_strcmp(*line, "0") || ft_strcmp(*line, "1")\
			|| ft_strcmp(*line, "2") || ft_strcmp(*line, "u")\
			|| ft_strcmp(*line, "D") || ft_strcmp(*line, "d")))
			exit(err("Unkown character\n"));
		line++;
	}
	line--;
	if (ft_strcmp(*line, "1"))
		exit(err("Need walls all around\n"));
}

void	parsing(char **av)
{
	int	fd;
	char *line;
	int n_w_s_e;

	n_w_s_e = 0;
	op_in(av);
	fd = open(av[1], O_RDONLY);
	line = get_next_line(fd);
	verif_only_one(line);
	while (line)
	{
		verif_component_line(line, &n_w_s_e);

		free(line);
		line = get_next_line(fd);
	}
	verif_only_one(line);
	free(line);
	if (n_w_s_e != 1)
		exit(err("Need ONE initial position : N E S or W\n"));
	// access to /regarder les permissions
}
