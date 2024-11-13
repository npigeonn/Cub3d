/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:04:42 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/13 11:01:07 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"


void	draw_arc(t_game *game, int cx, int cy, int radius, float start_angle, float end_angle, int color)
{
	for (float angle = start_angle; angle <= end_angle; angle += 0.01)
	{
		int x = cx + radius * cos(angle);
		int y = cy + radius * sin(angle);
		pixel_put(game, x, y, color);
	}
}

void	draw_rounded_rectangle(t_game *game, t_draw_info info)
{
	t_draw_info	info2;

	if (info.radius > info.width * 0.5) info.radius = info.width * 0.5;
	if (info.radius > info.height * 0.5) info.radius = info.height * 0.5;
	info2 = init_draw_info(0, "", info.x + info.radius, info.y);
	info2.color = info.color;
	info2.width = info.width - 2 * info.radius;
	info2.height = info.height;
	draw_rectangle(game, info2);
	info2 = init_draw_info(0, "", info.x, info.y + info.radius);
	info2.height = info.height - 2 * info.radius;
	info2.width = info.width;
	info2.color = info.color;
	draw_rectangle(game, info2);
	draw_arc(game, info.x + info.radius, info.y + info.radius, info.radius, M_PI, M_PI * 1.5, info.color);
	draw_arc(game, info.x + info.width - info.radius, info.y + info.radius, info.radius, M_PI * 1.5, 0, info.color);
	draw_arc(game, info.x + info.width - info.radius, info.y + info.height - info.radius, info.radius, 0, M_PI * 0.5, info.color);
	draw_arc(game, info.x + info.radius, info.y + info.height - info.radius, info.radius, M_PI * 0.5, M_PI, info.color);
	int radius_squared = info.radius * info.radius;
	for (int i = 0; i < info.radius; i++)
	{
		for (int j = 0; j < info.radius; j++)
		{
			if (!(i * i + j * j <= radius_squared))
				continue ;
			pixel_put(game, info.x + info.radius - i, info.y + info.radius - j, info.color);
			pixel_put(game, info.x + info.width - info.radius + i, info.y + info.radius - j, info.color);
			pixel_put(game, info.x + info.width - info.radius + i, info.y + info.height - info.radius + j, info.color);
			pixel_put(game, info.x + info.radius - i, info.y + info.height - info.radius + j, info.color);
		}
	}
}

void	update_multiplayer_click(t_game *game, int moux_x, int mouse_y, int keycode)
{
	if (keycode != 1) return;
	pthread_mutex_lock(&game->game_lock);
	if (game->menu->button_selected == 3)
		game->menu->status = MAIN_MENU;
	else if (game->menu->button_selected == 2)
	{
		game->menu->status = SERVER_CREATE;
		pthread_mutex_unlock(&game->game_lock);
		pthread_join(game->discover_servers_thread, NULL);
		return ;
	}
	else if (game->menu->button_selected == 1)
		game->menu->status = JOIN_SERVER;
	else if (game->menu->button_selected == 5)
	{
		game->menu->status = OPTIONS_KEYBOARD;
		game->menu->last_status = SERVERS;
	}
	else if (game->menu->server_selected != 0)
	{
		int i = 1;
		t_server_info *current = game->servers;

		while (current)
		{
			if (i == game->menu->server_selected)
			{
				ft_strcpy(game->client->ip, current->ip);
				game->menu->status = VALID_JOIN_SERVER;
				break;
			}
			current = current->next;
			i++;
		}
	}
	game->menu->button_selected = 0;
	pthread_mutex_unlock(&game->game_lock);
}


void	update_multiplayer_menu(t_game *game, int mouse_x, int mouse_y)
{
	pthread_mutex_lock(&game->game_lock);
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
			pthread_mutex_unlock(&game->game_lock);
			return ;
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
	pthread_mutex_unlock(&game->game_lock);
	check_mouse_on_gear(game, mouse_x, mouse_y);
}

void	*discover_servers_thread(void *arg)
{
	t_game				*game = (t_game *)arg;
	int					sockfd;
	struct sockaddr_in	recv_addr;
	char				buffer[256];
	socklen_t			addr_len = sizeof(recv_addr);
	time_t				now;

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
		perror("bind failed");
		close(sockfd);
		return NULL;
	}

	while (1)
	{
		struct timeval tv = {1, 0};
		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
		now = time(NULL);

		int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&recv_addr, &addr_len);
		if (bytes_received > 0)
		{
			buffer[bytes_received] = '\0';
			if (strstr(buffer, "ServerInfo") != NULL)
			{
				t_server_info *new_server = gc_malloc(game->mem, sizeof(t_server_info));
				if (!new_server || !(new_server->name = gc_malloc(game->mem, 64)))
				{
					perror("malloc failed");
					gc_free(game->mem, new_server);
					break;
				}

				sscanf(buffer, "ServerInfo:%[^;];Players:%d/%d;Ping:%dms", new_server->name, &new_server->players, &new_server->max_players, &new_server->ping);
				new_server->ip = gc_strdup(game->mem, inet_ntoa(recv_addr.sin_addr));
				new_server->port = ntohs(recv_addr.sin_port);
				new_server->last_seen = now;
				new_server->next = NULL;

				pthread_mutex_lock(&game->mutex);
				t_server_info *current = game->servers, *last_server = NULL;
				int server_exists = 0;

				while (current)
				{
					if (strcmp(current->ip, new_server->ip) == 0 && current->port == new_server->port)
					{
						current->last_seen = now;
						current->players = new_server->players;
						current->ping = new_server->ping;
						gc_free(game->mem, new_server->name);
						gc_free(game->mem, new_server->ip);
						gc_free(game->mem, new_server);
						server_exists = 1;
						break;
					}
					last_server = current;
					current = current->next;
				}
				if (!server_exists)
				{
					if (last_server)
						last_server->next = new_server;
					else
						game->servers = new_server;
				}
				pthread_mutex_unlock(&game->mutex);
			}
		}

		pthread_mutex_lock(&game->mutex);
		t_server_info *current = game->servers, *prev = NULL;
		while (current)
		{
			if (difftime(now, current->last_seen) > 10)
			{
				if (prev)
					prev->next = current->next;
				else
					game->servers = current->next;

				t_server_info *to_delete = current;
				current = current->next;
				gc_free(game->mem, to_delete->name);
				gc_free(game->mem, to_delete->ip);
				gc_free(game->mem, to_delete);
			}
			else
			{
				prev = current;
				current = current->next;
			}
		}
		if (game->menu->status != SERVERS)
			break ;
		pthread_mutex_unlock(&game->mutex);
	}
	close(sockfd);
	return NULL;
}


static void	draw_selected_button(t_game *game)
{
	t_draw_info	info;
	const int	list_width = game->screen_width * 0.65;
	const int	list_x = (game->screen_width - list_width) * 0.1;
	const int	remaining_space = game->screen_width - (list_x + list_width);
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	btn_x = list_x + list_width + (remaining_space - btn_width) * 0.5;
	const int	btn_y_start = game->screen_height * 0.25;
	info = init_draw_info(0, "", btn_x - 4, btn_y_start - 4);

	info.width = btn_width + 8;
	info.height = btn_height + 8;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	info.radius = 10;
	if (game->menu->button_selected == 1)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + game->screen_height * 0.05;
	if (game->menu->button_selected == 2)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + game->screen_height * 0.05;
	if (game->menu->button_selected == 3)
		draw_rounded_rectangle(game, info);
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
	t_draw_info info2;
	t_draw_info info = init_draw_info(btn_height * 0.5, "Available Servers", list_x + list_width * 0.5, list_y + 20);

	info.color = MENU_BUTTON_TEXT_COLOR;
	info2 = init_draw_info(0, "", list_x, list_y);
	info2.height = list_height;
	info2.width = list_width;
	info2.color = MENU_BUTTON_COLOR;
	info2.radius = 25;
	draw_rounded_rectangle(game, info2);
	draw_text(game, info);
	int server_y_offset = list_y + 80;
	if (game->servers == NULL)
	{
		ft_strcpy(info.str, "Searching for servers...");
		info.x = list_x + list_width * 0.5;
		info.y = game->screen_height >> 1;
		draw_text(game, info);
	}
	else
	{
		int i = 1;
		pthread_mutex_lock(&game->mutex);
		t_server_info *current = game->servers;
		while (current)
		{
			t_draw_info info3 = init_draw_info(84, "", list_x + 8, server_y_offset - 2);
			info3.color = 0xda1254;
			info3.width = list_width - 16;
			info3.radius = 15;
			if (game->menu->server_selected == i)
				draw_rounded_rectangle(game, info3);
			info3 = init_draw_info(80, "", list_x + 10, server_y_offset);
			info3.color = MENU_BUTTON_SELECTED_COLOR;
			info3.width = list_width - 20;
			info3.radius = 15;
			draw_rounded_rectangle(game, info3);
			info3 = init_draw_info(btn_height * 0.4, current->name, list_x + 40, server_y_offset + 10);
			info3.color = MENU_BUTTON_TEXT_COLOR;
			draw_text_left(game, info3);
			ft_strcpy(info.str, "1/4");
			info3.y = server_y_offset + 22;
			info3.x = list_x + list_width - 80;
			info3.height = btn_height * 0.4;
			draw_text_right(game, info3);
			info3.x = list_x + 40;
			ft_strcpy(info.str, "Ping: 30ms");
			info3.y = server_y_offset + 50;
			info3.height = btn_height * 0.3;
			draw_text_left(game, info3);
			server_y_offset += 90;
			current = current->next;
			i++;
		}
		pthread_mutex_unlock(&game->mutex);
	}

	draw_selected_button(game);
	const int remaining_space = game->screen_width - (list_x + list_width);
	const int btn_x = list_x + list_width + (remaining_space - btn_width) * 0.5;
	const int btn_y_start = game->screen_height * 0.25;
	info2 = init_draw_info(0, "", btn_x, btn_y_start);
	info2.height = btn_height;
	info2.width = btn_width;
	info2.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Join Server");
	info.x = btn_x + btn_width * 0.5;
	info.y = btn_y_start + btn_height * 0.33 - 5;
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Create Server");
	info.y = btn_y_start + btn_height + spacing + btn_height * 0.33 - 5;
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Back");
	info.y = btn_y_start + 2 * (btn_height + spacing) + btn_height * 0.33 - 5;
	draw_text(game, info);

	const int	gear_size = game->screen_width * 0.035;
	const int	gear_x = game->screen_width - gear_size - 17;
	const int	gear_y = 15;
	draw_gear_icon(game, gear_x, gear_y, gear_size);
}