/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:35:58 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/03 16:39:00 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*receive_file_from_server(t_game *game, int server_socket)
{
	char			filename[256];
	char			buffer[1024];
	FILE			*file;
	t_game_message	msg;
	long			file_size;

	recv(server_socket, filename, sizeof(filename), 0);
	replace_path(game, filename);
	create_dir_if_not_exists();
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

	total_received = 0;
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

void	create_dir_if_not_exists(void)
{
	struct stat	st;

	st = (struct stat){0};
	if (stat("./assets/maps/multi", &st) == -1)
		mkdir("./assets/maps/multi", 0700);
}
