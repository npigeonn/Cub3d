/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:06:26 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/14 09:24:12 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <pthread.h>
# include <sys/epoll.h>

# define MAX_PLAYERS 4
# define MAX_PSEUDO_LENGTH 32
# define PORT 12345
# define BROADCAST_PORT 12345

typedef enum s_message_type
{
	MSG_CONNECT = 0,
	MSG_RECONNECT = 1,
	MSG_MOVE = 2,
	MSG_DISCONNECT = 3,
	MSG_FULL = 4,
	MSG_BROADCAST = 5,
	MSG_DOOR = 6,
	MSG_CHAT = 7,
	MSG_GET_PLAYER = 8,
	MSG_FILE_SIZE = 9,
	MSG_BROADCAST_ENEMIES = 10,
	MSG_PLAYER_HIT = 11,
	MSG_PLAYER_SHOOT = 12,
	MSG_DESTROY_SPRITE = 13,
}	t_message_type;

typedef struct s_game_message
{
	t_message_type	type;
	int				player_id;
	char			pseudo[MAX_PSEUDO_LENGTH];
	float			x;
	float			y;
	float			dir_x;
	float			dir_y;
	float			plane_x;
	float			plane_y;
	float			height;
	float			health;
	int				floor;
	int				open;
	int				ammo;
	char			message[MAX_MESSAGE_LENGTH];
	int				file_size;
	float			animation;
	int				selected_anim;
	int				state;
}	t_game_message;

typedef struct s_game_message_queue
{
	t_game_message				message;
	struct s_game_message_queue	*next;
}	t_game_message_queue;

typedef struct s_server
{
	int						sock_bc;
	char					*ip;
	char					**av;
	char					name[20];
	int						nb_player;
	int						client_sockets[MAX_PLAYERS];
	bool					server_ready;
	pthread_mutex_t			*game_lock;
	t_game_message_queue	*game_queue;
	int						server_fd;
	int						epoll_fd;
	int						addrlen;
	struct sockaddr_in		address;
	char					***map;
	float					x;
	float					y;
	int						floor;
	float					delta_time;
	struct timeval			last_time;
	t_door					*door;
	t_sprite				*sprites;
	t_projectile			*projectiles;
	t_memory_table			*mem;
	pthread_t				logic_game_thread;
	pthread_t				main_server_thread;
	pthread_mutex_t			mutex;
	t_sprite				*current_enemy;
	bool					stop;
}	t_server;

//broadcast
int			init_broadcast(t_server *server);

//find player
t_sprite	*find_player_by_pseudo(t_server *server, char *pseudo);
t_sprite	*find_player_by_id(t_sprite *players, int id);
t_sprite	*get_target_player(t_server *server, float *min_distance, t_sprite *current_enemy);

//logic
void		*logic_game(void *arg);
void		loop_server(t_server *server);
void		create_server(t_game *game);
void		game_multi_death(t_game *game);
void		game_engine(t_game *game);
void		handle_client_receive_msg(t_server *server, int client_socket);

//new player
void		new_player(t_server *server, int new_socket, char *pseudo);
char		*existing_player(t_server *server, int new_socket);

//notification
void		notify_players_of_connection(t_server *server, int player_id,
				char *pseudo);
void		notify_players_of_reconnection(t_server *server, int player_id,
				char *pseudo);
void		notify_players_of_disconnection(t_server *server, int id);
void		notify_players(t_server *server, t_game_message msg);

//playe node
void		add_player_node(t_server *server, int id, char *pseudo);
void		update_player_node(t_server *server, char *pseudo,
				t_game_message msg);
void		notify_all_players(t_server *server, t_game_message msg);
int			reconnect_player(t_server *server, t_sprite *player,
				int socket);

//queue
void		add_game_message_to_queue(t_server *server, t_game_message msg);

//send
void		send_all_players(t_server *server, int id);
void		send_reconnected_message(t_server *server, t_sprite *player,
				char *pseudo);
void		send_info(t_server *server, t_game_message msg);
void		send_enemies(t_server *server, t_game_message msg);

// projectile
void		shoot_at_player_server(t_sprite *enemy, t_sprite *player_pos,
				t_server *server);
void		add_projectile(t_server *server, t_game_message msg);
void		update_projectiles_server(t_server *server);

// enemy
void		update_enemies_server(t_server *server);

void		set_anim(t_game *game);
void		create_join_server(t_game *game);

//utils
float		distance_squared(float x1, float y1, float x2, float y2);

#endif