/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:06:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/14 11:24:11 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
		gc_free(server->mem, pseudo);
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
	events = gc_malloc(server->mem, sizeof(struct epoll_event) * MAX_PLAYERS);
	if (server->epoll_fd == -1)
		gc_exit(server->mem, 1);
	event.events = EPOLLIN;
	event.data.fd = server->server_fd;
	if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD,
			server->server_fd, &event) == -1)
		gc_exit(server->mem, 1);
	while (!server->stop)
	{
		num_events = epoll_wait(server->epoll_fd, events, MAX_PLAYERS, 1000);
		if (num_events == -1)
			gc_exit(server->mem, 1);
		i = -1;
		while (++i < num_events)
			loop_server_event(server, events, &event, i);
	}
}
