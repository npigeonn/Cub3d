/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:43:21 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/05 11:48:16 by npigeon          ###   ########.fr       */
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
			gc_free(game->mem, game->map_cy[floor][i]);
		gc_free(game->mem, game->map_cy[floor]);
	}
	return (gc_free(game->mem, game->map_cy), 0);
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
			gc_free(game->mem, game->map[floor][i]);
		gc_free(game->mem, game->map[floor]);
	}
	gc_free(game->mem, game->map);
	return (0);
}

int	free_split(t_game *game, char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		gc_free(game->mem, str[i]);
	gc_free(game->mem, str);
	return (0);
}