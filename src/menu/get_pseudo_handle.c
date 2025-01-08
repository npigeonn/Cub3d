/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pseudo_handle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:54:32 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/02 18:54:40 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	handle_keycode_input(t_game *game, char *field, int keycode,
int len)
{
	if (len >= 15)
		return ;
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

static void	handle_backspace_or_enter(t_game *game, int keycode, int len)
{
	if (keycode == 65288 && len > 0)
		game->client->pseudo[len - 1] = '\0';
	else if (keycode == 65293 && game->menu->text_field_selected == 1)
	{
		if (game->client->pseudo[0] == '\0')
			game->menu->error_name = true;
		else
			game->menu->status = MAIN_MENU;
	}
}

void	handle_pseudo_input(t_game *game, int keycode)
{
	char	*field;
	int		len;

	field = game->client->pseudo;
	if (!field)
		return ;
	len = ft_strlen(field);
	if ((keycode >= 32 && keycode <= 126)
		|| (keycode >= 65429 && keycode <= 65439))
		handle_keycode_input(game, field, keycode, len);
	else
		handle_backspace_or_enter(game, keycode, len);
}
