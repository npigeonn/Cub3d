/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:08:06 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:30:53 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_keys(t_game *game, int ***primary, int ***secondary)
{
	static int	*primary_keys[8];
	static int	*secondary_keys[8];

	primary_keys[0] = &game->player->key->up;
	primary_keys[1] = &game->player->key->down;
	primary_keys[2] = &game->player->key->left;
	primary_keys[3] = &game->player->key->right;
	primary_keys[4] = &game->player->key->use;
	primary_keys[5] = &game->player->key->escape;
	primary_keys[6] = &game->player->key->pause;
	primary_keys[7] = &game->player->key->chat;
	secondary_keys[0] = &game->player->key->up2;
	secondary_keys[1] = &game->player->key->down2;
	secondary_keys[2] = &game->player->key->left2;
	secondary_keys[3] = &game->player->key->right2;
	secondary_keys[4] = &game->player->key->use2;
	secondary_keys[5] = &game->player->key->escape2;
	secondary_keys[6] = &game->player->key->pause2;
	secondary_keys[7] = &game->player->key->chat2;
	*primary = primary_keys;
	*secondary = secondary_keys;
}

void	update_option_menu_click_keyboard(t_game *game, int mouse_x,
int mouse_y)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	int			info[5];

	info[0] = panel_width;
	info[1] = (game->screen_width - panel_width) * 0.5;
	info[2] = (game->screen_height - panel_height) * 0.5 + panel_height * 0.30;
	info[3] = panel_height;
	game->menu->text_field_selected = 0;
	info[4] = -1;
	while (++info[4] < 8)
	{
		update_buton_option_click(game, mouse_x, mouse_y, info);
		info[2] += 50;
	}
}

void	update_option_menu_key_keyboard(t_game *game, int keycode)
{
	int	primary_index;
	int	secondary_index;
	int	**primary_keys;
	int	**secondary_keys;
	int	i;

	get_keys(game, &primary_keys, &secondary_keys);
	if (game->menu->text_field_selected == 0)
		return ;
	i = -1;
	while (++i < 8)
	{
		primary_index = i * 2 + 1;
		secondary_index = i * 2 + 2;
		if (*primary_keys[i] == keycode)
			*primary_keys[i] = -1;
		else if (*secondary_keys[i] == keycode)
			*secondary_keys[i] = -1;
		if (game->menu->text_field_selected == primary_index)
			*primary_keys[i] = keycode;
		else if (game->menu->text_field_selected == secondary_index)
			*secondary_keys[i] = keycode;
	}
	game->menu->text_field_selected = 0;
}

t_key_binding	*get_binding(t_game *game)
{
	t_key_binding	*binding;
	t_keycode		*key;

	binding = malloc(sizeof(t_key_binding) * 8);
	key = game->player->key;
	binding[0] = (t_key_binding){"Move Forward", get_key_name(game, key->up),
		get_key_name(game, key->up2)};
	binding[1] = (t_key_binding){"Move Backward", get_key_name(game, key->down),
		get_key_name(game, key->down2)};
	binding[2] = (t_key_binding){"Move Left", get_key_name(game, key->left),
		get_key_name(game, key->left2)};
	binding[3] = (t_key_binding){"Move Right", get_key_name(game, key->right),
		get_key_name(game, key->right2)};
	binding[4] = (t_key_binding){"Use", get_key_name(game, key->use),
		get_key_name(game, key->use2)};
	binding[5] = (t_key_binding){"Quit", get_key_name(game, key->escape),
		get_key_name(game, key->escape2)};
	binding[6] = (t_key_binding){"Pause", get_key_name(game, key->pause),
		get_key_name(game, key->pause2)};
	binding[7] = (t_key_binding){"Chat", get_key_name(game, key->chat),
		get_key_name(game, key->chat2)};
	return (binding);
}
