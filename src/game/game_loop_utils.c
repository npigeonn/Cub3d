/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:32:18 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 12:32:18 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_hud(t_game *game)
{
	mini_map(game);
	crosshair(game);
	gun_draw(game);
	if (game->player->being_tpted)
		animation_teleportation(game);
	health_point_draw(game);
	ammo_written(game);
	if (game->player->health <= 0 && (game->menu->status == CHATING
			|| game->menu->status == MULTI_PLAYER))
		draw_dead_screen_multiplayer(game);
	calculate_fps(game);
}

void	game_multi_death(t_game *game)
{
	t_sprite	*current;

	if ((game->menu->status != MULTI_PLAYER && game->menu->status != CHATING)
		|| game->player->health > 0)
		return ;
	current = game->sprites;
	while (current)
	{
		if (current->type == SPRITE_PLAYER && current->health > 0)
		{
			game->player->x = current->x;
			game->player->y = current->y;
			game->player->floor = current->floor;
			game->player->dir_x = current->dir_x;
			game->player->dir_y = current->dir_y;
			game->player->plane_x = current->plane_x;
			game->player->plane_y = current->plane_y;
			break ;
		}
		current = current->next;
	}
}

void	set_anim(t_game *game)
{
	game->player->animation += game->delta_time;
	if (game->player->animation >= 2)
		game->player->animation = 0.5;
	if (game->player->animation >= 0.5 && game->player->animation < 1)
		game->player->selected_anim = 1;
	else if (game->player->animation >= 1 && game->player->animation < 1.5)
		game->player->selected_anim = 2;
	else if (game->player->animation >= 1.5 && game->player->animation < 2)
		game->player->selected_anim = 3;
}

void	game_engine(t_game *game)
{
	t_player	*p;

	p = game->player;
	game->menu->message = NOTHING;
	handle_key(game);
	calculate_delta_time(game);
	if (game->menu->status == PLAYING)
	{
		update_door_animation(game);
		update_enemies(game);
		update_projectiles(game);
		on_ammo(game);
		on_life(game);
	}
	set_anim(game);
	game_multi_death(game);
	render_multithreaded(game);
	chat_draw(game);
	show_message(game);
	draw_hud(game);
	if (is_a_teleporter(game->map[p->floor][(int)p->y][(int)p->x]))
		game->menu->message = TELEPORT;
	if (game->menu->message != NOTHING)
		show_menu_message(game);
}

void	create_join_server(t_game *game)
{
	if (game->menu->status == VALID_SERVER_CREATE)
	{
		create_server(game);
		ft_strcpy(game->client->ip, "127.0.0.1");
		game->menu->status = VALID_JOIN_SERVER;
	}
	else if (game->menu->status == VALID_JOIN_SERVER)
	{
		if (join_server(game) == 1)
		{
			game->menu->status = MULTI_PLAYER;
			x_fixes_cursor(game, 'h');
		}
	}
}
