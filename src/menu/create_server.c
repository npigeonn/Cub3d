/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_server.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:49:50 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/21 00:23:03 by ybeaucou         ###   ########.fr       */
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

void	valid_name_server(t_game *game)
{
	if (game->client->name[0] == '\0')
		game->menu->error_name = true;
	else
		game->menu->status = VALID_SERVER_CREATE;
}

void	handle_ip_input(t_game *game, int keycode)
{
	char	*field;
	int		len;

	field = game->client->ip;
	len = ft_strlen(field);
	if ((keycode >= 65429 && keycode <= 65439)
		|| (keycode >= 48 && keycode <= 57) || keycode == 46)
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
		valid_name_server(game);
}

void	update_server_name(t_game *game, int keycode, int len, char *field)
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

void	handle_text_input(t_game *game, int keycode)
{
	char	*field;
	int		len;

	field = get_field(game);
	len = ft_strlen(field);
	if (!field)
		return ;
	if (game->menu->text_field_selected == 1
		&& game->menu->status == JOIN_SERVER)
		return (handle_ip_input(game, keycode));
	if ((keycode >= 32 && keycode <= 126)
		|| (keycode >= 65429 && keycode <= 65439) && len < 15)
		update_server_name(game, keycode, len, field);
	else if (keycode == 65288 && len > 0)
		field[len - 1] = '\0';
	else if (keycode == 65293 && game->menu->text_field_selected == 1)
	{
		if (game->client->name[0] == '\0')
			game->menu->error_name = true;
		else
			game->menu->status = VALID_SERVER_CREATE;
	}
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

void	handle_create_server(t_game *game)
{
	if (game->client->name[0] == '\0')
		game->menu->error_name = true;
	if (game->client->name[0] != '\0')
		game->menu->status = VALID_SERVER_CREATE;
}

void	update_create_server_menu_text2(t_game *game, int keycode)
{
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
}

void	update_create_server_menu_text(t_game *game, int mouse_x, int mouse_y,
int keycode)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	spacing = game->screen_height * 0.06;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.33;

	if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + 30
		&& mouse_y <= y + 30 + btn_height)
	{
		if (keycode == 1)
		{
			game->menu->text_field_selected = 1;
			game->menu->error_name = false;
		}
	}
	else
		game->menu->text_field_selected = 0;
	update_create_server_menu_text2(game, keycode);
}

void	update_create_server_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_height = game->screen_height * 0.08;
	const int	btn_h_width = game->screen_width * 0.25 * 0.45;
	const int	t_btn_width = 2 * btn_h_width + game->screen_width * 0.02;
	const int	btn_x_start = (game->screen_width - t_btn_width) / 2;
	const int	btn_y_start = game->screen_height * 0.33 + 2
		* (btn_height + game->screen_height * 0.06);

	if (mouse_x >= btn_x_start && mouse_x <= btn_x_start + btn_h_width
		&& mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
		game->menu->button_selected = 1;
	else if (mouse_x >= btn_x_start + btn_h_width + game->screen_width * 0.02
		&& mouse_x <= btn_x_start + 2 * btn_h_width + game->screen_width * 0.02
		&& mouse_y >= btn_y_start && mouse_y <= btn_y_start + btn_height)
		game->menu->button_selected = 2;
	else
		game->menu->button_selected = 0;
	check_mouse_on_gear(game, mouse_x, mouse_y);
}

void	draw_text_field(t_game *game, t_draw_info info, char *text)
{
	info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, info);
	if (text)
	{
		info = init_draw_info(info.height * 0.5, text, info.x + 10,
				info.y + info.height / 2 - 10);
		info.color = MENU_BUTTON_TEXT_COLOR;
		ft_strlcpy(info.str, text, MAX_MESSAGE_LENGTH);
		draw_text_left(game, info);
	}
}

void	draw_button_with_selection(t_game *game, t_draw_info btn_info,
int is_selected, const char *label)
{
	t_draw_info	info;

	if (is_selected)
	{
		info = init_draw_info(btn_info.height + 8, "", btn_info.x - 4,
				btn_info.y - 4);
		info.color = MENU_BUTTON_SELECTED_COLOR;
		info.width = btn_info.width + 8;
		info.radius = 10;
		draw_rounded_rectangle(game, info);
	}
	btn_info.color = MENU_BUTTON_COLOR;
	draw_rectangle(game, btn_info);
	info = init_draw_info(btn_info.height * 0.5, label, btn_info.x
			+ btn_info.width / 2, btn_info.y + btn_info.height / 3 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_labeled_text_field(t_game *game, char *content, t_draw_info info2)
{
	t_draw_info	info;

	info = init_draw_info(info2.height * 0.5, info2.str, info2.x, info2.y - 10);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
	info = init_draw_info(info2.height, "", info2.x, info2.y);
	info.width = info2.width;
	draw_text_field(game, info, content);
	gc_free(game->mem, content);
}

void	draw_create_server_name(t_game *game, int btn_height, int y)
{
	t_draw_info	info;

	info = init_draw_info(btn_height * 0.9, "Create Server",
			game->screen_width >> 1, y - 140);
	info.color = 0xFFFFF;
	draw_text(game, info);
}

void	draw_create_server_error(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.33;
	t_draw_info	info;

	if (game->menu->text_field_selected == 1 || game->menu->error_name)
	{
		info = init_draw_info(btn_height + 8, "", x - 4, y + 30 - 4);
		if (game->menu->error_name)
			info.color = 0xFF0000;
		else
			info.color = MENU_BUTTON_SELECTED_COLOR;
		info.width = btn_width + 8;
		info.radius = 10;
		draw_rounded_rectangle(game, info);
	}
}

void	draw_buttons_create_server(t_game *game, int btn_width, int btn_height,
int y)
{
	const int	btn_h_width = btn_width * 0.45;
	const int	total_btn_width = 2 * btn_h_width + game->screen_width * 0.02;
	const int	btn_x_start = (game->screen_width - total_btn_width) / 2;
	const int	btn_y_start = y + 2 * (btn_height + game->screen_height * 0.06);
	t_draw_info	btn_info;

	btn_info = init_draw_info(btn_height, "", btn_x_start, btn_y_start);
	btn_info.width = btn_h_width;
	draw_button_with_selection(game, btn_info,
		game->menu->button_selected == 1, "Back");
	btn_info = init_draw_info(btn_height, "", btn_x_start + btn_h_width
			+ game->screen_width * 0.02, btn_y_start);
	btn_info.width = btn_h_width;
	draw_button_with_selection(game, btn_info,
		game->menu->button_selected == 2, "Create");
	draw_gear_icon(game, game->screen_width - game->screen_width * 0.035 - 17,
		15, game->screen_width * 0.035);
}

void	draw_create_server_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.33;
	t_draw_info	info;

	draw_create_server_name(game, btn_height, y);
	draw_create_server_error(game);
	info = init_draw_info(btn_height, "Server Name", x, y + 30);
	info.width = btn_width;
	draw_labeled_text_field(game, gc_strdup(game->mem, game->client->name),
		info);
	info = init_draw_info(btn_height, "Server IP", x,
			y + (btn_height + game->screen_height * 0.06) + 30);
	info.width = btn_width;
	draw_labeled_text_field(game, get_local_ip(game), info);
	draw_buttons_create_server(game, btn_width, btn_height, y);
}
