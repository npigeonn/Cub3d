/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:13:44 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/12/07 05:07:42 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	manage_position(t_game *game, t_game_message msg)
{
	game->player->x = msg.x;
	game->player->y = msg.y;
	game->player->health = msg.health;
	game->player->height = msg.height;
	game->player->floor = msg.floor;
	if (game->player->health < 0)
		game->player->health = 0;
}

void	replace_path(t_game *game, char *filename)
{
	const char	*old_path = "./assets/maps/";
	const char	*new_path = "./assets/maps/multi/";
	size_t		new_length;
	char		*pos;
	char		*new_filename;

	pos = strstr(filename, old_path);
	if (pos)
	{
		new_length = ft_strlen(filename) - ft_strlen(old_path)
			+ ft_strlen(new_path);
		new_filename = gc_malloc(game->mem, new_length + 1);
		strncpy(new_filename, filename, pos - filename);
		new_filename[pos - filename] = '\0';
		strcat(new_filename, new_path);
		strcat(new_filename, pos + ft_strlen(old_path));
		strcpy(filename, new_filename);
		gc_free(game->mem, new_filename);
	}
}

int	receive_data(t_game *game, size_t file_size, int server_socket, FILE *file)
{
	size_t	total_received;
	size_t	bytes_received;
	size_t	bytes_to_receive;
	char	buffer[1024];

	while (total_received < file_size)
	{
		if (file_size - total_received < sizeof(buffer))
			bytes_to_receive = file_size - total_received;
		else
			bytes_to_receive = sizeof(buffer);
		bytes_received = recv(server_socket, buffer, bytes_to_receive, 0);
		if (bytes_received <= 0)
			return (0);
		fwrite(buffer, 1, bytes_received, file);
		total_received += bytes_received;
	}
	return (1);
}

char	*receive_file_from_server(t_game *game, int server_socket)
{
	char			filename[256];
	char			buffer[1024];
	FILE			*file;
	t_game_message	msg;
	long			file_size;

	recv(server_socket, filename, sizeof(filename), 0);
	replace_path(game, filename);
	file = fopen(filename, "wb");
	if (!file)
		return (NULL);
	recv(server_socket, &file_size, sizeof(file_size), 0);
	if (!receive_data(game, file_size, server_socket, file))
	{
		fclose(file);
		remove(filename);
		return (NULL);
	}
	fclose(file);
	return (gc_strdup(game->mem, filename));
}

int	receive_map_from_server(t_game *game, int client_socket)
{
	char	*file;
	char	**tmp;

	file = receive_file_from_server(game, client_socket);
	if (!file)
		return (0);
	tmp = game->av;
	game->av = gc_malloc(game->mem, sizeof(char *) * 3);
	game->av[0] = gc_strdup(game->mem, "cub3d");
	game->av[1] = file;
	game->av[2] = NULL;
	parsing(game->av, game);
	free_split(game->mem, game->av);
	game->av = tmp;
	return (1);
}

static int	manage_connection(t_game *game)
{
	t_game_message	connect_msg;
	ssize_t			recv_size;

	recv_size = recv(game->client->sock, &connect_msg,
			sizeof(t_game_message), 0);
	if (recv_size <= 0 || connect_msg.type == MSG_FULL)
	{
		if (recv_size <= 0)
			game->menu->status = SERVER_DISCONNECTED;
		else
			game->menu->status = SERVER_FULL;
		close(game->client->sock);
		return (0);
	}
	game->client->player_id = connect_msg.player_id;
	manage_position(game, connect_msg);
	if (!receive_map_from_server(game, game->client->sock))
	{
		game->menu->status = SERVER_DISCONNECTED;
		close(game->client->sock);
		return (0);
	}
	return (1);
}

static int	manage_error(t_game *game, struct sockaddr_in serv_addr)
{
	if (inet_pton(AF_INET, game->client->ip, &serv_addr.sin_addr) <= 0)
	{
		printf("Invalid address/ Address not supported\n");
		game->menu->status = SERVER_DISCONNECTED;
		return (0);
	}
	if (connect(game->client->sock, (struct sockaddr *)&serv_addr,
			sizeof(serv_addr)) < 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		return (0);
	}
	return (1);
}

void	reset_enemies(t_game *game)
{
	t_sprite	*current;
	t_sprite	*previous;
	t_sprite	*next;

	previous = NULL;
	current = game->sprites;
	while (current)
	{
		if (current->type == SPRITE_ENEMY)
		{
			next = current->next;
			gc_free(game->mem, current);
			if (previous)
				previous->next = next;
			else
				game->sprites = next;
			current = next;
		}
		else
		{
			previous = current;
			current = current->next;
		}
	}
}

int	join_server(t_game *game)
{
	struct sockaddr_in	serv_addr;
	char				pseudo[MAX_PSEUDO_LENGTH];
	pthread_t			thread_id;

	ft_strcpy(pseudo, game->client->pseudo);
	reset_enemies(game);
	game->client->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (game->client->sock < 0)
	{
		game->menu->status = SERVER_DISCONNECTED;
		return (0);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if (!manage_error(game, serv_addr))
		return (0);
	send(game->client->sock, pseudo, ft_strlen(pseudo) + 1, 0);
	if (!manage_connection(game))
		return (0);
	pthread_mutex_init(&game->game_lock, NULL);
	pthread_create(&thread_id, NULL, receive_updates, game);
	return (1);
}
