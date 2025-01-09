/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_global.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:58:52 by npigeon           #+#    #+#             */
/*   Updated: 2025/01/08 16:31:22 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	unload_mu(t_block_info *param, t_game *game)
{
	UnloadMusicStream(*(Music *)(param->ptr));
	UnloadSound(*(Sound *)(param->ptr2));
	UnloadSound(*(Sound *)(param->ptr3));
	UnloadSound(*(Sound *)(param->ptr4));
	UnloadSound(*(Sound *)(param->ptr5));
	CloseAudioDevice();
	(void)game;
}

void	play_sound_mine(Sound s, int *it)
{
	PlaySound(s);
	*it = 0;
}

void	play_sound_global(t_game *game, t_sound *sound, Music music)
{
	while (1)
	{
		pthread_mutex_lock(&game->mutex_music);
		UpdateMusicStream(music);
		if (game->player->picking_up_ammo)
			play_sound_mine(sound->ammo, &game->player->picking_up_ammo);
		if (game->player->is_shooting)
			play_sound_mine(sound->gunshot, &game->player->is_shooting);
		if (game->player->injured)
			play_sound_mine(sound->pain, &game->player->injured);
		if (game->player->life_up)
			play_sound_mine(sound->life, &game->player->life_up);
		if (game->player->telep_signal)
			play_sound_mine(sound->telep, &game->player->telep_signal);
		if (!game->is_running)
		{
			pthread_mutex_unlock(&game->mutex_music);
			break ;
		}
		pthread_mutex_unlock(&game->mutex_music);
		usleep(200000);
	}
}

void	loading_sound(t_sound *sound)
{
	sound->gunshot = LoadSound("./assets/sound/gunshot.wav");
	sound->ammo = LoadSound("./assets/sound/pickupammo.wav");
	sound->pain = LoadSound("./assets/sound/dsplpain.wav");
	sound->life = LoadSound("./assets/sound/lifeup.wav");
	sound->telep = LoadSound("./assets/sound/teleport.wav");
}

void	*test_music(void *arg)
{
	t_game			*game;
	t_block_info	*block;
	Music			music;
	t_sound			*sound;

	game = (t_game *)arg;
	sound = gc_malloc(game->mem, sizeof(t_sound));
	pthread_mutex_lock(&game->mutex_music);
	music = LoadMusicStream("./assets/sound/ps.wav");
	loading_sound(sound);
	PlayMusicStream(music);
	block = gc_malloc(game->mem, sizeof(t_block_info));
	block->ptr = &music;
	block->ptr2 = &sound->gunshot;
	block->ptr3 = &sound->ammo;
	block->ptr4 = &sound->pain;
	block->ptr5 = &sound->life;
	block->ptr6 = &sound->telep;
	pthread_mutex_unlock(&game->mutex_music);
	play_sound_global(game, sound, music);
	return (NULL);
}
