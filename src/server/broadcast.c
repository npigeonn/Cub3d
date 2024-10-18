/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 23:56:18 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 15:56:14 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	broadcast_server_info(t_server *server,
struct sockaddr_in *broadcast_addr)
{
	char	message[256];

	ft_strcpy(message, "ServerInfo:");
	ft_strlcat(message, server->name, 256);
	ft_strlcat(message, ";Players:1/4;Ping:30ms", 256);
	sendto(server->sock_bc, message, ft_strlen(message), 0,
		(struct sockaddr *)broadcast_addr, sizeof(*broadcast_addr));
}

static void	thread_broadcast_server_info(t_server *server)
{
	int					broadcast_fd;
	struct sockaddr_in	broadcast_addr;

	ft_memset(&broadcast_addr, 0, sizeof(broadcast_addr));
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_port = htons(BROADCAST_PORT);
	broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	while (1)
	{
		broadcast_server_info(server, &broadcast_addr);
		sleep(5);
	}
}

int	init_broadcast(t_server *server)
{
	pthread_t	broadcast_thread;
	int			broadcast;

	broadcast = 1;
	server->sock_bc = socket(AF_INET, SOCK_DGRAM, 0);
	if (server->sock_bc < 0)
		return (-1);
	if (setsockopt(server->sock_bc, SOL_SOCKET, SO_BROADCAST, &broadcast,
			sizeof(broadcast)) < 0)
	{
		close(server->sock_bc);
		return (-1);
	}
	pthread_create(&broadcast_thread, NULL,
		(void *)thread_broadcast_server_info, server);
	return (0);
}
