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

void	add_connection_msg(t_game *game, char *pseudo)
{
	t_message	*new_msg = malloc(sizeof(t_message));
	ft_strlcpy(new_msg->message, pseudo, MAX_PSEUDO_LENGTH);
	ft_strlcat(new_msg->message + ft_strlen(pseudo), " join the game.", MAX_MESSAGE_LENGTH);
	new_msg->pseudo[0] = '\0';
	new_msg->next = game->chatbox->messages;
	new_msg->color = 0x32CD32;
	gettimeofday(&new_msg->time, NULL);
	game->chatbox->messages = new_msg;
}

void	add_player(t_game *game, GameMessage msg)
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
	ft_strlcpy(new_player->pseudo, msg.pseudo, MAX_PSEUDO_LENGTH);
	if (msg.type != MSG_GET_PLAYER)
		add_connection_msg(game, msg.pseudo);
	if (!game->server->players)
		game->server->players = new_player;
	else
	{
		t_player_info	*current = game->server->players;
		while (current->next)
			current = current->next;
		current->next = new_player;
	}
}

void	remove_player(t_game *game, int player_id)
{
	t_player_info	*current = game->server->players;
	t_player_info	*prev = NULL;

	while (current)
	{
		if (current->player_id == player_id)
		{
			t_message	*new_msg = malloc(sizeof(t_message));
			ft_strlcpy(new_msg->message, current->pseudo, MAX_PSEUDO_LENGTH);
			ft_strlcat(new_msg->message + ft_strlen(current->pseudo), " left the game.", MAX_MESSAGE_LENGTH);
			new_msg->pseudo[0] = '\0';
			new_msg->next = game->chatbox->messages;
			new_msg->color = 0xFF4500;
			gettimeofday(&new_msg->time, NULL);
			game->chatbox->messages = new_msg;
			if (prev)
				prev->next = current->next;
			else
				game->server->players = current->next;
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

void	add_msg_chat(t_game *game, GameMessage msg)
{
	t_message	*new_msg = malloc(sizeof(t_message));
	ft_strlcpy(new_msg->message, msg.message, MAX_MESSAGE_LENGTH);
	ft_strlcpy(new_msg->pseudo, msg.pseudo, 20);
	new_msg->next = game->chatbox->messages;
	new_msg->color = 0x000000;
	gettimeofday(&new_msg->time, NULL);
	game->chatbox->messages = new_msg;
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
			game->menu->status = SERVER_DISCONNECTED;
			close(sock);
			break;
		}
 		if (msg.type == MSG_FULL)
		{
			game->menu->status = SERVER_DISCONNECTED;
			close(sock);
			break;
		}
		else if (msg.type == MSG_MOVE)
			update_player_position(server, msg);
		else if (msg.type == MSG_RECONNECT || msg.type == MSG_CONNECT || msg.type == MSG_GET_PLAYER)
			add_player(game, msg);
		else if (msg.type == MSG_DISCONNECT)
			remove_player(game, msg.player_id);
		else if (msg.type == MSG_DOOR)
			update_door(game, msg);
		else if (msg.type == MSG_CHAT)
			add_msg_chat(game, msg);
		else
			printf("Unknown message type received: %d\n", msg.type);
	}
}

int	join_server(t_game *game)
{
	struct sockaddr_in	serv_addr;
	char				pseudo[MAX_PSEUDO_LENGTH];

	ft_strcpy(pseudo, game->server->pseudo);
	if ((game->server->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		return (0);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, game->server->ip, &serv_addr.sin_addr) <= 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		return (0);
	}
	if (connect(game->server->sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		return (0);
	}
	send(game->server->sock, pseudo, ft_strlen(pseudo) + 1, 0);
	GameMessage connect_msg;
	ssize_t recv_size = recv(game->server->sock, &connect_msg, sizeof(GameMessage), 0);
	if (recv_size <= 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		close(game->server->sock);
		return (0);
	}
	if (connect_msg.type == MSG_FULL)
	{
		game->menu->status = SERVER_FULL;
		close(game->server->sock);
		return (0);
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
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, receive_updates, game);
	return (1);
}
