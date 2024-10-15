#include "../../includes/cub3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>


void	update_player_position(t_server *server, GameMessage msg)
{
	t_player_info	*player = find_player_by_id(server->players, msg.player_id);
	if (player)
	{
		player->x = msg.x;
		player->y = msg.y;
		player->dir_x = msg.dir_x;
		player->dir_y = msg.dir_y;
		player->health = msg.health;
		player->height = msg.height;
		player->floor = msg.floor;
	}
}

void	add_player(t_server *server, GameMessage msg)
{
	t_player_info	*new_player = malloc(sizeof(t_player_info));
	new_player->player_id = msg.player_id;
	new_player->x = msg.x;
	new_player->y = msg.y;
	new_player->dir_x = msg.dir_x;
	new_player->dir_y = msg.dir_y;
	new_player->health = msg.health;
	new_player->height = msg.height;
	new_player->floor = msg.floor;
	new_player->next = NULL;
	strcpy(new_player->pseudo, msg.pseudo);
	if (!server->players)
		server->players = new_player;
	else
	{
		t_player_info	*current = server->players;
		while (current->next)
			current = current->next;
		current->next = new_player;
	}
}

void	remove_player(t_server *server, int player_id)
{
	t_player_info	*current = server->players;
	t_player_info	*prev = NULL;

	while (current)
	{
		if (current->player_id == player_id)
		{
			if (prev)
				prev->next = current->next;
			else
				server->players = current->next;
			free(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

void	update_door(t_game *game, GameMessage msg)
{
	t_door	*current;

	current = game->door;
	while(current)
	{
		if (current->x == msg.x && current->y == msg.y && current->floor == msg.floor)
		{
			current->open = msg.open;
			break ;
		}
		current = current->next;
	}
}

void	*receive_updates(void *args)
{
	t_game		*game = (t_game *)args;
	t_server	*server = game->server;
	int			sock = server->sock;
	GameMessage	msg;

	while (1)
	{
		ssize_t valread = recv(sock, &msg, sizeof(GameMessage), 0);
		if (valread <= 0)
		{
			printf("Disconnected from server\n");
			close(sock);
			exit(0);
		}
 		if (msg.type == MSG_FULL)
			printf("Server is full\n");
		else if (msg.type == MSG_MOVE)
		{
			printf("Player %d (%s) moved to x = %.2f, y = %.2f, floor = %d\n", msg.player_id, msg.pseudo, msg.x, msg.y, msg.floor);
			update_player_position(server, msg);
		}
		else if (msg.type == MSG_CONNECT)
		{
			printf("Player %d (%s) connected at x = %.2f, y = %.2f\n", msg.player_id, msg.pseudo, msg.x, msg.y);
			add_player(server, msg);
		}
		else if (msg.type == MSG_RECONNECT)
		{
			printf("Player %d (%s) reconnected at x = %.2f, y = %.2f\n", msg.player_id, msg.pseudo, msg.x, msg.y);
			add_player(server, msg);
		}
		else if (msg.type == MSG_DISCONNECT)
		{
			printf("Player %d disconnected\n", msg.player_id);
			remove_player(server, msg.player_id);
		}
		else if (msg.type == MSG_DOOR)
			update_door(game, msg);
		else
			printf("Unknown message type received: %d\n", msg.type);
	}
}

int	join_server(t_game *game)
{
	struct sockaddr_in serv_addr;
	char pseudo[MAX_PSEUDO_LENGTH];

	ft_strcpy(pseudo, game->server->pseudo);
	if ((game->server->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\nSocket creation error\n");
		return (-1);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, game->server->ip, &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address / Address not supported\n");
		return (-1);
	}
	if (connect(game->server->sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed\n");
		return (-1);
	}
	send(game->server->sock, pseudo, ft_strlen(pseudo) + 1, 0);
	GameMessage connect_msg;
	ssize_t recv_size = recv(game->server->sock, &connect_msg, sizeof(GameMessage), 0);
	if (recv_size <= 0)
	{
		printf("Error receiving connection message from server.\n");
		close(game->server->sock);
		return (-1);
	}
	if (connect_msg.type == MSG_FULL)
	{
		printf("Server is full\n");
		close(game->server->sock);
		return (-1);
	}
	game->server->player_id = connect_msg.player_id;
	if (connect_msg.x < 0 || connect_msg.y < 0)
	{
		connect_msg.type = MSG_MOVE;
		connect_msg.x = game->player->x;
		connect_msg.y = game->player->y;
		connect_msg.health = game->player->health;
		connect_msg.height = game->player->height;
		connect_msg.floor = game->player->floor;
		send(game->server->sock, &connect_msg, sizeof(GameMessage), 0);
	}
	else
	{
		game->player->x = connect_msg.x;
		game->player->y = connect_msg.y;
		game->player->health = connect_msg.health;
		game->player->height = connect_msg.height;
		game->player->floor = connect_msg.floor;	
	}
	printf("Connected as player %d (%s) at x = %f, y = %f, floor = %d\n", connect_msg.player_id, connect_msg.pseudo, connect_msg.x, connect_msg.y, connect_msg.floor);
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, receive_updates, game);
}
