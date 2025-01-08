/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:09:52 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 16:45:39 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "cub3d.h"
# include <sys/stat.h>

int			join_server(t_game *game);
void		*discover_servers_thread(void *arg);
void		*receive_updates(void *args);
void		update_door(t_game *game, t_game_message msg);
void		add_connection_msg(t_game *game, char *pseudo, t_game_message msg);
void		remove_player(t_game *game, int player_id);
void		add_player(t_game *game, t_game_message msg);
void		update_player_position(t_game *game, t_game_message msg);
t_draw_info	init_draw_info(int height, char *str, int x, int y);

// map
int			receive_map_from_server(t_game *game, int client_socket);
void		replace_path(t_game *game, char *filename);
void		create_dir_if_not_exists(void);
int			receive_data(size_t file_size, int server_socket, FILE *file);

// player
void		update_player_health(t_game *game, t_game_message msg);
void		destroy_sprite(t_game *game, t_game_message msg);

#endif