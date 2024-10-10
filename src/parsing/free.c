/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:43:21 by npigeon           #+#    #+#             */
/*   Updated: 2024/10/10 15:55:33 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int free_map_copy(t_game *game)
{
	int	i;
	int	floor;

	floor = -1;
	while (game->map_cy[++floor])
	{
		i = -1;
		while (game->map_cy[floor][++i])
			free(game->map_cy[floor][i]);
		free(game->map_cy[floor]);
	}
	free(game->map_cy);
	return (0);
}

int free_map(t_game *game)
{
	int	i;
	int	floor;

	floor = -1;
	while (game->map[++floor])
	{
		i = -1;
		while (game->map[floor][++i])
			free(game->map[floor][i]);
		free(game->map[floor]);
	}
	free(game->map);
	return (0);
}

int	free_split(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
	return (0);
}