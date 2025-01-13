/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:02:43 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/13 13:01:54 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

static void	main_server(void	*arg)
{
	int			opt;
	t_server	*server;

	server = (t_server *)arg;
	opt = 1;
	server->addrlen = sizeof(server->address);
	init_server(server, &opt);
	pthread_create(&server->logic_game_thread, NULL, logic_game, arg);
	loop_server(server);
}

void	copy_sprite(t_game *game, t_server *server)
{
	t_sprite	*current;
	t_sprite	*new_sprite;

	current = game->sprites;
	while (current)
	{
		new_sprite = gc_malloc(game->mem, sizeof(t_sprite));
		if (new_sprite)
		{
			ft_memcpy(new_sprite, current, sizeof(t_sprite));
			new_sprite->next = server->sprites;
			server->sprites = new_sprite;
		}
		current = current->next;
	}
}

static t_server	*server_init(t_game *game)
{
	t_server	*server;

	server = gc_malloc(game->mem, sizeof(t_server));
	server->nb_player = 0;
	server->stop = false;
	server->game_queue = NULL;
	server->server_ready = false;
	server->game_lock = gc_malloc(game->mem, sizeof(pthread_mutex_t));
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
	return (server);
}

void	create_server(t_game *game)
{
	int			is_good;
	t_server	*server;

	is_good = 0;
	server = server_init(game);
	ft_strcpy(server->name, game->client->name);
	pthread_mutex_init(server->game_lock, NULL);
	pthread_create(&server->main_server_thread, NULL, (void *)main_server,
		(void *)server);
	while (!is_good)
	{
		pthread_mutex_lock(server->game_lock);
		is_good = server->server_ready;
		pthread_mutex_unlock(server->game_lock);
		usleep(200);
	}
	init_broadcast(server);
	game->server = server;
}
