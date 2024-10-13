/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 23:56:18 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/13 23:57:35 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	broadcast_server_info(int sockfd, struct sockaddr_in *broadcast_addr)
{
	char message[256];

	snprintf(message, sizeof(message), "ServerInfo:MyGameServer;Players:1/4;Ping:30ms");
	int sent_bytes = sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)broadcast_addr, sizeof(*broadcast_addr));
	if (sent_bytes < 0)
		perror("Erreur lors de l'envoi du message broadcast");
}

void	thread_broadcast_server_info(int *sockfd)
{
	int					broadcast_fd;
	struct sockaddr_in	broadcast_addr;

	memset(&broadcast_addr, 0, sizeof(broadcast_addr));
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_port = htons(BROADCAST_PORT);
	broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	while (1)
	{
		broadcast_server_info(sockfd, &broadcast_addr);
		sleep(5);
	}
}

int	init_broadcast(void)
{
	pthread_t	broadcast_thread;	
	int			sockfd;
	int			broadcast = 1;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("Erreur lors de la création du socket");
		return (-1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
	{
		perror("Erreur lors de l'activation du mode broadcast");
		close(sockfd);
		return (-1);
	}
	pthread_create(&broadcast_thread, NULL, (void *)thread_broadcast_server_info, (void *)sockfd);
	return (0);
}
