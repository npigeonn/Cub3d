/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:02:43 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/05 13:36:16 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	loop_server_event(t_server *server, struct epoll_event *events,
struct epoll_event *event, int i)
{
	char	*pseudo;
	int		new_socket;

	if (events[i].data.fd == server->server_fd)
	{
		new_socket = accept(server->server_fd,
				(struct sockaddr *)&(server->address),
				(socklen_t *)&(server->addrlen));
		pseudo = existing_player(server, new_socket);
		if (pseudo && pseudo[0] != '\0')
		{
			event->events = EPOLLIN;
			event->data.fd = new_socket;
			epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, new_socket, event);
			new_player(server, new_socket, pseudo);
		}
	}
	else
		handle_client_receive_msg(server, events[i].data.fd);
}

void	loop_server(t_server *server)
{
	struct epoll_event	event;
	struct epoll_event	*events;
	int					i;
	int					num_events;

	server->epoll_fd = 0;
	server->epoll_fd = epoll_create1(0);
	events = malloc(sizeof(struct epoll_event) * MAX_PLAYERS);
	if (server->epoll_fd == -1)
		exit(EXIT_FAILURE);
	event.events = EPOLLIN;
	event.data.fd = server->server_fd;
	if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD,
			server->server_fd, &event) == -1)
		exit(EXIT_FAILURE);
	while (1)
	{
		num_events = epoll_wait(server->epoll_fd, events, MAX_PLAYERS, -1);
		if (num_events == -1)
			exit(EXIT_FAILURE);
		i = -1;
		while (++i < num_events)
			loop_server_event(server, events, &event, i);
	}
}

void	init_server(t_server *server, int *opt)
{
	ft_memset(server->client_sockets, -1, sizeof(server->client_sockets));
	server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->server_fd == 0)
		exit(EXIT_FAILURE);
	if (setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
			opt, sizeof(*opt)))
		exit(EXIT_FAILURE);
	server->address.sin_family = AF_INET;
	server->address.sin_addr.s_addr = INADDR_ANY;
	server->address.sin_port = htons(PORT);
	if (bind(server->server_fd, (struct sockaddr *)&(server->address),
			sizeof(server->address)) < 0)
		exit(EXIT_FAILURE);
	if (listen(server->server_fd, 10) < 0)
		exit(EXIT_FAILURE);
	pthread_mutex_lock(server->game_lock);
	server->server_ready = 1;
	pthread_mutex_unlock(server->game_lock);
}

void	main_server(void	*arg)
{
	int			opt;
	pthread_t	logic_game_thread;
	t_server	*server;

	server = (t_server *)arg;
	opt = 1;
	server->addrlen = sizeof(server->address);
	init_server(server, &opt);
	pthread_create(&logic_game_thread, NULL, logic_game, arg);
	loop_server(server);
}

void	copy_sprite(t_game *game, t_server *server)
{
	t_sprite	*current;
	
	current = game->sprites;
	
	while(current)
	{
		t_sprite	*new_sprite = malloc(sizeof(t_sprite));
		if (new_sprite)
		{
			ft_memcpy(new_sprite, current, sizeof(t_sprite));
			new_sprite->next = server->sprites;
			server->sprites = new_sprite;
		}
		current = current->next;	
	}
}

void	create_server(t_game *game)
{
	pthread_t	server_thread;
	int			is_good;
	t_server	*server;

	is_good = 0;
	server = malloc(sizeof(t_server));
	server->nb_player = 0;
	server->game_queue = NULL;
	server->server_ready = false;
	server->game_lock = malloc(sizeof(pthread_mutex_t));
	server->map = game->map;
	server->x = game->player->x;
	server->y = game->player->y;
	server->floor = game->player->floor;
	server->door = game->door;
	server->sprites = NULL;
	copy_sprite(game, server);
	server->projectiles = game->projectiles;
	server->delta_time = 0;
	server->last_time = game->last_time;
	server->av = game->av;
	server->mem = game->mem;
	ft_strcpy(server->name, game->client->name);
	pthread_mutex_init(server->game_lock, NULL);
	pthread_create(&server_thread, NULL, (void *)main_server, (void *)server);
	while (!is_good)
	{
		pthread_mutex_lock(server->game_lock);
		is_good = server->server_ready;
		pthread_mutex_unlock(server->game_lock);
		usleep(200);
	}
	init_broadcast(server);
}
