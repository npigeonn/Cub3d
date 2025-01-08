/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:30:28 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/03 18:27:23 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*get_field(t_game *game)
{
	if (game->menu->text_field_selected == 1)
	{
		if (game->menu->status == SERVER_CREATE)
			return (game->client->name);
		else
			return (game->client->ip);
	}
	return (NULL);
}

char	get_keycode_pad(int keycode)
{
	if (keycode == 65436)
		return ('1');
	if (keycode == 65433)
		return ('2');
	if (keycode == 65435)
		return ('3');
	if (keycode == 65430)
		return ('4');
	if (keycode == 65437)
		return ('5');
	if (keycode == 65432)
		return ('6');
	if (keycode == 65429)
		return ('7');
	if (keycode == 65431)
		return ('8');
	if (keycode == 65434)
		return ('9');
	if (keycode == 65438)
		return ('0');
	if (keycode == 65439)
		return ('.');
	return ('0');
}

void	valid_name_server(t_game *game)
{
	if (game->client->name[0] == '\0')
		game->menu->error_name = true;
	else
		game->menu->status = VALID_SERVER_CREATE;
}

char	*get_local_ip(t_game *game)
{
	char			hostbuffer[256];
	struct hostent	*host_entry;
	int				hostname;
	struct in_addr	*address;
	char			ip_buffer[INET_ADDRSTRLEN];

	hostname = gethostname(hostbuffer, sizeof(hostbuffer));
	if (hostname == -1)
	{
		perror("gethostname");
		exit(1);
	}
	host_entry = gethostbyname(hostbuffer);
	if (host_entry == NULL)
	{
		perror("gethostbyname");
		exit(1);
	}
	address = (struct in_addr *)host_entry->h_addr_list[0];
	ft_strcpy(ip_buffer, inet_ntoa(*address));
	return (gc_strdup(game->mem, ip_buffer));
}
