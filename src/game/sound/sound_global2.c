/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_globale2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:09:26 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 11:09:26 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	music_launch(t_game *game)
{
	InitAudioDevice();
	if (pthread_create(&game->thread, NULL, test_music, game) != 0)
		gc_exit(game->mem, err("pb music\n"));
}
