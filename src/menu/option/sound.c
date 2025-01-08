/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:55:23 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 18:02:02 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_sound_slider(t_game *game, int mouse_x, int mouse, int i)
{
	const float	panel_width = game->screen_width * 0.9;
	const int	slider_x = (panel_width - panel_width * 0.3)
		* 0.5 + (game->screen_width - panel_width) * 0.5;
	float		*values;

	if (i == 0)
		values = &game->menu->music_volume;
	else if (i == 1)
		values = &game->menu->effects_volume;
	else
		values = &game->menu->menu_music_volume;
	*values = ((float)(mouse_x - slider_x) / (panel_width * 0.3)) * 100;
	if (*values < 0)
		*values = 0;
	if (*values > 100)
		*values = 100;
	game->menu->dragging = true;
}

void	update_sound_options_interaction(t_game *game, int mouse_x, int mouse_y)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const int	slider_x = (panel_width - panel_width * 0.3)
		* 0.5 + (game->screen_width - panel_width) * 0.5;
	int			slider_y;
	int			i;

	slider_y = (game->screen_height - panel_height) * 0.5 + panel_height * 0.4;
	i = -1;
	while (++i < 3)
	{
		if (mouse_x >= slider_x && mouse_x <= slider_x + panel_width * 0.3
			&& mouse_y >= slider_y
			&& mouse_y <= slider_y + game->screen_height * 0.025)
			update_sound_slider(game, mouse_x, mouse_y, i);
		slider_y += panel_height * 0.15;
	}
}

void	update_option_menu_button_sound(t_game *game, int mouse_x, int mouse_y)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const int	slider_x = (panel_width - panel_width * 0.3)
		* 0.5 + (game->screen_width - panel_width) * 0.5;
	int			slider_y;
	int			i;

	slider_y = (game->screen_height - panel_height) * 0.5 + panel_height * 0.4;
	if (!game->menu->dragging)
		return ;
	i = -1;
	while (++i < 3)
	{
		if (mouse_x >= slider_x && mouse_x <= slider_x + panel_width * 0.3
			&& mouse_y >= slider_y && mouse_y <= slider_y
			+ game->screen_height * 0.025)
		{
			update_sound_slider(game, mouse_x, mouse_y, i);
			return ;
		}
		slider_y += panel_height * 0.15;
	}
	game->menu->dragging = false;
}

void	draw_sound_slider(t_game *game, int i, const float slider_x, float slider_y)
{
	const char	*labels[] = {
		"Music Volume", "Effects Volume", "Menu Music Volume"
	};
	float		values[3];
	t_draw_info	label_info;

	values[0] = game->menu->music_volume;
	values[1] = game->menu->effects_volume;
	values[2] = game->menu->menu_music_volume;
	label_info = init_draw_info(
			game->screen_height * 0.03, labels[i],
			slider_x, slider_y - game->screen_height * 0.025);
	label_info.color = 0xFFFFFF;
	draw_text_left(game, label_info);
	draw_slider(game, slider_x, slider_y, values[i]);
}
