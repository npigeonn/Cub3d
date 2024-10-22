/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:35:09 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/22 08:55:29 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_a_color(char *color, t_game *game)
{
	char **split;

	split = ft_split(color, ',');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
		return (free_split(split), 0);
	if (ft_strlen(split[0]) > 4 || ft_strlen(split[1]) > 4 
		|| ft_strlen(split[2]) > 4
		|| !(0 <= ft_atoi(split[0]) && ft_atoi(split[0]) <= 255)
		|| !(0 <= ft_atoi(split[1]) && ft_atoi(split[1]) <= 255)
		|| !(0 <= ft_atoi(split[2]) && ft_atoi(split[2]) <= 255))
	{
		free_split(split);
		exit(err("Texture's color is not right"));
	}
    game->clr = (atoi(split[0]) << 16) | (atoi(split[1]) << 8) | atoi(split[2]);
	free_split(split);
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

	game->clr = -1;
	split = ft_split(line + 2, ' ');
	if (split[0][ft_strlen(split[0]) - 1] == '\n')
		split[0][ft_strlen(split[0]) - 1] = '\0';
	if (!split || !split[0] || split[1])
		return (exit(err("Texture cannot be opened\n")), 0);// free_split(split);
	if (is_a_color(split[0], game))
		return (free_split(split), 1);
	if (!file_dot_xpm(split[0]))
		return (free_split(split), exit(err("Texture without .xpm\n")), 0);
	fd = open(split[0], O_RDONLY);
	if (fd <= 0)
		return (free_split(split), exit(err("Can't open this texture\n")), 0);
	close(fd);
	fd = open(split[0], O_RDONLY | O_DIRECTORY);
	if (fd > -1)
		return (free_split(split), exit(err("Is a folder\n")), 0);
	close(fd);
	load_txtre_globale(game, line, split[0]);
	return (free_split(split), 1);
}


int	txtre_or_nline(char *line, t_game *game)
{
	if (!line)
		return (0);
	// printf(" la comp a%sa et NO , %d\n", line, ft_strncmp(line, "F ", 2));
	if (!ft_strncmp(line, "C ", 2) && set_up_txtr(game, line))
		return (game->textures->color_c = game->clr, game->textures->c++, 1);
	else if (!ft_strncmp(line, "NO ", 3) && set_up_txtr(game, line))
		return (game->textures->color_no = game->clr, game->textures->no++, 1);
	else if (!ft_strncmp(line, "SO ", 3) && set_up_txtr(game, line))
		return (game->textures->color_so = game->clr, game->textures->so++, 1);
	else if (!ft_strncmp(line, "EA ", 3) && set_up_txtr(game, line))
		return (game->textures->color_ea = game->clr, game->textures->ea++, 1);
	else if (!ft_strncmp(line, "WE ", 3) && set_up_txtr(game, line))
		return (game->textures->color_we = game->clr, game->textures->we++, 1);
	else if (!ft_strncmp(line, "F ", 2) && set_up_txtr(game, line))
		return (game->textures->color_f = game->clr, game->textures->f++, 1);
	else if (!ft_strcmp(line, "\n"))
		return (1);
	else
		return (0);
}

void	init_data(t_game *game)
{
	game->textures->so = 0;
	game->textures->no = 0;
	game->textures->c = 0;
	game->textures->f = 0;
	game->textures->ea = 0;
	game->textures->we = 0;
	game->textures->color_so = -1;
	game->textures->color_no = -1;
	game->textures->color_c = -1;
	game->textures->color_f = -1;
	game->textures->color_ea = -1;
	game->textures->color_we = -1;
	game->map_begin = 0;
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
		load_texture(game, game->textures->south, path);
	else if (!ft_strncmp(line, "EA ", 3))
		load_texture(game, game->textures->east, path);
	else if (!ft_strncmp(line, "F ", 2))
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

	fd = open(to_open, O_RDONLY);
	line = get_next_line(fd);
	while (txtre_or_nline(line, game))
	{
		
		line = switch_line(line, fd);
		game->map_begin++;
	}
	free(line);
	close(fd);
	if (game->textures->so != 1 || game->textures->no != 1
		|| game->textures->we != 1 || game->textures->c != 1
		|| game->textures->f != 1 || game->textures->ea != 1)
		exit(err("Texture problem\n"));
	return (1);
}
