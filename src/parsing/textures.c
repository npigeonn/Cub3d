/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:35:09 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/10 10:47:22 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_a_color(char *color)
{
	char **split;

	split = ft_split(color, ',');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
		return (0); // free_split(split);
	if (ft_strlen(split[0]) > 4 || ft_strlen(split[1]) > 4 
		|| ft_strlen(split[2]) > 4
		|| !(0 <= ft_atoi(split[0]) && ft_atoi(split[0]) <= 255)
		|| !(0 <= ft_atoi(split[1]) && ft_atoi(split[1]) <= 255)
		|| !(0 <= ft_atoi(split[2]) && ft_atoi(split[2]) <= 255))
	{
		// free_split(split);
		exit(err("Texture's color is not right"));
	}
	// TODO  split dans une couleur pour le Yann
	return (1);
}

int	file_dot_xpm(char *file_textre)
{
	int	i;

	i = 0;
	while (file_textre[i])
		i++;
	if (i < 5)
		return (0);
	
	return (!ft_strcmp(".xpm", file_textre + i - 4));
}

int	set_up_txtr(t_game *game, char *line)
{
	char	**split;
	int		fd;

	split = ft_split(line + 2, ' ');
	if (split[0][ft_strlen(split[0]) - 1] == '\n')
		split[0][ft_strlen(split[0]) - 1] = '\0';
	if (!split || !split[0] || split[1])
	{
		// free_split(split);
		exit(err("Texture cannot be opened\n"));
	}
	if (is_a_color(split[0]))
		return (1); // free_split(split);
	printf("la texture = %s\n", split[0]);
	if (!file_dot_xpm(split[0]))
		exit(err("Texture without .xpm\n"));// free_split(split);
	fd = open(split[0], O_RDONLY);
	if (fd <= 0)
		exit(err("Impossible to open this texture\n"));// free_split(split);
	close(fd);
	fd = open(split[0], O_RDONLY | O_DIRECTORY);
	if (fd > -1)
		exit(err("Is a folder\n"));// free_split(split);
	close(fd);
	load_txtre_globale(game, line, split[0]);
	return (1); // free_split(split);
}


int	txtre_or_nline(char *line, t_game *game)
{
	if (!line)
		return (0);
	// printf(" la comp a%sa et NO , %d\n", line, ft_strncmp(line, "F ", 2));
	if (!ft_strncmp(line, "C ", 2) && set_up_txtr(game, line))
		return ((game->textures->c)++, 1);
	else if (!ft_strncmp(line, "NO ", 3) && set_up_txtr(game, line))
		return ((game->textures->no)++, 1);
	else if (!ft_strncmp(line, "SO ", 3) && set_up_txtr(game, line))
		return ((game->textures->so)++, 1);
	else if (!ft_strncmp(line, "EA ", 3) && set_up_txtr(game, line))
		return ((game->textures->ea)++, 1);
	else if (!ft_strncmp(line, "WE ", 3) && set_up_txtr(game, line))
		return ((game->textures->we)++, 1);
	else if (!ft_strncmp(line, "F ", 2) && set_up_txtr(game, line))
		return ((game->textures->f)++, 1);
	else if (!ft_strcmp(line, "\n"))
		return (1);
	else
		return (0);
}

void	init_texture(t_game *game)
{
	game->textures->so = 0;
	game->textures->no = 0;
	game->textures->c = 0;
	game->textures->f = 0;
	game->textures->ea = 0;
	game->textures->we = 0;
}

void	load_txtre_globale(t_game *game, char *line, char *path)
{
	if (!path || path[0] == '\n')
		return ;
	if (!ft_strncmp(line, "C ", 2))
		load_texture(game, game->textures->ceil, path);
	else if (!ft_strncmp(line, "NO ", 3))
		load_texture(game, game->textures->north, path);
	else if (!ft_strncmp(line, "SO ", 3))
		load_texture(game, game->textures->sud, path);
	else if (!ft_strncmp(line, "EA ", 3))
		load_texture(game, game->textures->east, path);
	else if (!ft_strncmp(line, "F ", 2) && printf("ok\n"))
		load_texture(game, game->textures->floor, path);
	else if (!ft_strncmp(line, "WE ", 3))
		load_texture(game, game->textures->west, path);
	else
		return ;
	
}

int	textures(char *to_open, t_game *game)
{
	int		fd;
	char	*line;
	int		begin;

	init_texture(game);
	begin = 0;
	fd = open(to_open, O_RDONLY);
	line = get_next_line(fd);
	while (txtre_or_nline(line, game))
	{
		
		line = switch_line(line, fd);
		begin++;
	}
	free(line);
	close(fd);
	if (game->textures->so != 1 || game->textures->no != 1
		|| game->textures->we != 1 || game->textures->c != 1
		|| game->textures->f != 1 || game->textures->ea != 1)
		exit(err("Texture problem\n"));
	return (begin);
}
