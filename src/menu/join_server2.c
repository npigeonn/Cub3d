/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_server2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:15:23 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 16:13:42 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	handle_join_server(t_game *game)
{
	const int			sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in	s_addr;

	if (game->client->ip[0] == '\0')
		game->menu->error_name = true;
	if (game->client->pseudo[0] == '\0')
		game->menu->error_pseudo = true;
	if (game->client->pseudo[0] == '\0' || game->client->ip[0] == '\0'
		|| sockfd < 0)
		return ;
	ft_memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, game->client->ip, &s_addr.sin_addr) <= 0)
	{
		game->menu->error_name = true;
		close(sockfd);
		return ;
	}
	if (connect(sockfd, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1)
		game->menu->error_name = true;
	else
		game->menu->status = VALID_JOIN_SERVER;
	close(sockfd);
	game->menu->server_selected = 0;
}

static void	check_buttons(t_game *game, int mouse_x, int mouse_y,
int btn_x_start)
{
	const int	btn_width = game->screen_width * 0.25 * 0.45;
	const int	spacing = game->screen_width * 0.02;
	const int	btn_y_start = game->screen_height
		* 0.40 + (game->screen_height * 0.08 + spacing);

	if (mouse_x >= btn_x_start && mouse_x <= btn_x_start + btn_width
		&& mouse_y >= btn_y_start
		&& mouse_y <= btn_y_start + game->screen_height * 0.08)
		game->menu->button_selected = 1;
	else if (mouse_x >= btn_x_start + btn_width + spacing
		&& mouse_x <= btn_x_start + 2 * btn_width + spacing
		&& mouse_y >= btn_y_start
		&& mouse_y <= btn_y_start + game->screen_height * 0.08)
		game->menu->button_selected = 2;
	else
		game->menu->button_selected = 0;
}

void	update_join_server_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_half_width = game->screen_width * 0.25 * 0.45;
	const int	total_btn_width = 2 * btn_half_width + game->screen_width
		* 0.02;
	const int	btn_x_start = (game->screen_width - total_btn_width) / 2;

	check_buttons(game, mouse_x, mouse_y, btn_x_start);
	check_mouse_on_gear(game, mouse_x, mouse_y);
}

static void	update_join_server_menu_text2(t_game *game)
{
	if (game->menu->button_selected == 1)
	{
		game->client->ip[0] = '\0';
		game->menu->status = SERVERS;
		game->menu->server_selected = 0;
	}
	if (game->menu->button_selected == 2)
		handle_join_server(game);
	if (game->menu->button_selected == 5)
	{
		game->menu->status = OPTIONS_KEYBOARD;
		game->menu->last_status = JOIN_SERVER;
	}
}

void	update_join_server_menu_text(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.08;
	const int	spacing = game->screen_height * 0.06;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.40;

	if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y + 30
		&& mouse_y <= y + 30 + btn_height)
	{
		game->menu->text_field_selected = 1;
		game->menu->error_name = false;
	}
	else if (mouse_x >= x && mouse_x <= x + btn_width
		&& mouse_y >= y + btn_height + spacing + 30
		&& mouse_y <= y + btn_height + spacing + 30 + btn_height)
	{
		game->menu->text_field_selected = 2;
		game->menu->error_pseudo = false;
	}
	else
		game->menu->text_field_selected = 0;
	update_join_server_menu_text2(game);
}
