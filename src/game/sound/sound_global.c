/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_global.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:53:52 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/08 11:59:53 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	*test_music(void *arg)
{ // tir - mort d'un ennemi - bruit qd degats - game over - success - teleportations - portes 
	t_game *game = (t_game *)arg;
	
	InitAudioDevice();
	Music music = LoadMusicStream("./assets/sound/sound.wav");
	PlayMusicStream(music);

	while (1)
	{
		UpdateMusicStream(music);
		sleep(0.5);
	}
	UnloadMusicStream(music);
	CloseAudioDevice();
}

void	music_launch(t_game *game)
{
	if (pthread_create(game->thread, \
		NULL, test_music, game) != 0)
		gc_exit(game->mem, err("pb music\n"));
}
