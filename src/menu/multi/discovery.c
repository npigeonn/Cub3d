/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   discovery.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:42:05 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/14 11:24:57 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	handle_server_discovery_loop(t_game *game,
t_server_info **last_server, t_server_info **new_server, int *server_exists)
{
	t_server_info	*current;

	*last_server = NULL;
	*server_exists = 0;
	current = game->servers;
	while (current)
	{
		if (strcmp(current->ip, (*new_server)->ip) == 0
			&& current->port == (*new_server)->port)
		{
			current->last_seen = time(NULL);
			current->players = (*new_server)->players;
			current->ping = (*new_server)->ping;
			gc_free(game->mem, (*new_server)->name);
			gc_free(game->mem, (*new_server)->ip);
			gc_free(game->mem, (*new_server));
			*server_exists = 1;
			break ;
		}
		*last_server = current;
		current = current->next;
	}
}

static void	handle_server_discovery(t_game *game, char *buffer,
struct sockaddr_in *recv_addr)
{
	t_server_info	*new_server;
	t_server_info	*last_server;
	int				server_exists;

	new_server = gc_malloc(game->mem, sizeof(t_server_info));
	new_server->name = gc_malloc(game->mem, 64);
	sscanf(buffer, "ServerInfo:%[^;];Players:%d/%d;Ping:%dms",
		new_server->name, &new_server->players,
		&new_server->max_players, &new_server->ping);
	new_server->ip = gc_strdup(game->mem, inet_ntoa(recv_addr->sin_addr));
	new_server->port = ntohs(recv_addr->sin_port);
	new_server->last_seen = time(NULL);
	new_server->next = NULL;
	pthread_mutex_lock(&game->mutex);
	handle_server_discovery_loop(game, &last_server, &new_server,
		&server_exists);
	if (!server_exists)
	{
		if (last_server)
			last_server->next = new_server;
		else
			game->servers = new_server;
	}
	pthread_mutex_unlock(&game->mutex);
}

static int	setup_socket(struct sockaddr_in *recv_addr)
{
	int	sockfd;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("socket failed");
		return (-1);
	}
	ft_memset(recv_addr, 0, sizeof(*recv_addr));
	recv_addr->sin_family = AF_INET;
	recv_addr->sin_port = htons(BROADCAST_PORT);
	recv_addr->sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *)recv_addr, sizeof(*recv_addr)) < 0)
	{
		perror("bind failed");
		close(sockfd);
		return (-1);
	}
	return (sockfd);
}

void	discover_servers_loop(t_game *game, char *buffer,
struct sockaddr_in recv_addr)
{
	if (game->bytes_received > 0)
	{
		buffer[game->bytes_received] = '\0';
		if (strstr(buffer, "ServerInfo") != NULL)
			handle_server_discovery(game, buffer, &recv_addr);
	}
	purge_old_servers(game, time(NULL));
}

void	*discover_servers_thread(void *arg)
{
	t_game				*game;
	struct sockaddr_in	recv_addr;
	char				buffer[256];
	socklen_t			addr_len;
	struct timeval		tv;

	game = (t_game *)arg;
	addr_len = sizeof(recv_addr);
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	game->sockfd = setup_socket(&recv_addr);
	if (game->sockfd < 0)
		return (NULL);
	while (1)
	{
		setsockopt(game->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
		game->bytes_received = recvfrom(game->sockfd, buffer,
				sizeof(buffer) - 1, 0, (struct sockaddr *)&recv_addr,
				&addr_len);
		discover_servers_loop(game, buffer, recv_addr);
		if (game->menu->status != SERVERS)
			break ;
	}
	close(game->sockfd);
	return (NULL);
}
