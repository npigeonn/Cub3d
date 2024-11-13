/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_server.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:49:50 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/13 10:47:08 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

void	handle_ip_input(t_game *game, int keycode)
{
	char *field;
	int len;

	field = game->client->ip;
	len = ft_strlen(field);
	if ((keycode >= 65429 && keycode <= 65439) ||
		(keycode >= 48 && keycode <= 57) || keycode == 46)
	{
		if (len < 15) 
		{
			if (keycode >= 65429 && keycode <= 65439)
				field[len] = get_keycode_pad(keycode);
			else if (keycode >= 48 && keycode <= 57)
				field[len] = (char)keycode;
			else if (keycode == 46)
				field[len] = '.';
			field[len + 1] = '\0';
		}
	}
	else if (keycode == 65288 && len > 0)
		field[len - 1] = '\0';
	else if (keycode == 65293)
	{
		if (game->client->ip[0] == '\0')
			game->menu->error_name = true;
		else
			game->menu->status = VALID_JOIN_SERVER;
	}
	pthread_mutex_unlock(&game->game_lock);
}

void	handle_text_input(t_game *game, int keycode)
{
	char	*field;
	int		len;

	pthread_mutex_lock(&game->game_lock);
	field = get_field(game);
	len = ft_strlen(field);
	if (!field)
	{
		pthread_mutex_unlock(&game->game_lock);
		return ;
	}
	if (game->menu->text_field_selected == 1 && game->menu->status == JOIN_SERVER)
		return (handle_ip_input(game, keycode));
	if ((keycode >= 32 && keycode <= 126) || (keycode >= 65429 && keycode <= 65439))
	{
		if (len < 15)
		{
			if (keycode >= 65429 && keycode <= 65439)
				field[len] = get_keycode_pad(keycode);
			else if (keycode == 32)
				field[len] = '_';
			else if (is_key_pressed(game, KEY_SHIFT))
				field[len] = ft_toupper((char)keycode);
			else
				field[len] = (char)keycode;
			field[len + 1] = '\0';
		}
	}
	else if (keycode == 65288 && len > 0)
		field[len - 1] = '\0';
	else if (keycode == 65293)
	{
		if (game->menu->text_field_selected == 1)
		{
			if (game->client->name[0] == '\0')
				game->menu->error_name = true;
			else
				game->menu->status = VALID_SERVER_CREATE;
		}
	}
	pthread_mutex_unlock(&game->game_lock);
}

void	get_local_ip(char *ip_buffer, size_t buffer_size)
{
	char hostbuffer[256];
	struct hostent *host_entry;
	int hostname;

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
	struct in_addr *address = (struct in_addr *)host_entry->h_addr_list[0];
	ft_strcpy(ip_buffer, inet_ntoa(*address));
}

void	handle_create_server(t_game *game)
{
	pthread_mutex_lock(&game->game_lock);
	if (game->client->name[0] == '\0')
		game->menu->error_name = true;
	if (game->client->name[0] != '\0')
		game->menu->status = VALID_SERVER_CREATE;
	pthread_mutex_unlock(&game->game_lock);
}

void	update_create_server_menu_text(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	pthread_mutex_lock(&game->game_lock);
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.33;

	if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + 30 && mouse_y <= y + 30 + btn_height)
	{
		if (keycode == 1)
		{
			game->menu->text_field_selected = 1;
			game->menu->error_name = false;
		}
	}
	else
		game->menu->text_field_selected = 0;
	if (game->menu->button_selected == 1 && keycode == 1)
	{
		game->client->name[0] = '\0';
		game->menu->status = SERVERS;
	}
	if (game->menu->button_selected == 2 && keycode == 1)
		handle_create_server(game);
	if (game->menu->button_selected == 5 && keycode == 1)
	{
		game->menu->status = OPTIONS_KEYBOARD;
		game->menu->last_status = SERVER_CREATE;
	}
	pthread_mutex_unlock(&game->game_lock);
}
void	update_create_server_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	pthread_mutex_lock(&game->game_lock);
	const int btn_width = game->screen_width * 0.25;  
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;

	const int btn_half_width = btn_width * 0.45;
	const int total_btn_width = 2 * btn_half_width + game->screen_width * 0.02;
	const int btn_x_start = (game->screen_width - total_btn_width) / 2;
	const int btn_y_start = game->screen_height * 0.33 + 2 * (btn_height + spacing);

	if (mouse_x >= btn_x_start && mouse_x <= btn_x_start + btn_half_width && 
		mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
		game->menu->button_selected = 1;
	else if (mouse_x >= btn_x_start + btn_half_width + game->screen_width * 0.02 && 
			 mouse_x <= btn_x_start + 2 * btn_half_width + game->screen_width * 0.02 && 
			 mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
		game->menu->button_selected = 2;
	else
		game->menu->button_selected = 0;
	pthread_mutex_unlock(&game->game_lock);
	check_mouse_on_gear(game, mouse_x, mouse_y);
}

void	draw_text_field(t_game *game, int x, int y, int width, int height, char *text)
{
	t_draw_info	info;

	info = init_draw_info(0, "", x, y);
	info.width = width;
	info.height = height;
	info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, info);
	if (text)
	{
		info = init_draw_info(height * 0.5, text, x + 10, y + height / 2 - 10);
		info.color = MENU_BUTTON_TEXT_COLOR;
		ft_strlcpy(info.str, text, MAX_MESSAGE_LENGTH);
		draw_text_left(game, info);
	}
}

void	draw_create_server_menu(t_game *game)
{
	pthread_mutex_lock(&game->game_lock);
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.33;
	t_draw_info info3;

	t_draw_info info = init_draw_info(btn_height * 0.9, "Create Server", game->screen_width >> 1, y - 140);
	info.color = 0xFFFFF;
	info.width = btn_height * 0.9;
	draw_text(game, info);
	if (game->menu->text_field_selected == 1)
	{
		info3 = init_draw_info(btn_height + 8, "", x - 4, y + 30 - 4);
		info3.color = MENU_BUTTON_SELECTED_COLOR;
		info3.width = btn_width + 8;
		info3.radius = 10;
		draw_rounded_rectangle(game, info3);
	}
	if (game->menu->error_name)
	{
		info3 = init_draw_info(btn_height + 8, "", x - 4, y + 30 - 4);
		info3.color = 0xFF0000;
		info3.width = btn_width + 8;
		info3.radius = 10;
		draw_rounded_rectangle(game, info3);
	}
	info.color = MENU_BUTTON_TEXT_COLOR;
	info.height = btn_height * 0.5;
	info.y = y - 10;
	ft_strcpy(info.str, "Server Name");
	draw_text(game, info);
	draw_text_field(game, x, y + 30, btn_width, btn_height, game->client->name);
	info.y = y + (btn_height + spacing) - 10;
	ft_strcpy(info.str, "Server IP");
	draw_text(game, info);
    char ip[INET_ADDRSTRLEN];
    get_local_ip(ip, sizeof(ip));
	draw_text_field(game, x, y + (btn_height + spacing) + 30, btn_width, btn_height, ip);

	const int btn_half_width = btn_width * 0.45;
	const int total_btn_width = 2 * btn_half_width + game->screen_width * 0.02;
	const int btn_x_start = (game->screen_width - total_btn_width) / 2;
	const int btn_y_start = y + 2 * (btn_height + spacing);
	t_draw_info info2 = init_draw_info(0, "", btn_x_start, btn_y_start);
	info2.color = MENU_BUTTON_COLOR;
	info2.width = btn_half_width;
	info2.height = btn_height;
	if (game->menu->button_selected == 1)
	{
		info3 = init_draw_info(btn_height + 8, "", btn_x_start - 4, btn_y_start - 4);
		info3.color = MENU_BUTTON_SELECTED_COLOR;
		info3.width = btn_half_width + 8;
		info3.radius = 10;
		draw_rounded_rectangle(game, info3);
	}
	draw_rectangle(game, info2);
	info = init_draw_info(btn_height * 0.5, "Back", btn_x_start + (btn_half_width / 2), btn_y_start + btn_height / 3 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	ft_strcpy(info.str, "Back");
	info.x = btn_x_start + (btn_half_width / 2);
	info.y = btn_y_start + btn_height / 3 - 5;
	draw_text(game, info);
	if (game->menu->button_selected == 2)
	{
		info3 = init_draw_info(btn_height + 8, "", btn_x_start + btn_half_width + game->screen_width * 0.02 - 4, btn_y_start - 4);
		info3.color = MENU_BUTTON_SELECTED_COLOR;
		info3.width = btn_half_width + 8;
		info3.radius = 10;
		draw_rounded_rectangle(game, info3);
	}
	info2.x += btn_half_width + game->screen_width * 0.02;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Create");
	info.x = btn_x_start + btn_half_width + game->screen_width * 0.02 + (btn_half_width / 2);
	draw_text(game, info);

	const int	gear_size = game->screen_width * 0.035;
	const int	gear_x = game->screen_width - gear_size - 17;
	const int	gear_y = 15;
	draw_gear_icon(game, gear_x, gear_y, gear_size);
	pthread_mutex_unlock(&game->game_lock);
}
