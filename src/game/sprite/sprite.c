/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:51:45 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/15 12:28:44 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	get_info_sprite_band(t_game *game, t_sprite *sprite, int x)
{
	t_spritecast	*sp;

	sp = sprite->spritecast;
	sp->step = (float)sp->texture->sprite_height / sp->height;
	sp->tex_pos = (sp->draw_start_y - game->cen_y + sp->height * 0.5f
			+ game->player->height * sp->height) * sp->step;
	sp->bpp = sp->texture->bpp / 8;
	sp->texture_data = sp->texture->data;
	sp->anim_frame = sprite->selected_anim % sp->texture->sprite_height;
}

void	draw_vertical_sprite_band_loop(t_game *game, t_sprite *sprite,
int x, int y)
{
	int				tex_y;
	int				color;
	t_spritecast	*sp;

	sp = sprite->spritecast;
	tex_y = (int)(sp->tex_pos) % sp->texture->sprite_height;
	sp->tex_pos += sp->step;
	if (tex_y < 0)
		tex_y = 0;
	else if (tex_y >= sp->texture->sprite_height)
		tex_y = sp->texture->sprite_height - 1;
	color = *((int *)(sp->texture_data + (tex_y + sp->texture->sprite_height
					* sp->anim_frame) * sp->texture->size_line
				+ sp->tex_x * sp->bpp));
	if (sprite->type == SPRITE_PLAYER && color >= 0)
		color = blend_colors(color, 0x51789, 0.5);
	if (color >= 0)
		pixel_put(game, x, y, color);
}

static void	draw_vertical_sprite_band(t_game *game, t_sprite *sprite, int x)
{
	t_spritecast	*sp;
	int				y;
	int				tex_y;
	int				color;

	sp = sprite->spritecast;
	if (x < 0 || x >= game->screen_width
		|| sp->draw_start_y >= game->screen_height || sp->draw_end_y < 0)
		return ;
	if (sprite->selected_anim < 0)
		sprite->selected_anim = 0;
	if (sp->draw_start_y < 0)
		sp->draw_start_y = 0;
	if (sp->draw_end_y >= game->screen_height)
		sp->draw_end_y = game->screen_height - 1;
	get_info_sprite_band(game, sprite, x);
	y = sp->draw_start_y - 1;
	while (++y < sp->draw_end_y)
		draw_vertical_sprite_band_loop(game, sprite, x, y);
}

void	init_spritecast(t_game *game, t_sprite *sprite, t_image *texture)
{
	t_spritecast	*sp;
	t_player		*p;

	sp = sprite->spritecast;
	pthread_mutex_lock(&game->mutex);
	p = game->player;
	sp->r_x = sprite->x - p->x;
	sp->r_y = sprite->y - p->y;
	pthread_mutex_unlock(&game->mutex);
	sp->inv_det = 1.0f / (p->plane_x * p->dir_y - p->dir_x * p->plane_y);
	sp->transform_x = sp->inv_det * (p->dir_y * sp->r_x - p->dir_x * sp->r_y);
	sp->transform_y = sp->inv_det
		* (-p->plane_y * sp->r_x + p->plane_x * sp->r_y);
	sp->player_angle = atan2(p->dir_y, p->dir_x);
	sp->relative_angle = sprite->dir - sp->player_angle;
	if (sp->relative_angle > M_PI)
		sp->relative_angle -= 2 * M_PI;
	if (sp->relative_angle < -M_PI)
		sp->relative_angle += 2 * M_PI;
	sp->texture = texture;
}

int	get_spritecast_info(t_game *game, t_sprite *sp, int *sprite_index,
int sprite_order)
{
	t_spritecast	*spc;

	spc = sp->spritecast;
	*sprite_index = sprite_order[sprite_index];
	if (*sprite_index >= spc->texture->nb_sprite || *sprite_index < 0)
		*sprite_index = 0;
	spc->screen_x = (int)(game->cen_x
			* (1 + spc->transform_x / spc->transform_y));
	spc->height = abs((int)(game->screen_height / spc->transform_y))
		* sp->scale;
	spc->width = abs((int)(game->screen_height / spc->transform_y))
		* sp->scale;
	spc->draw_start_y = (-spc->height / 2) + game->cen_y
		- (int)(game->player->height * spc->height)
		+ (int)(sp->z_offset * spc->height);
	spc->draw_end_y = (spc->height / 2) + game->cen_y
		- (int)(game->player->height * spc->height)
		+ (int)(sp->z_offset * spc->height);
	if (spc->width <= 0 || spc->draw_end_y <= 0
		|| spc->draw_start_y >= game->screen_height)
		return (0);
	spc->stripe_start = spc->screen_x - (spc->width / 2);
	spc->stripe_end = spc->screen_x + (spc->width / 2);
	return (1);
}

void	get_spritecast_tex_x(t_game *game, t_sprite *sprite, int stripe,
int sprite_index)
{
	t_spritecast	*sp;

	sp = sprite->spritecast;
	sp->tex_x_base = (int)((stripe - sp->stripe_start)
			* sp->texture->sprite_width / sp->width);
	sp->tex_x = sp->tex_x_base + sprite_index * sp->texture->sprite_width;
}

void	draw_player_pseudo(t_game *game, t_sprite *sprite)
{
	t_spritecast	*sp;
	float			distance;
	t_draw_info		info;
	int				height;

	sp = sprite->spritecast;
	if (sprite->type == SPRITE_PLAYER)
	{
		distance = sqrtf(sp->r_x * sp->r_x + sp->r_y * sp->r_y);
		height = game->screen_height * 0.1 / distance;
		info = init_draw_info(height, sprite->pseudo,
				sp->screen_x, sp->draw_start_y + 20);
		info.color = 0xFFFFF;
		draw_text(game, info);
	}
}

void	draw_sprite(t_game *game, t_image *texture, t_sprite *sprite)
{
	const int		sprite_order[8] = {5, 6, 7, 3, 2, 1, 0, 4};
	t_spritecast	*sp;
	int				sprite_index;
	int				stripe;

	sp = sprite->spritecast;
	init_spritecast(game, sprite, texture);
	sprite_index = (int)((sp->relative_angle + M_PI) / (2 * M_PI)
			* texture->nb_sprite) % texture->nb_sprite;
	if (sp->transform_y <= 0 || !get_spritecast_info(game, sprite,
			&sprite_index, sprite_order))
		return ;
	stripe = sp->stripe_start - 1;
	while (++stripe < sp->stripe_end)
	{
		if (stripe >= 0 && stripe < game->screen_width
			&& sp->transform_y < game->wall_distances[stripe])
		{
			get_spritecast_tex_x(game, sprite, stripe, sprite_index);
			if (sp->tex_x_base >= 0 && sp->tex_x < texture->width)
				draw_vertical_sprite_band(game, sprite, stripe);
		}
	}
	draw_player_pseudo(game, sprite);
}

float	calculate_distance(t_game *game, t_sprite *sprite)
{
	float	sprite_x;
	float	sprite_y;

	if (sprite->type == SPRITE_TELEPORTER && sprite->floor1 == game->player->y)
	{
		sprite_x = sprite->x1;
		sprite_y = sprite->y1;
	}
	else
	{
		sprite_x = sprite->x;
		sprite_y = sprite->y;
	}
	return (sqrtf((sprite_x - game->player->x) * (sprite_x - game->player->x)
			+ (sprite_y - game->player->y) * (sprite_y - game->player->y)));
}

void	sort_sprites_swap(t_sprite **prev, t_sprite **current, t_sprite **next,
int *swapped)
{
	t_sprite	*tmp;

	if (*prev)
		(*prev)->next = *next;
	tmp = *next;
	(*current)->next = (*next)->next;
	(*next)->next = *current;
	*next = *current;
	*current = tmp;
	*swapped = true;
}

void	sort_sprites_not_swap(t_sprite **prev, t_sprite **current,
t_sprite **next)
{
	*prev = *current;
	*current = (*current)->next;
	*next = (*next)->next;
}

void	sort_sprites(t_game *game, t_sprite **head)
{
	bool		swapped;
	t_sprite	*current;
	t_sprite	*prev;
	t_sprite	*next_sprite;

	prev = NULL;
	swapped = true;
	if (*head == NULL || (*head)->next == NULL)
		return ;
	while (swapped)
	{
		swapped = false;
		prev = NULL;
		current = *head;
		while (current->next != NULL)
		{
			next_sprite = current->next;
			if (calculate_distance(game, current)
				< calculate_distance(game, next_sprite))
				sort_sprites_swap(&prev, &current, &next_sprite, &swapped);
			else
				sort_sprites_not_swap(&prev, &current, &next_sprite);
		}
	}
}

int	is_player_in_front(t_sprite *enemy, t_player *player)
{
	const float	angle_to_player = atan2(player->y - enemy->y,
			player->x - enemy->x);
	const float	enemy_angle = atan2(enemy->dir_y, enemy->dir_x);
	const float	angle_difference = fmod(angle_to_player - enemy_angle
			+ M_PI, 2 * M_PI) - M_PI;

	return (fabs(angle_difference) < M_PI / 4);
}

void	draw_sprites_enemies(t_game *game, t_sprite *current)
{
	if (current->health <= 0)
	{
		current->dir = 0;
		draw_sprite(game, game->textures->enemy_death, current);
	}
	else
	{
		current->dir = atan2(current->dir_y, current->dir_x);
		if (current->state == CHASE
			&& is_player_in_front(current, game->player))
			draw_sprite(game, game->textures->enemy_fire, current);
		else
			draw_sprite(game, game->textures->enemy, current);
	}
}

void	draw_sprites2(t_game *game, t_sprite *current)
{
	if (current->type == SPRITE_ENEMY && current->floor == game->player->floor)
		draw_sprites_enemies(game, current);
	else if (current->type == SPRITE_AMMO && current->still_exist
		&& current->floor == game->player->floor)
		draw_sprite(game, game->textures->ammo, current);
	else if (current->type == SPRITE_HEALTH && current->still_exist
		&& current->floor == game->player->floor)
		draw_anim_health(game, current, game->textures->health);
	else if (current->type == SPRITE_PLAYER
		&& current->floor == game->player->floor)
	{
		current->dir = atan2(current->dir_y, current->dir_x);
		draw_sprite(game, game->textures->enemy, current);
	}
}

void	draw_sprites(t_game *game)
{
	t_sprite	*current;

	// sort_sprites(game, &game->sprites); TODO: Fix this
	current = game->sprites;
	while (current)
	{
		if (game->menu->status == MULTI_PLAYER || game->menu->status == CHATING)
			pthread_mutex_lock(&game->game_lock);
		if (current->type == SPRITE_TELEPORTER)
		{
			if (current->floor == game->player->floor)
				draw_sprite(game, game->textures->tp, current);
			if (current->floor1 == game->player->floor)
				draw_sprite(game, game->textures->tp, current);
		}
		else if (current->type == SPRITE_EXIT
			&& current->floor == game->player->floor)
			draw_sprite(game, game->textures->exit, current);
		else
			draw_sprites2(game, current);
		current = current->next;
		if (game->menu->status == MULTI_PLAYER || game->menu->status == CHATING)
			pthread_mutex_unlock(&game->game_lock);
	}
}
