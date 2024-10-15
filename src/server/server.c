#include <pthread.h>
#include <sys/epoll.h>
#include "../../includes/cub3d.h"

pthread_mutex_t game_lock;
int	client_sockets[MAX_PLAYERS];

typedef struct s_game_message_queue
{
	GameMessage		message;
	struct s_game_message_queue	*next;
} GameMessageQueue;

GameMessageQueue	*gameQueue;

t_player_info	*players;

pthread_cond_t server_ready_cond;
int server_ready = 0;

GameMessageQueue	*create_game_message_queue(GameMessage msg)
{
	GameMessageQueue	*new_node = malloc(sizeof(GameMessageQueue));

	new_node->message = msg;
	new_node->next = NULL;
	return (new_node);
}

void	add_game_message_to_queue(GameMessage msg)
{
	GameMessageQueue	*current;

	if (!gameQueue)
		gameQueue = create_game_message_queue(msg);
	else
	{
		current = gameQueue;
		while (current->next)
			current = current->next;
		current->next = create_game_message_queue(msg);
	}
}

t_player_info	*find_player_by_pseudo(char *pseudo)
{
	t_player_info	*current = players;

	while (current)
	{
		if (strcmp(current->pseudo, pseudo) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_player_info	*find_player_by_id(t_player_info *this_players, int id)
{
	t_player_info	*current = this_players;

	while (current)
	{
		if (current->player_id == id)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	add_player_node(int id, char *pseudo)
{
	t_player_info	*new_node = malloc(sizeof(t_player_info));
	t_player_info	*current;

	new_node->player_id = id;
	strncpy(new_node->pseudo, pseudo, MAX_PSEUDO_LENGTH);
	new_node->x = -1;
	new_node->y = -1;
	new_node->dir_x = 0;
	new_node->dir_y = 1;
	new_node->floor = 0;
	new_node->health = 100;
	new_node->height = 0;
	new_node->next = NULL;
	if (!players)
		players = new_node;
	else
	{
		current = players;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	update_player_node(char *pseudo, GameMessage msg)
{
	t_player_info	*player;

	player = find_player_by_pseudo(pseudo);
	if (!player)
		return ;
	player->x = msg.x;
	player->y = msg.y;
	player->dir_x = msg.dir_x;
	player->dir_y = msg.dir_y;
	player->floor = msg.floor;
	player->health = msg.health;
	player->height = msg.height;	
}

void	notify_players_of_connection(int player_id, char *pseudo)
{
	GameMessage connect_msg = { .type = MSG_CONNECT, .player_id = player_id};
	int				i;
	t_player_info	*player;
	
	player = find_player_by_pseudo(pseudo);
	if (!player)
		return ;
	connect_msg.x = player->x;
	connect_msg.y = player->y;
	connect_msg.floor = player->floor;
	connect_msg.height = player->height;
	connect_msg.dir_x = player->dir_x;
	connect_msg.dir_y = player->dir_y;
	connect_msg.health = player->health;
	strcpy(connect_msg.pseudo, pseudo);
	i = -1;
	while (++i < MAX_PLAYERS)
		if (client_sockets[i] > 0 && i != player_id)
			send(client_sockets[i], &connect_msg, sizeof(GameMessage), 0);
}

void	notify_players_of_reconnection(int player_id, char *pseudo)
{
	GameMessage connect_msg = { .type = MSG_RECONNECT, .player_id = player_id};
	int				i;
	t_player_info	*player;
	
	player = find_player_by_pseudo(pseudo);
	connect_msg.x = player->x;
	connect_msg.y = player->y;
	connect_msg.floor = player->floor;
	connect_msg.height = player->height;
	connect_msg.dir_x = player->dir_x;
	connect_msg.dir_y = player->dir_y;
	connect_msg.health = player->health;
	strcpy(connect_msg.pseudo, pseudo);
	i = -1;
	while (++i < MAX_PLAYERS)
		if (client_sockets[i] > 0 && i != player_id)
			send(client_sockets[i], &connect_msg, sizeof(GameMessage), 0);
}

void	notify_players_of_disconnection(int id)
{
	t_player_info	*player;
	GameMessage		disconnect_msg = { .type = MSG_DISCONNECT };
	int				i;

	player = find_player_by_id(players, id);
	if (player) {
		disconnect_msg.player_id = player->player_id;
		strncpy(disconnect_msg.pseudo, player->pseudo, MAX_PSEUDO_LENGTH);
		player->player_id = -1;
		i = -1;
		while (++i < MAX_PLAYERS)
			if (client_sockets[i] > 0 && i != player->player_id)
				send(client_sockets[i], &disconnect_msg, sizeof(GameMessage), 0);
	}
}

void	notify_players_of_move(GameMessage msg)
{
	GameMessage		move_msg;
	int				i;

	move_msg.type = MSG_MOVE;
	move_msg.player_id = msg.player_id;
	move_msg.x = msg.x;
	move_msg.y = msg.y;
	move_msg.floor = msg.floor;
	move_msg.height = msg.height;
	move_msg.dir_x = msg.dir_x;
	move_msg.dir_y = msg.dir_y;
	move_msg.health = msg.health;
	update_player_node(msg.pseudo, msg);
	strcpy(move_msg.pseudo, msg.pseudo);
	i = -1;
	while (++i < MAX_PLAYERS)
		if (client_sockets[i] > 0 && i != msg.player_id)
			send(client_sockets[i], &move_msg, sizeof(GameMessage), 0);
}

void	notify_players_of_door(GameMessage msg)
{
	int	i;

	i = -1;
	while (++i < MAX_PLAYERS)
		if (client_sockets[i] > 0 && i != msg.player_id)
			send(client_sockets[i], &msg, sizeof(GameMessage), 0);	
}

void	handle_client_msg(GameMessage msg)
{
	int				i;
	t_player_info	*player;

	pthread_mutex_lock(&game_lock);
	if (msg.type == MSG_MOVE)
	{
		player = find_player_by_pseudo(msg.pseudo);
		if (player)
		{
			player->x = msg.x;
			player->y = msg.y;
			player->dir_x = msg.dir_x;
			player->dir_y = msg.dir_y;
			player->floor = msg.floor;
			player->height = msg.height;
			add_game_message_to_queue(msg);
		}
	}
	if (msg.type == MSG_DOOR)
		add_game_message_to_queue(msg);
	pthread_mutex_unlock(&game_lock);
}

void	handle_client_message(int client_socket)
{
	GameMessage	msg;
	ssize_t		valread;
	int			i;

	valread = recv(client_socket, &msg, sizeof(GameMessage), 0);
	if (valread <= 0)
	{
		i = -1;
		while (++i < MAX_PLAYERS)
		{
			if (client_sockets[i] == client_socket)
			{
				msg.type = MSG_DISCONNECT;
				msg.player_id = i;
				add_game_message_to_queue(msg);
				close(client_socket);
				client_sockets[i] = -1;
				break ;
			}
		}
		return ;
	}
	handle_client_msg(msg);
}

void	new_player(int new_socket, char *pseudo)
{
	int	i;

	pthread_mutex_lock(&game_lock);
	t_player_info *existing_player = find_player_by_pseudo(pseudo);
	if (existing_player && existing_player->player_id >= 0)
	{
		close(new_socket);
		pthread_mutex_unlock(&game_lock);
		return ;
	}
	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if (client_sockets[i] == -1)
		{
			client_sockets[i] = new_socket;
			add_player_node(i, pseudo);
			GameMessage connect_msg = {.type = MSG_CONNECT, .player_id = i, .x = -1, .y = -1};
			strcpy(connect_msg.pseudo, pseudo);
			add_game_message_to_queue(connect_msg);
			break;
		}
	}
	if (i >= MAX_PLAYERS)
    {
		GameMessage full_msg = {.type = MSG_FULL};
		send(new_socket, &full_msg, sizeof(GameMessage), 0);
        close(new_socket);
    }
	pthread_mutex_unlock(&game_lock);
}

char *existing_player(int *nb_player, int server_fd, struct sockaddr_in address, int addrlen, int *new_socket, int epoll_fd)
{
	char pseudo[MAX_PSEUDO_LENGTH];
	t_player_info *player;
	struct epoll_event event;
	int i;

	*new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if (*new_socket < 0)
	{
		perror("accept");
		return strdup("");
	}
	recv(*new_socket, pseudo, sizeof(pseudo), 0);
	pthread_mutex_lock(&game_lock);
	player = find_player_by_pseudo(pseudo);
	if (player)
	{
		if (player->player_id >= 0 && client_sockets[player->player_id] >= 0) {
			close(*new_socket);
			pthread_mutex_unlock(&game_lock);
			return strdup("");
		}
		else
		{
			if (player->player_id < 0)
			{
				for (i = 0; i < MAX_PLAYERS; i++) 
				{
					if (client_sockets[i] == -1)
					{
						player->player_id = i;
						break;
					}
				}
			}
			client_sockets[player->player_id] = *new_socket;
			(*nb_player)++;
			event.events = EPOLLIN;
			event.data.fd = *new_socket;
			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, *new_socket, &event) < 0)
			{
				perror("epoll_ctl: add");
				close(*new_socket);
				pthread_mutex_unlock(&game_lock);
				return (NULL);
			}
			GameMessage connect_msg;
			connect_msg.type = MSG_RECONNECT;
			connect_msg.player_id = player->player_id;
			connect_msg.x = player->x;
			connect_msg.y = player->y;
			connect_msg.dir_x = player->dir_x;
			connect_msg.dir_y = player->dir_y;
			connect_msg.floor = player->floor;
			connect_msg.health = player->health;
			connect_msg.height = player->height;
			strcpy(connect_msg.pseudo, pseudo);
			add_game_message_to_queue(connect_msg);
			pthread_mutex_unlock(&game_lock);
			return (NULL);
		}
	}
	pthread_mutex_unlock(&game_lock);
	return strdup(pseudo);
}

void	loop_server(int *nb_player, int server_fd, struct sockaddr_in address, int addrlen)
{
	int					epoll_fd, new_socket;
	struct epoll_event	event, events[MAX_PLAYERS];
	char				*pseudo;
	int					i;

	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
	event.events = EPOLLIN;
	event.data.fd = server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1)
	{
		perror("epoll_ctl");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		int num_events = epoll_wait(epoll_fd, events, MAX_PLAYERS, -1);
		if (num_events == -1)
		{
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}
		for (i = 0; i < num_events; i++)
		{
			if (events[i].data.fd == server_fd)
			{
				pseudo = existing_player(nb_player, server_fd, address, addrlen, &new_socket, epoll_fd);
				if (pseudo && pseudo[0] == '\0')
					continue;
				if (pseudo)
				{
					(nb_player)++;
					event.events = EPOLLIN;
					event.data.fd = new_socket;
					epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event);
					new_player(new_socket, pseudo);
				}
			}
			else
				handle_client_message(events[i].data.fd);
		}
	}
}

void	init_server(int *server_fd, struct sockaddr_in *address, int *opt)
{
	memset(client_sockets, -1, sizeof(client_sockets));
	if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons(PORT);
	if (bind(*server_fd, (struct sockaddr*)address, sizeof(*address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(*server_fd, 10) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	pthread_mutex_lock(&game_lock);
	server_ready = 1;
	pthread_cond_broadcast(&server_ready_cond);
	pthread_mutex_unlock(&game_lock);
}

void	send_all_players(int id)
{
	t_player_info	*current;
	GameMessage		connect_msg;

	current = players;
	connect_msg.type = MSG_CONNECT;
	while(current)
	{
		if (current->player_id >= 0 && current->player_id != id)
		{
			connect_msg.player_id = current->player_id;
			connect_msg.x = current->x;
			connect_msg.y = current->y;
			connect_msg.dir_x = current->dir_x;
			connect_msg.dir_y = current->dir_y;
			connect_msg.floor = current->floor;
			connect_msg.height = current->height;
			strncpy(connect_msg.pseudo, current->pseudo, MAX_PSEUDO_LENGTH);
			send(client_sockets[id], &connect_msg, sizeof(GameMessage), 0);
		}
		current = current->next;
	}
}

void	*logic_game(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&game_lock);
		if (gameQueue)
		{
			GameMessageQueue *current = gameQueue;
			gameQueue = gameQueue->next;
			GameMessage msg = current->message;
			if (msg.type == MSG_CONNECT)
			{
				send(client_sockets[msg.player_id], &msg, sizeof(GameMessage), 0);
				send_all_players(msg.player_id);
				notify_players_of_connection(msg.player_id, msg.pseudo);
			}
			else if (msg.type == MSG_RECONNECT)
			{
				send(client_sockets[msg.player_id], &msg, sizeof(GameMessage), 0);
				send_all_players(msg.player_id);
				notify_players_of_reconnection(msg.player_id, msg.pseudo);
			}
			else if (msg.type == MSG_DISCONNECT)
				notify_players_of_disconnection(msg.player_id);
			else if (msg.type == MSG_MOVE)
				notify_players_of_move(msg);
			else if (msg.type == MSG_DOOR)
				notify_players_of_door(msg);
			free(current); 
		}
		pthread_mutex_unlock(&game_lock);
		usleep(1000);
	}
}

void	server(void)
{
	int					nb_player;
	int					server_fd;
	int					opt;
	struct sockaddr_in	address;
	int					addrlen;
	pthread_t			logic_game_thread;

	opt = 1;
	nb_player = 0;
	addrlen = sizeof(address);
	pthread_mutex_init(&game_lock, NULL);
	init_server(&server_fd, &address, &opt);
	pthread_create(&logic_game_thread, NULL, logic_game, NULL);
	loop_server(&nb_player, server_fd, address, addrlen);
}

void create_server(t_game *game)
{
	pthread_t server_thread;

	game->server->nb_player = 0;
	pthread_create(&server_thread, NULL, (void *)server, NULL);
	pthread_mutex_lock(&game_lock);
	while (!server_ready)
		pthread_cond_wait(&server_ready_cond, &game_lock);
	pthread_mutex_unlock(&game_lock);
	init_broadcast(game);
}
