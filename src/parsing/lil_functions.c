/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lil_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:43:01 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/07 11:49:35 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char	*switch_line(t_memory_table *mem, char *line, int fd)
{
	gc_free(mem, line);
	return (gc_get_next_line(mem, fd));
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
	game->mouse = 1;
}
