/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:40:24 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 12:27:17 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_info(t_game *game, t_draw_info *info, t_draw_info *info2,
int list_x)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	remaining_space = game->screen_width
		- (list_x + game->screen_width * 0.65);
	const int	btn_x = list_x + game->screen_width
		* 0.65 + (remaining_space - btn_width) * 0.5;
	const int	btn_height = game->screen_height * 0.1;

	*info = init_draw_info(game->screen_height * 0.1 * 0.5, "",
			list_x + game->screen_width * 0.65 * 0.5, game->screen_height
			* 0.25 + 20);
	info->color = MENU_BUTTON_TEXT_COLOR;
	*info2 = init_draw_info(0, "", btn_x, game->screen_height * 0.25);
	info2->height = btn_height;
	info2->width = btn_width;
	info2->color = MENU_BUTTON_COLOR;
}

void	get_ip_server(t_game *game)
{
	t_server_info	*current;
	int				i;

	i = 1;
	current = game->servers;
	while (current)
	{
		if (i == game->menu->server_selected)
		{
			ft_strcpy(game->client->ip, current->ip);
			game->menu->status = VALID_JOIN_SERVER;
			break ;
		}
		current = current->next;
		i++;
	}
}
