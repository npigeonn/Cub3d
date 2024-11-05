/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:45:22 by ybeaucou          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/cub3d.h"

void draw_vertical_sprite_band(t_game *game, int x, int draw_start, int draw_end, t_image *texture, int tex_x, int sprite_height, int anim)
{
	if (x < 0 || x >= game->screen_width || draw_start >= game->screen_height || draw_end < 0)
		return;

	if (draw_start < 0) draw_start = 0;
	if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;

	float step = (float)texture->sprite_height / sprite_height;
	float tex_pos = (draw_start - game->screen_height * 0.5f + sprite_height * 0.5f + game->player->height * sprite_height) * step;

	int bpp_div_8 = texture->bpp / 8;
	char *texture_data = texture->data;
	for (int y = draw_start; y < draw_end; y++)
	{
		int tex_y = (int)tex_pos % texture->sprite_height;
		tex_pos += step;
		int color = *((int *)(texture_data + (tex_y + texture->sprite_height * anim) * texture->size_line + tex_x * bpp_div_8));
		if (color >= 0)
			pixel_put(game, x, y, color);
	}
}

void	draw_sprite(t_game *game, t_image *texture, float sprite_x, float sprite_y, float sprite_dir, float scale, float z_offset, int anim)
{
	int sprite_order[8] = {5, 6, 7, 3, 2, 1, 0, 4};

	float relative_sprite_x = sprite_x - game->player->x;
	float relative_sprite_y = sprite_y - game->player->y;

	float inv_det = 1.0f / (game->player->planeX * game->player->dir_y - game->player->dir_x * game->player->planeY);

	float transform_x = inv_det * (game->player->dir_y * relative_sprite_x - game->player->dir_x * relative_sprite_y);
	float transform_y = inv_det * (-game->player->planeY * relative_sprite_x + game->player->planeX * relative_sprite_y);

	if (transform_y <= 0)
		return;
	float player_angle = atan2(game->player->dir_y, game->player->dir_x);
	float relative_angle = sprite_dir - player_angle;

	if (relative_angle > M_PI) relative_angle -= 2 * M_PI;
	if (relative_angle < -M_PI) relative_angle += 2 * M_PI;

	int sprite_index = (int)((relative_angle + M_PI) / (2 * M_PI) * texture->nb_sprite) % texture->nb_sprite;
	sprite_index = sprite_order[sprite_index];
	if(sprite_index >= texture->nb_sprite)
		sprite_index = 0;

	int sprite_screen_x = (int)((game->screen_width * 0.5f) * (1 + transform_x / transform_y));

	int sprite_height = abs((int)(game->screen_height / transform_y)) * scale;
	int sprite_width = abs((int)(game->screen_height / transform_y)) * scale;

	int draw_start_y = (-sprite_height / 2) + (game->screen_height / 2) - (int)(game->player->height * sprite_height) + (int)(z_offset * sprite_height);
	int draw_end_y = (sprite_height / 2) + (game->screen_height / 2) - (int)(game->player->height * sprite_height) + (int)(z_offset * sprite_height);

	if (sprite_width <= 0 || draw_end_y <= 0 || draw_start_y >= game->screen_height)
		return;

	int stripe_start = sprite_screen_x - (sprite_width / 2);
	int stripe_end = sprite_screen_x + (sprite_width / 2);

	for (int stripe = stripe_start; stripe < stripe_end; stripe++)
	{
		if (stripe >= 0 && stripe < game->screen_width && transform_y < game->wall_distances[stripe])
		{
			int tex_x_base = (int)((stripe - stripe_start) * texture->sprite_width / sprite_width);
			int tex_x = tex_x_base + sprite_index * texture->sprite_width;
			if (tex_x_base >= 0 && tex_x < texture->width)
				draw_vertical_sprite_band(game, stripe, draw_start_y, draw_end_y, texture, tex_x, sprite_height, anim);
		}
	}
}

float	calculate_distance(t_sprite *sprite, float camX, float camY, int player_floor)
{
	float sprite_x, sprite_y;

	if (sprite->type == SPRITE_TELEPORTER)
	{
		if (sprite->floor == player_floor)
		{
			sprite_x = sprite->x;
			sprite_y = sprite->y;
		} else
		{
			sprite_x = sprite->x1;
			sprite_y = sprite->y1;
		}
	} else
	{
		sprite_x = sprite->x;
		sprite_y = sprite->y;
	}

	return (sqrtf((sprite_x - camX) * (sprite_x - camX) + (sprite_y - camY) * (sprite_y - camY)));
}

void	sort_sprites(t_sprite **head, float camX, float camY, int player_floor)
{
	if (*head == NULL || (*head)->next == NULL)
		return;

	bool swapped = true;
	t_sprite *current;
	t_sprite *prev = NULL;
	t_sprite *next_sprite;

	while (swapped)
	{
		swapped = false;
		prev = NULL;
		current = *head;

		while (current->next != NULL)
		{
			next_sprite = current->next;
			float distance_current = calculate_distance(current, camX, camY, player_floor);
			float distance_next = calculate_distance(next_sprite, camX, camY, player_floor);
			if (distance_current < distance_next)
			{
				if (prev == NULL)
					*head = next_sprite;
				else
					prev->next = next_sprite;
				current->next = next_sprite->next;
				next_sprite->next = current;
				swapped = true;
				prev = next_sprite;
			}
			else
			{
				prev = current;
				current = current->next;
			}
		}
	}
}

int is_player_in_front(t_sprite *enemy, t_player *player)
{
	const float	angle_to_player = atan2(player->y - enemy->y, player->x - enemy->x);
	const float	enemy_angle = atan2(enemy->dir_y, enemy->dir_x);
	const float	angle_difference = fmod(angle_to_player - enemy_angle + M_PI, 2 * M_PI) - M_PI;

	return (fabs(angle_difference) < M_PI / 4);
}

void	draw_sprites(t_game *game)
{
	t_sprite	*current;

	sort_sprites(&game->sprites, game->player->x, game->player->y, game->player->floor);
	current = game->sprites;
	while (current)
	{
		if (current->type == SPRITE_TELEPORTER)
		{
			if (current->floor == game->player->floor)
				draw_sprite(game, game->textures->tp, current->x, current->y, 150, 0.4, 1, 0);
			if (current->floor1 == game->player->floor)
				draw_sprite(game, game->textures->tp, current->x1, current->y1, 150, 0.4, 1, 0);
		}
		if (current->type == SPRITE_EXIT && current->floor == game->player->floor)
			draw_sprite(game, game->textures->exit, current->x + 0.5, current->y + 0.5, 150, 1.5, 0.005, 0);
		else if (current->type == SPRITE_ENEMY && current->floor == game->player->floor)
		{
			if (current->health <= 0)
				draw_sprite(game, game->textures->enemy_death, current->x, current->y, 0, 1, 0, current->selected_anim);
			else if (current->state == CHASE && is_player_in_front(current, game->player))
				draw_sprite(game, game->textures->enemy_fire, current->x, current->y, atan2(current->dir_y, current->dir_x), 1, 0, current->selected_anim);
			else
				draw_sprite(game, game->textures->enemy, current->x, current->y, atan2(current->dir_y, current->dir_x), 1, 0, current->selected_anim);
		}
		else if (current->type == SPRITE_AMMO && current->still_exist && current->floor == game->player->floor)
			draw_sprite(game, game->textures->ammo, current->x, current->y, 150, 0.2, 2.1, 0);
		else if (current->type == SPRITE_HEALTH && current->still_exist && current->floor == game->player->floor)
			draw_anim_health(game, current->x, current->y, game->textures->health);
		else if (current->type == SPRITE_PLAYER && current->floor == game->player->floor)
			draw_sprite(game, game->textures->enemy, current->x, current->y, atan2(current->dir_y, current->dir_x), 1, 0, 0);
		current = current->next;
	}
}
