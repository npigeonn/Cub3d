
#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/epoll.h>

#define MAX_PLAYERS 4
#define MAX_PSEUDO_LENGTH 32
#define PORT 12345
#define BROADCAST_PORT 12345

typedef struct t_player_node
{
	char					pseudo[MAX_PSEUDO_LENGTH];
	t_player_info			player;
	struct t_player_node	*next;
}	t_player_node;

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
}	t_message_type;

typedef struct s_game_message
{
	t_message_type	type;
	int			player_id;
	char		pseudo[MAX_PSEUDO_LENGTH];
	float		x;
	float		y;
	float		dir_x;
	float		dir_y;
	float		height;
	float		health;
	int			floor;
	int			open;
	char		message[MAX_MESSAGE_LENGTH];
}	t_game_message;

typedef struct s_move_message
{
	int		player_id;
	float	new_x;
	float	new_y;
}	MoveMessage;

typedef struct	s_game_message_queue
{
	t_game_message				message;
	struct s_game_message_queue	*next;
}	t_game_message_queue;

typedef struct s_server
{
	int						sock_bc;
	char					*ip;
	char					name[20];
	int						nb_player;
	t_player_info			*players;
	int						client_sockets[MAX_PLAYERS];
	bool					server_ready;
	pthread_mutex_t			*game_lock;
	t_game_message_queue	*game_queue;
	int						server_fd;
	int						epoll_fd;
	int						addrlen;
	struct sockaddr_in		address;
}	t_server;

//broadcast
int				init_broadcast(t_server *server);

//find player
t_player_info	*find_player_by_pseudo(t_server *server, char *pseudo);
t_player_info	*find_player_by_id(t_player_info *players, int id);

//logic
void	*logic_game(void *arg);

//new player
void	new_player(t_server *server, int new_socket, char *pseudo);
char	*existing_player(t_server *server, int new_socket);

//notification
void	notify_players_of_connection(t_server *server, int player_id, char *pseudo);
void	notify_players_of_reconnection(t_server *server, int player_id, char *pseudo);
void	notify_players_of_disconnection(t_server *server, int id);
void	notify_players_of_move(t_server *server, t_game_message msg);
void	notify_players_of_door(t_server *server, t_game_message msg);
void	notify_players_of_chat(t_server *server, t_game_message msg);

//playe node
void	add_player_node(t_server *server, int id, char *pseudo);
void	update_player_node(t_server *server, char *pseudo, t_game_message msg);

//queue
void	add_game_message_to_queue(t_server *server, t_game_message msg);

//send
void	send_all_players(t_server *server, int id);
void	send_reconnected_message(t_server *server, t_player_info *player, char *pseudo);

#endif