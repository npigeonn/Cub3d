/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:43:21 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/06 08:40:00 by ybeaucou         ###   ########.fr       */
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

int	free_split(t_memory_table *mem, char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		gc_free(mem, str[i]);
	gc_free(mem, str);
	return (0);
}