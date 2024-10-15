
#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

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
}	GameMessage;

typedef struct s_move_message
{
	int		player_id;
	float	new_x;
	float	new_y;
}	MoveMessage;

t_player_info	*find_player_by_id(t_player_info *players, int id);
void			*discover_servers_thread(void *arg);

#endif