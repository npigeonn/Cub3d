/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_global.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:53:52 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/12 11:49:54 by npigeon          ###   ########.fr       */
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

void	unload_mu(t_block_info *param)
{
	UnloadMusicStream(*(Music *)(param->ptr));
	UnloadSound(*(Sound *)(param->ptr2));
	UnloadSound(*(Sound *)(param->ptr3));
	UnloadSound(*(Sound *)(param->ptr4));
	UnloadSound(*(Sound *)(param->ptr5));
	CloseAudioDevice();
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

	InitAudioDevice();
	music = LoadMusicStream("./assets/sound/ps.wav");
	gunshot = LoadSound("./assets/sound/gunshot.wav");
	ammo = LoadSound("./assets/sound/pickupammo.wav");
	pain =  LoadSound("./assets/sound/dsplpain.wav");
	life =  LoadSound("./assets/sound/lifeup.wav");
	PlayMusicStream(music);
	block = gc_malloc(game->mem, sizeof(t_block_info));
	block->ptr = &music;
	block->ptr2 = &gunshot;
	block->ptr3 = &ammo;
	block->ptr4 = &pain;
	block->ptr5 = &life;
	gc_add_memory_block(game->mem, &music, unload_mu, block);
	while (1)
	{
		UpdateMusicStream(music);
		if (game->player->picking_up_ammo)
		{
			PlaySound(ammo);
			game->player->picking_up_ammo = 0;
		}
		if (game->player->is_shooting)
		{
			PlaySound(gunshot);
			game->player->is_shooting = 0;  // Réinitialisation pour permettre un prochain tir
		}
		if (game->player->injured)
		{
			PlaySound(pain);
			game->player->injured = 0;
		}
		if (game->player->life_up)
		{
			PlaySound(life);
			game->player->life_up = 0;			
		}
		sleep(0.2);
	}
}

void	music_launch(t_game *game)
{
	if (pthread_create(&game->thread, NULL, test_music, game) != 0)
		gc_exit(game->mem, err("pb music\n"));
}
