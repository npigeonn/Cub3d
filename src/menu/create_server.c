/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_server.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:49:50 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/18 17:50:49 by ybeaucou         ###   ########.fr       */
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
	if (game->menu->text_field_selected == 2)
		return (game->client->pseudo);
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
	len = strlen(field);
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
}

void	handle_text_input(t_game *game, int keycode)
{
	char	*field;
	int		len;

	field = get_field(game);
	len = ft_strlen(field);
	if (!field)
		return ;
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
			else
				field[len] = (char)keycode;
			field[len + 1] = '\0';
		}
	}
	else if (keycode == 65288 && len > 0)
		field[len - 1] = '\0';
}

void get_local_ip(char *ip_buffer, size_t buffer_size)
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
	if (game->client->name[0] == '\0')
		game->menu->error_name = true;
	if (game->client->pseudo[0] == '\0')
		game->menu->error_pseudo = true;
	if (game->client->pseudo[0] != '\0' && game->client->name[0] != '\0')
		game->menu->status = VALID_SERVER_CREATE;
}

void	update_create_server_menu_text(t_game *game, int mouse_x, int mouse_y, int mouse_button)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.22;

	if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + 30 && mouse_y <= y + 30 + btn_height)
	{
		if (mouse_button == 1)
		{
			game->menu->text_field_selected = 1;
			game->menu->error_name = false;
		}
	}
	else if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + btn_height + spacing + 30 && mouse_y <= y + btn_height + spacing + 30 + btn_height)
	{
		if (mouse_button == 1)
		{
			game->menu->text_field_selected = 2;
			game->menu->error_pseudo = false;
		}
	}
	else
		game->menu->text_field_selected = 0;
	if (game->menu->button_selected == 1 && mouse_button == 1)
	{
		game->client->name[0] = '\0';
		game->client->pseudo[0] = '\0';
		game->menu->status = SERVERS;
	}
	if (game->menu->button_selected == 2 && mouse_button == 1)
		handle_create_server(game);
}
void update_create_server_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int btn_width = game->screen_width * 0.25;  
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;

	const int btn_half_width = btn_width * 0.45;
	const int total_btn_width = 2 * btn_half_width + game->screen_width * 0.02;
	const int btn_x_start = (game->screen_width - total_btn_width) / 2;
	const int btn_y_start = game->screen_height * 0.22 + 3 * (btn_height + spacing);

	if (mouse_x >= btn_x_start && mouse_x <= btn_x_start + btn_half_width && 
		mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
		game->menu->button_selected = 1;
	else if (mouse_x >= btn_x_start + btn_half_width + game->screen_width * 0.02 && 
			 mouse_x <= btn_x_start + 2 * btn_half_width + game->screen_width * 0.02 && 
			 mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
		game->menu->button_selected = 2;
	else
		game->menu->button_selected = 0;
}

void	draw_text_field(t_game *game, int x, int y, int width, int height, char *text)
{
	draw_rectangle(game, x, y, width, height, MENU_BUTTON_COLOR);
	if (text)
		draw_text_left(game, text, x + 10, y + height / 2 - 10, height * 0.5, MENU_BUTTON_TEXT_COLOR);
}

void	draw_create_server_menu(t_game *game)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.22;

	draw_text(game, "Create Server", game->screen_width >> 1, y - 140, btn_height * 0.9, 0xFFFFF);
	if (game->menu->text_field_selected == 1)
		draw_rounded_rectangle(game, x - 4, y + 30 - 4, btn_width + 8, btn_height + 8, 10, MENU_BUTTON_SELECTED_COLOR);
	if (game->menu->error_name)
		draw_rounded_rectangle(game, x - 4, y + 30 - 4, btn_width + 8, btn_height + 8, 10, 0xFF0000);
	draw_text(game, "Server Name", game->screen_width >> 1, y - 10, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_text_field(game, x, y + 30, btn_width, btn_height, game->client->name);
	if (game->menu->text_field_selected == 2)
		draw_rounded_rectangle(game, x - 4, y + btn_height + spacing + 30 - 4, btn_width + 8, btn_height + 8, 10, MENU_BUTTON_SELECTED_COLOR);
	if (game->menu->error_pseudo)
		draw_rounded_rectangle(game, x - 4, y + btn_height + spacing + 30 - 4, btn_width + 8, btn_height + 8, 10, 0xFF0000);
	draw_text(game, "Pseudo", game->screen_width >> 1, y + btn_height + spacing - 10, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_text_field(game, x, y + btn_height + spacing + 30, btn_width, btn_height, game->client->pseudo);
	draw_text(game, "Server IP", game->screen_width >> 1, y + 2 * (btn_height + spacing) - 10, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
    char ip[INET_ADDRSTRLEN];
    get_local_ip(ip, sizeof(ip));
	draw_text_field(game, x, y + 2 * (btn_height + spacing) + 30, btn_width, btn_height, ip);

	const int btn_half_width = btn_width * 0.45;
	const int total_btn_width = 2 * btn_half_width + game->screen_width * 0.02;
	const int btn_x_start = (game->screen_width - total_btn_width) / 2;
	const int btn_y_start = y + 3 * (btn_height + spacing);
	if (game->menu->button_selected == 1)
		draw_rounded_rectangle(game, btn_x_start - 4, btn_y_start - 4, btn_half_width + 8, btn_height + 8, 10, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x_start, btn_y_start, btn_half_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Back", btn_x_start + (btn_half_width / 2), btn_y_start + btn_height / 3 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	if (game->menu->button_selected == 2)
		draw_rounded_rectangle(game, btn_x_start + btn_half_width + game->screen_width * 0.02 - 4, btn_y_start - 4, btn_half_width + 8, btn_height + 8, 10, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x_start + btn_half_width + game->screen_width * 0.02, btn_y_start, btn_half_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Create", btn_x_start + btn_half_width + game->screen_width * 0.02 + (btn_half_width / 2), btn_y_start + btn_height / 3 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
}
