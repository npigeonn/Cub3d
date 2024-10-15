/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:55:57 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/15 12:15:55 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	handle_join_server(t_game *game)
{
	if (game->server->ip[0] == '\0')
		game->menu->error_name = true;
	if (game->server->pseudo[0] == '\0')
		game->menu->error_pseudo = true;
	if (game->server->pseudo[0] == '\0' || game->server->ip[0] == '\0')
		return ;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	if (sockfd < 0)
		return;
	ft_memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, game->server->ip, &server_addr.sin_addr) <= 0)
	{
		game->menu->error_name = true;
		close(sockfd);
		return;
	}
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		game->menu->error_name = true;
	else
		game->menu->status = VALID_JOIN_SERVER;
	close(sockfd);
}

void	update_join_server_menu_button(t_game *game, int mouse_x, int mouse_y)
{
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
}

void	update_join_server_menu_text(t_game *game, int mouse_x, int mouse_y, int mouse_button)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.33;

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
		game->server->ip[0] = '\0';
		game->server->pseudo[0] = '\0';
		game->menu->status = SERVERS;
	}
	if (game->menu->button_selected == 2 && mouse_button == 1)
		handle_join_server(game);
}

void	draw_join_server_menu(t_game *game)
{
	const int btn_width = game->screen_width * 0.25;
	const int btn_height = game->screen_height * 0.08;
	const int spacing = game->screen_height * 0.06;
	const int x = (game->screen_width - btn_width) * 0.5;
	const int y = game->screen_height * 0.33;

	draw_text(game, "Join Server", game->screen_width >> 1, y - 140, btn_height * 0.9, 0xFFFFF);
		
	if (game->menu->text_field_selected == 1)
		draw_rounded_rectangle(game, x - 4, y + 30 - 4, btn_width + 8, btn_height + 8, 10, MENU_BUTTON_SELECTED_COLOR);
	if (game->menu->error_name)
		draw_rounded_rectangle(game, x - 4, y + 30 - 4, btn_width + 8, btn_height + 8, 10, 0xFF0000);
	draw_text(game, "Server IP", game->screen_width >> 1, y - 10, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_text_field(game, x, y + 30, btn_width, btn_height, game->server->ip);

	if (game->menu->text_field_selected == 2)
		draw_rounded_rectangle(game, x - 4, y + btn_height + spacing + 30 - 4, btn_width + 8, btn_height + 8, 10, MENU_BUTTON_SELECTED_COLOR);
	if (game->menu->error_pseudo)
		draw_rounded_rectangle(game, x - 4, y + btn_height + spacing + 30 - 4, btn_width + 8, btn_height + 8, 10, 0xFF0000);
	draw_text(game, "Pseudo", game->screen_width >> 1, y + btn_height + spacing - 10, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
	draw_text_field(game, x, y + btn_height + spacing + 30, btn_width, btn_height, game->server->pseudo);

	const int btn_half_width = btn_width * 0.45;
	const int total_btn_width = 2 * btn_half_width + game->screen_width * 0.02;
	const int btn_x_start = (game->screen_width - total_btn_width) / 2;
	const int btn_y_start = y + 2 * (btn_height + spacing);
		
	if (game->menu->button_selected == 1)
		draw_rounded_rectangle(game, btn_x_start - 4, btn_y_start - 4, btn_half_width + 8, btn_height + 8, 10, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x_start, btn_y_start, btn_half_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Back", btn_x_start + (btn_half_width / 2), btn_y_start + btn_height / 3 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
		
	if (game->menu->button_selected == 2)
		draw_rounded_rectangle(game, btn_x_start + btn_half_width + game->screen_width * 0.02 - 4, btn_y_start - 4, btn_half_width + 8, btn_height + 8, 10, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, btn_x_start + btn_half_width + game->screen_width * 0.02, btn_y_start, btn_half_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Join", btn_x_start + btn_half_width + game->screen_width * 0.02 + (btn_half_width / 2), btn_y_start + btn_height / 3 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
}
