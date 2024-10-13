#include "../../includes/cub3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>


t_player_info	*players_client;

void	update_player_position(GameMessage msg)
{
	t_player_info	*player = find_player_by_id(msg.player_id);
	if (player)
	{
		player->x = msg.x;
		player->y = msg.y;
	}
}

void	add_player(GameMessage msg)
{
	t_player_info	*new_player = malloc(sizeof(t_player_info));
	new_player->player_id = msg.player_id;
	new_player->x = msg.x;
	new_player->y = msg.y;
	new_player->next = NULL;
	strcpy(new_player->pseudo, msg.pseudo);
	if (!players_client)
		players_client = new_player;
	else
	{
		t_player_info	*current = players_client;
		while (current->next)
			current = current->next;
		current->next = new_player;
	}
}

void	remove_player(int player_id)
{
	t_player_info	*current = players_client;
	t_player_info	*prev = NULL;

	while (current)
	{
		if (current->player_id == player_id)
		{
			if (prev)
				prev->next = current->next;
			else
				players_client = current->next;
			free(current);
			break;
		}
		prev = current;
		current = current->next;
	}
}

void*	receive_updates(void* sock_ptr)
{
	int sock = *(int*)sock_ptr;
	GameMessage msg;

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
			printf("Player %d (%s) moved to x = %.2f, y = %.2f\n", msg.player_id, msg.pseudo, msg.x, msg.y);
			update_player_position(msg);
		}
		else if (msg.type == MSG_CONNECT)
		{
			printf("Player %d (%s) connected at x = %.2f, y = %.2f\n", msg.player_id, msg.pseudo, msg.x, msg.y);
			add_player(msg);
		}
		else if (msg.type == MSG_RECONNECT)
		{
			printf("Player %d (%s) reconnected at x = %.2f, y = %.2f\n", msg.player_id, msg.pseudo, msg.x, msg.y);
			add_player(msg);
		}
		else if (msg.type == MSG_DISCONNECT)
		{
			printf("Player %d disconnected\n", msg.player_id);
			remove_player(msg.player_id);
		}
		else
			printf("Unknown message type received: %d\n", msg.type);
	}
}

int	join_server(t_game *game)
{
	struct sockaddr_in serv_addr;
	char pseudo[MAX_PSEUDO_LENGTH];

	strncpy(pseudo, "yoyoazs", MAX_PSEUDO_LENGTH);
	if ((game->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\nSocket creation error\n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address / Address not supported\n");
		return -1;
	}
	if (connect(game->sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed\n");
		return -1;
	}
	send(game->sock, pseudo, strlen(pseudo) + 1, 0);
	GameMessage connect_msg;
	ssize_t recv_size = recv(game->sock, &connect_msg, sizeof(GameMessage), 0);
	if (recv_size <= 0) {
		printf("Error receiving connection message from server.\n");
		close(game->sock);
		return -1;
	}
	if (connect_msg.type == MSG_FULL)
	{
		printf("Server is full\n");
		close(game->sock);
		return -1;
	}
	game->player_id = connect_msg.player_id;
	game->pseudo = strdup(connect_msg.pseudo);
	printf("Connected as player %d (%s) at x = %f, y = %f\n", connect_msg.player_id, connect_msg.pseudo, connect_msg.x, connect_msg.y);
	if (connect_msg.x < 0 || connect_msg.y < 0)
	{
		connect_msg.type = MSG_MOVE;
		connect_msg.x = game->player->x;
		connect_msg.y = game->player->y;
		send(game->sock, &connect_msg, sizeof(GameMessage), 0);
	}
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, receive_updates, &game->sock);
}
