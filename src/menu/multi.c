/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 22:26:36 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 16:01:06 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_multiplayer_click(t_game *game, int moux_x, int mouse_y, int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 3)
		game->menu->status = MAIN_MENU;
	else if (game->menu->button_selected == 2)
		game->menu->status = SERVER_CREATE;
	else if (game->menu->button_selected == 1)
		game->menu->status = JOIN_SERVER;
	else if (game->menu->server_selected != 0)
	{
		int	i = 1;
		t_server_info *current;

		current = game->servers;
		while (current)
		{
			if (i == game->menu->server_selected)
			{
				ft_strcpy(game->client->ip, current->ip);
				game->menu->status = JOIN_SERVER;
				break;
			}
			current = current->next;
			i++;
		}
	}
	game->menu->button_selected = 0;
}

void	update_multiplayer_menu(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
		
	const int	list_width = game->screen_width * 0.65 - 20;
	const int	list_height = game->screen_height * 0.8;
	const int	list_x = (game->screen_width - list_width) * 0.1 + 10;
	const int	list_y = (game->screen_height - list_height) * 0.35;
	int			server_y_offset = list_y + 80;
	
	t_server_info	*current;
	int				i = 1;

	game->menu->button_selected = 0;
	game->menu->server_selected = 0;
	current = game->servers;
	while (current)
	{
		if (mouse_x >= list_x && mouse_x <= list_x + list_width &&
			mouse_y >= server_y_offset && mouse_y <= server_y_offset + 80)
		{
			game->menu->server_selected = i;
			return;
		}
		server_y_offset += 60;
		current = current->next;
		i++;
	}
	const int	remaining_space = game->screen_width - (list_x + list_width);
	const int	btn_x = list_x + list_width + (remaining_space - btn_width) * 0.5;
	const int	btn_y_start = game->screen_height * 0.25;
	if (mouse_x >= btn_x && mouse_x <= btn_x + btn_width)
	{
		if (mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
			game->menu->button_selected = 1;
		else if (mouse_y >= btn_y_start + btn_height + spacing && mouse_y <= btn_y_start + 2 * btn_height + spacing)
			game->menu->button_selected = 2;
		else if (mouse_y >= btn_y_start + 2 * (btn_height + spacing) && mouse_y <= btn_y_start + 3 * btn_height + 2 * spacing)
			game->menu->button_selected = 3;
	}
}

void	draw_arc(t_game *game, int cx, int cy, int radius, float start_angle, float end_angle, int color)
{
	for (float angle = start_angle; angle <= end_angle; angle += 0.01)
	{
		int x = cx + radius * cos(angle);
		int y = cy + radius * sin(angle);
		pixel_put(game, x, y, color);
	}
}

void	draw_rounded_rectangle(t_game *game, int x, int y, int width, int height, int radius, int color)
{
	if (radius > width * 0.5) radius = width * 0.5;
	if (radius > height * 0.5) radius = height * 0.5;

	draw_rectangle(game, x + radius, y, width - 2 * radius, height, color);
	draw_rectangle(game, x, y + radius, width, height - 2 * radius, color);
	draw_arc(game, x + radius, y + radius, radius, M_PI, M_PI * 1.5, MENU_BUTTON_SELECTED_COLOR);
	draw_arc(game, x + width - radius, y + radius, radius, M_PI * 1.5, 0, MENU_BUTTON_SELECTED_COLOR);
	draw_arc(game, x + width - radius, y + height - radius, radius, 0, M_PI * 0.5, MENU_BUTTON_SELECTED_COLOR);
	draw_arc(game, x + radius, y + height - radius, radius, M_PI * 0.5, M_PI, MENU_BUTTON_SELECTED_COLOR);
	for (int i = 0; i < radius; i++)
	{
		for (int j = 0; j < radius; j++)
		{
			if (sqrt(i * i + j * j) <= radius)
				pixel_put(game, x + radius - i, y + radius - j, color);
			if (sqrt(i * i + j * j) <= radius)
				pixel_put(game, x + width - radius + i, y + radius - j, color);
			if (sqrt(i * i + j * j) <= radius)
				pixel_put(game, x + width - radius + i, y + height - radius + j, color);
			if (sqrt(i * i + j * j) <= radius)
				pixel_put(game, x + radius - i, y + height - radius + j, color);
		}
	}
}

void	*discover_servers_thread(void *arg)
{
	t_game			*game = (t_game *)arg;
	int				sockfd;
	struct sockaddr_in	recv_addr;
	char			buffer[256];
	socklen_t		addr_len = sizeof(recv_addr);
	t_server_info	*last_server = NULL;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("socket failed");
		return NULL;
	}
	ft_memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(BROADCAST_PORT);
	recv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0)
	{
		close(sockfd);
		return NULL;
	}
	while (game->menu->status == SERVERS)
	{
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

		time_t now = time(NULL);
		int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&recv_addr, &addr_len);
		if (bytes_received > 0)
		{
			buffer[bytes_received] = '\0';
			if (strstr(buffer, "ServerInfo") != NULL)
			{
				t_server_info *new_server = malloc(sizeof(t_server_info));
				if (new_server == NULL)
				{
					perror("malloc failed");
					break;
				}
				new_server->name = malloc(64);
				if (new_server->name == NULL)
				{
					perror("malloc failed");
					free(new_server);
					break;
				}
				sscanf(buffer, "ServerInfo:%[^;];Players:%d/%d;Ping:%dms", new_server->name, &new_server->players, &new_server->max_players, &new_server->ping);
				new_server->ip = ft_strdup(inet_ntoa(recv_addr.sin_addr));
				new_server->port = ntohs(recv_addr.sin_port);
				new_server->last_seen = now;
				new_server->next = NULL;
				if (game->servers == NULL)
					game->servers = new_server;
				else
				{
					t_server_info *current = game->servers;
					t_server_info *last_server = NULL;
					int server_exists = 0;

					while (current != NULL)
					{
						if (strcmp(current->ip, new_server->ip) == 0 && current->port == new_server->port)
						{
							current->last_seen = now;
							current->players = new_server->players;
							current->ping = new_server->ping;
							free(new_server->name);
							free(new_server->ip);
							free(new_server);
							server_exists = 1;
							break;
						}
						last_server = current;
						current = current->next;
					}
					if (!server_exists)
						last_server->next = new_server;
				}
			}
		}
		t_server_info *prev = NULL;
		t_server_info *current = game->servers;
		while (current != NULL)
		{
			if (difftime(now, current->last_seen) > 10)
			{
				if (prev == NULL)
					game->servers = current->next;
				else
					prev->next = current->next;
				t_server_info *to_delete = current;
				current = current->next;
				free(to_delete->name);
				free(to_delete->ip);
				free(to_delete);
			}
			else
			{
				prev = current;
				current = current->next;
			}
		}
	}
	close(sockfd);
	return (NULL);
}

void	draw_multiplayer_menu(t_game *game)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.1;
	const int spacing = game->screen_height * 0.05;
	const int list_width = game->screen_width * 0.65;
	const int list_height = game->screen_height * 0.8;
	const int list_x = (game->screen_width - list_width) * 0.1;
	const int list_y = (game->screen_height - list_height) * 0.35;

	draw_rectangle(game, list_x, list_y, list_width, list_height, MENU_BUTTON_COLOR);
	draw_text(game, "Available Servers", list_x + list_width * 0.5, list_y + 20, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	int server_y_offset = list_y + 80;
	if (game->servers == NULL)
		draw_text(game, "Searching for servers...", list_x + list_width * 0.5, game->screen_height >> 1, 20, MENU_BUTTON_TEXT_COLOR);
	else
	{
		int i = 1;
		t_server_info *current = game->servers;
		while (current)
		{
			if (game->menu->server_selected == i)
				draw_rounded_rectangle(game, list_x + 8, server_y_offset - 2, list_width - 16, 84, 15, 0xda1254);
			draw_rounded_rectangle(game, list_x + 10, server_y_offset, list_width - 20, 80, 15, MENU_BUTTON_SELECTED_COLOR);
			draw_text_left(game, current->name, list_x + 40, server_y_offset + 10, btn_height * 0.4, MENU_BUTTON_TEXT_COLOR);
			draw_text_right(game, "1/4", list_x + list_width - 80, server_y_offset + 22, btn_height * 0.4, MENU_BUTTON_TEXT_COLOR);
			draw_text_left(game, "Ping: 30ms", list_x + 40, server_y_offset + 50, btn_height * 0.3, MENU_BUTTON_TEXT_COLOR);
			server_y_offset += 90;
			current = current->next;
			i++;
		}
	}

	const int remaining_space = game->screen_width - (list_x + list_width);
	const int btn_x = list_x + list_width + (remaining_space - btn_width) * 0.5;
	const int btn_y_start = game->screen_height * 0.25;

	if (game->menu->button_selected == 1)
		draw_rectangle(game, btn_x - 2, btn_y_start - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x, btn_y_start, btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Join Server", btn_x + btn_width * 0.5, btn_y_start + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);

	const int create_btn_y = btn_y_start + btn_height + spacing;
	if (game->menu->button_selected == 2)
		draw_rectangle(game, btn_x - 2, create_btn_y - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x, create_btn_y, btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Create Server", btn_x + btn_width * 0.5, create_btn_y + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);

	const int back_btn_y = create_btn_y + btn_height + spacing;
	if (game->menu->button_selected == 3)
		draw_rectangle(game, btn_x - 2, back_btn_y - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x, back_btn_y, btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Back", btn_x + btn_width * 0.5, back_btn_y + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
}

