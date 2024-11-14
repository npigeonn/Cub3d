/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:09:52 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/14 11:09:54 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "cub3d.h"

int			join_server(t_game *game);
void		*discover_servers_thread(void *arg);
void		*receive_updates(void *args);
void		update_door(t_game *game, t_game_message msg);
void		add_connection_msg(t_game *game, char *pseudo);
void		remove_player(t_game *game, int player_id);
void		add_player(t_game *game, t_game_message msg);
void		update_player_position(t_game *game, t_game_message msg);
t_draw_info	init_draw_info(int height, char *str, int x, int y);

#endif