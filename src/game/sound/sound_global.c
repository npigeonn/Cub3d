/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_global.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:58:52 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/13 11:36:22 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../includes/cub3d.h"

// void	*test_music(void *arg)
// { // tir - mort d'un ennemi - bruit qd degats - game over - success - teleportations - portes 
// 	t_game *game = (t_game *)arg;
// 	// Sound	sound; "./assets/sound/gunshot.wav"
// 	Music music;
	
// 	printf("1\n");
// 	InitAudioDevice();
// 	music = LoadMusicStream("./assets/sound/ps.wav");
// 	printf("2\n");
// 	PlayMusicStream(music);
// 	while (1)
// 	{
// 		UpdateMusicStream(music);
// 		sleep(0.5);
// 	}
// 	UnloadMusicStream(music);
// 	CloseAudioDevice();
// }

// void	music_launch(t_game *game)
// {
// 	if (pthread_create(&game->thread, \
// 		NULL, test_music, game) != 0)
// 		gc_exit(game->mem, err("pb music\n"));
// }


#include "../../includes/cub3d.h"

void	unload_mu(t_block_info *param, t_game *game)
{
	UnloadMusicStream(*(Music *)(param->ptr));
	UnloadSound(*(Sound *)(param->ptr2));
	UnloadSound(*(Sound *)(param->ptr3));
	UnloadSound(*(Sound *)(param->ptr4));
	UnloadSound(*(Sound *)(param->ptr5));
	CloseAudioDevice();
}

void	play_sound_mine(Sound s, int *it)
{
	PlaySound(s);
	*it = 0;
}

void	*test_music(void *arg)
{
	t_game	*game = (t_game *)arg;
	t_block_info *block;
	Music	music;
	Sound	gunshot;
	Sound	ammo;
	Sound	pain;
	Sound	life;
	Sound	telep;

	pthread_mutex_lock(&game->mutex_music);
	music = LoadMusicStream("./assets/sound/ps.wav");
	gunshot = LoadSound("./assets/sound/gunshot.wav");
	ammo = LoadSound("./assets/sound/pickupammo.wav");
	pain =  LoadSound("./assets/sound/dsplpain.wav");
	life =  LoadSound("./assets/sound/lifeup.wav");
	telep = LoadSound("./assets/sound/teleport.wav");
	PlayMusicStream(music);
	block = gc_malloc(game->mem, sizeof(t_block_info));
	block->ptr = &music;
	block->ptr2 = &gunshot;
	block->ptr3 = &ammo;
	block->ptr4 = &pain;
	block->ptr5 = &life;
	block->ptr6 = &telep;
	// gc_add_memory_block(game->mem, &music, unload_mu, block);
	pthread_mutex_unlock(&game->mutex_music);
	while (1)
	{
		pthread_mutex_lock(&game->mutex_music);
		UpdateMusicStream(music);
		if (game->player->picking_up_ammo)
			play_sound_mine(ammo, &game->player->picking_up_ammo);
		if (game->player->is_shooting)
			play_sound_mine(gunshot, &game->player->is_shooting);
		if (game->player->injured)
			play_sound_mine(pain, &game->player->injured);
		if (game->player->life_up)
			play_sound_mine(life, &game->player->life_up);
		if (game->player->telep_signal)
			play_sound_mine(telep, &game->player->telep_signal);
		if (!game->is_running)
		{
			pthread_mutex_unlock(&game->mutex_music);
			break ;
		}
		pthread_mutex_unlock(&game->mutex_music);
		sleep(0.2);
	}
}

void	music_launch(t_game *game)
{
	InitAudioDevice();
	if (pthread_create(&game->thread, NULL, test_music, game) != 0)
		gc_exit(game->mem, err("pb music\n"));
}
