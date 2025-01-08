/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:31:09 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/03 18:34:08 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	handle_create_server(t_game *game)
{
	if (game->client->name[0] == '\0')
		game->menu->error_name = true;
	if (game->client->name[0] != '\0')
		game->menu->status = VALID_SERVER_CREATE;
}
