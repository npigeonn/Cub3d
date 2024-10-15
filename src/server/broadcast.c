/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 23:56:18 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/15 10:53:52 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	broadcast_server_info(t_game *game, struct sockaddr_in *broadcast_addr)
{
	char message[256];

	ft_strcpy(message, "ServerInfo:");
	strcat(message, game->server->name);
	strcat(message, ";Players:1/4;Ping:30ms");
	int sent_bytes = sendto(game->server->sock_bc, message, ft_strlen(message), 0, (struct sockaddr *)broadcast_addr, sizeof(*broadcast_addr));
}

void	thread_broadcast_server_info(t_game *game)
{
	int					broadcast_fd;
	struct sockaddr_in	broadcast_addr;

	memset(&broadcast_addr, 0, sizeof(broadcast_addr));
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_port = htons(BROADCAST_PORT);
	broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	while (1)
	{
		broadcast_server_info(game, &broadcast_addr);
		sleep(5);
	}
}

int	init_broadcast(t_game *game)
{
	pthread_t	broadcast_thread;	
	int			broadcast = 1;

	game->server->sock_bc = socket(AF_INET, SOCK_DGRAM, 0);
	if (game->server->sock_bc < 0)
		return (-1);
	if (setsockopt(game->server->sock_bc, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
	{
		close(game->server->sock_bc);
		return (-1);
	}
	pthread_create(&broadcast_thread, NULL, (void *)thread_broadcast_server_info, game);
	return (0);
}
