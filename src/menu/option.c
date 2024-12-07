/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:56:43 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/12/07 01:40:57 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_buton_click(t_game *game, int mouse_x, int mouse_y, int info[4])
{
	const int	key_x1 = (info[0] - info[0] * 0.5) * 0.5 + info[0] * 0.24;
	const int	key_x2 = key_x1 + info[0] * 0.2;
	const int	key_height = game->screen_height * 0.03 + 10;

	if (mouse_x >= info[1] && mouse_x <= info[1] + info[0]
		&& mouse_y >= info[2] && mouse_y <= info[2] + key_height)
	{
		if (mouse_x >= key_x1 - 5 && mouse_x <= key_x1 + info[0] * 0.15)
			game->menu->text_field_selected = info[4] * 2 + 1;
		else if (mouse_x >= key_x2 - 5 && mouse_x <= key_x2 + info[0] * 0.15)
			game->menu->text_field_selected = info[4] * 2 + 2;
	}
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
		update_buton_click(game, mouse_x, mouse_y, info);
		info[2] += 50;
	}
}

void	draw_mouse_check_box(t_game *game, int mouse_x, int mouse_y,
int info[5])
{
	const int	checkbox_size = 20;
	const int	checkbox_x = (info[2] + info[0] * 0.135) + info[0] * 0.5;
	const int	checkbox_y = info[4] - 5;

	if (mouse_x >= checkbox_x && mouse_x <= checkbox_x + checkbox_size
		&& mouse_y >= checkbox_y && mouse_y <= checkbox_y + checkbox_size)
		game->player->invert_mouse_x = !game->player->invert_mouse_x;
}

void	draw_mouse_slider(t_game *game, int mouse_x, int mouse_y, int info[5])
{
	const int	slider_x = (info[0] - info[0] * 0.3) * 0.5 + info[2];
	const int	slider_y = info[3] + info[1] * 0.4;
	const int	slider_width = info[0] * 0.3;
	const int	slider_height = game->screen_height * 0.025;

	if (mouse_x >= slider_x && mouse_x <= slider_x + slider_width
		&& mouse_y >= slider_y && mouse_y <= slider_y + slider_height)
	{
		game->menu->mouse_sensitivity = ((float)(mouse_x - slider_x)
				/ slider_width) * 100;
		if (game->menu->mouse_sensitivity < 0)
			game->menu->mouse_sensitivity = 0;
		if (game->menu->mouse_sensitivity > 100)
			game->menu->mouse_sensitivity = 100;
		game->menu->dragging = true;
	}
}

void	update_mouse_options_interaction(t_game *game, int mouse_x, int mouse_y)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	panel_x = (game->screen_width - panel_width) * 0.5;
	const float	panel_y = (game->screen_height - panel_height) * 0.5;
	int			info[5];

	info[0] = panel_width;
	info[1] = panel_height;
	info[2] = panel_x;
	info[3] = panel_y;
	info[4] = panel_y + panel_height * 0.372;
	draw_mouse_slider(game, mouse_x, mouse_y, info);
	info[4] += panel_height * 0.15;
	draw_mouse_check_box(game, mouse_x, mouse_y, info);
}

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

void	update_option_menu_click(t_game *game, int mouse_x, int mouse_y,
int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->status == OPTIONS_KEYBOARD)
		update_option_menu_click_keyboard(game, mouse_x, mouse_y);
	if (game->menu->status == OPTIONS_MOUSE)
		update_mouse_options_interaction(game, mouse_x, mouse_y);
	if (game->menu->status == OPTIONS_SOUND)
		update_sound_options_interaction(game, mouse_x, mouse_y);
	if (game->menu->button_selected == 1)
		game->menu->status = OPTIONS_KEYBOARD;
	else if (game->menu->button_selected == 2)
		game->menu->status = OPTIONS_MOUSE;
	else if (game->menu->button_selected == 3)
		game->menu->status = OPTIONS_SOUND;
	else if (game->menu->button_selected == 4)
		game->menu->status = game->menu->last_status;
	else
		return ;
	game->menu->button_selected = 0;
}

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

void	update_option_menu_button_mouse(t_game *game, int mouse_x, int mouse_y)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const int	slider_x = (panel_width - panel_width * 0.3)
		* 0.5 + (game->screen_width - panel_width) * 0.5;
	const int	slider_y = (game->screen_height - panel_height)
		* 0.5 + panel_height * 0.4;

	if (!game->menu->dragging)
		return ;
	if (!(mouse_x >= slider_x && mouse_x <= slider_x + panel_width * 0.3
			&& mouse_y >= slider_y
			&& mouse_y <= slider_y + game->screen_height * 0.025))
	{
		game->menu->dragging = false;
		return ;
	}
	game->menu->mouse_sensitivity = ((float)(mouse_x - slider_x)
			/ (panel_width * 0.3)) * 100;
	if (game->menu->mouse_sensitivity < 0.0001)
		game->menu->mouse_sensitivity = 0.0001;
	if (game->menu->mouse_sensitivity > 100)
		game->menu->mouse_sensitivity = 100;
}

void	update_option_menu_button2(t_game *game, int mouse_x, int mouse_y,
int panel_y)
{
	const float	x = (game->screen_width - (game->screen_width * 0.9) * 0.25)
		* 0.5;
	const float	y = panel_y + (game->screen_height * 0.8) + 20;

	if (mouse_x >= x && mouse_x <= x + (game->screen_width * 0.9) * 0.25
		&& mouse_y >= y && mouse_y <= y + game->screen_height * 0.1)
		game->menu->button_selected = 4;
	if (game->menu->status == OPTIONS_MOUSE)
		update_option_menu_button_mouse(game, mouse_x, mouse_y);
	if (game->menu->status == OPTIONS_SOUND)
		update_option_menu_button_sound(game, mouse_x, mouse_y);
}

void	update_option_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const float	panel_x = (game->screen_width - (game->screen_width * 0.9))
		* 0.5;
	const float	panel_y = (game->screen_height - (game->screen_height * 0.8))
		* 0.7 - game->screen_height * 0.1;
	const float	x = (game->screen_width - (game->screen_width * 0.9) * 0.25)
		* 0.5;
	const float	y = panel_y + (game->screen_height * 0.8) + 20;

	game->menu->button_selected = 0;
	if (mouse_x >= panel_x && mouse_x <= panel_x + (game->screen_width * 0.9)
		&& mouse_y >= panel_y
		&& mouse_y <= panel_y + (game->screen_height * 0.8) * 0.095)
	{
		if (mouse_x <= panel_x + (game->screen_width * 0.9) * 0.33)
			game->menu->button_selected = 1;
		else if (mouse_x >= panel_x + (game->screen_width * 0.9) * 0.33
			&& mouse_x <= panel_x + 2 * (game->screen_width * 0.9) * 0.33)
			game->menu->button_selected = 2;
		else if (mouse_x >= panel_x + 2 * (game->screen_width * 0.9) * 0.33)
			game->menu->button_selected = 3;
	}
	update_option_menu_button2(game, mouse_x, mouse_y, panel_y);
}

static void	draw_slider(t_game *game, int x, int y, float value)
{
	const int	width = game->screen_width * 0.9 * 0.3;
	const int	filled_width = (int)(width * value);
	t_draw_info	info;
	const int	height = game->screen_height * 0.025;

	info = init_draw_info(height, "", x, y);
	info.width = width;
	info.color = 0x333333;
	draw_rectangle(game, info);
	info.width = filled_width;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	draw_rectangle(game, info);
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

static void	draw_key_highlight(t_game *game, int key_x, int key_height,
int info[3])
{
	t_draw_info	highlight_info;

	highlight_info = init_draw_info(key_height + 10, "", key_x - 10,
			info[2] - 10);
	highlight_info.width = info[0] * 0.15 + 10;
	highlight_info.color = 0xFFFFFF;
	draw_rectangle(game, highlight_info);
}

static void	draw_key_rectangles(t_game *game, int key_x1, int key_x2,
int info[3])
{
	t_draw_info	key_info;
	const int	key_height = game->screen_height * 0.03 + 10;

	key_info = init_draw_info(key_height, "", key_x1 - 5, info[2] - 5);
	key_info.width = info[0] * 0.15;
	key_info.color = MENU_BUTTON_SELECTED_COLOR;
	draw_rectangle(game, key_info);
	key_info.x = key_x2 - 5;
	draw_rectangle(game, key_info);
}

static void	draw_key_text(t_game *game, t_key_binding *bindings, int i,
int info[3])
{
	t_draw_info	key_info;
	const int	key_x1 = (info[0] - info[0] * 0.5) * 0.5 + info[0] * 0.24;
	const int	key_x2 = key_x1 + info[0] * 0.2;

	key_info = init_draw_info(game->screen_height * 0.03, bindings[i].key1,
			key_x1, info[2] + 8);
	key_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, key_info);
	key_info = init_draw_info(game->screen_height * 0.03, bindings[i].key2,
			key_x2, info[2] + 8);
	key_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, key_info);
}

static void	draw_binding_option(t_game *game, t_key_binding *bindings, int i,
int info[3])
{
	const int	key_x1 = (info[0] - info[0] * 0.5) * 0.5 + info[0] * 0.24;
	const int	key_x2 = key_x1 + info[0] * 0.2;
	const int	key_height = game->screen_height * 0.03 + 10;
	t_draw_info	key_info;

	key_info = init_draw_info(game->screen_height * 0.03, bindings[i].action,
			(info[0] - info[0] * 0.5) * 0.47 + info[1], info[2] + 8);
	key_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, key_info);
	if (game->menu->text_field_selected == (i * 2 + 1))
		draw_key_highlight(game, key_x1, key_height, info);
	else if (game->menu->text_field_selected == (i * 2 + 2))
		draw_key_highlight(game, key_x2, key_height, info);
	draw_key_rectangles(game, key_x1, key_x2, info);
	draw_key_text(game, bindings, i, info);
}

void	draw_options_keyboard(t_game *game)
{
	const float		panel_height = game->screen_height * 0.9;
	int				info[3];
	int				i;
	t_key_binding	*bindings;
	t_draw_info		header_info;

	info[0] = game->screen_width * 0.9;
	info[1] = (game->screen_width - info[0]) * 0.5;
	info[2] = (game->screen_height - panel_height) * 0.5 + panel_height * 0.30;
	bindings = get_binding(game);
	header_info = init_draw_info(game->screen_height * 0.06, "Key Bindings",
			info[1] + info[0] * 0.5, (game->screen_height - panel_height)
			* 0.5 + 50 + panel_height * 0.1);
	header_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, header_info);
	i = -1;
	while (++i < 8)
	{
		draw_binding_option(game, bindings, i, info);
		info[2] += 50;
	}
}

static void	draw_sound_header(t_game *game)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	panel_x = (game->screen_width - panel_width) * 0.5;
	const float	panel_y = (game->screen_height - panel_height) * 0.5;
	t_draw_info	header_info;

	header_info = init_draw_info(
			game->screen_height * 0.06, "Sound Settings",
			panel_x + panel_width * 0.5, panel_y + 50 + panel_height * 0.1);
	header_info.color = 0xFFFFFF;
	draw_text(game, header_info);
}

static void	draw_sound_slider(t_game *game, int i, float slider_x,
float slider_y)
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
	draw_slider(game, slider_x, slider_y, values[i] / 100);
}

void	draw_options_sound(t_game *game)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	slider_x = (panel_width - panel_width * 0.3)
		* 0.5 + (game->screen_width - panel_width) * 0.5;
	float		slider_y;
	int			i;

	draw_sound_header(game);
	i = -1;
	slider_y = (game->screen_height - panel_height) * 0.5 + panel_height * 0.4;
	while (++i < 3)
	{
		draw_sound_slider(game, i, slider_x, slider_y);
		slider_y += panel_height * 0.15;
	}
}

static void	draw_mouse_header(t_game *game)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	x = (game->screen_width - panel_width)
		* 0.5 + panel_width * 0.5;
	const float	y = (game->screen_height - panel_height)
		* 0.5 + 50 + panel_height * 0.1;
	t_draw_info	header_info;

	header_info = init_draw_info(game->screen_height * 0.06, "Mouse Options", x,
			y);
	header_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, header_info);
}

static void	draw_mouse_checkbox(t_game *game, float checkbox_x,
float checkbox_y, int checkbox_size)
{
	t_draw_info	checkbox_info;

	checkbox_info = init_draw_info(
			checkbox_size + 4, "", checkbox_x - 2, checkbox_y - 2);
	checkbox_info.color = 0xFFFFFF;
	checkbox_info.width = checkbox_size + 4;
	draw_rectangle(game, checkbox_info);
	checkbox_info = init_draw_info(checkbox_size, "", checkbox_x, checkbox_y);
	checkbox_info.color = MENU_BUTTON_COLOR;
	checkbox_info.width = checkbox_size;
	if (!game->player->invert_mouse_x)
		draw_rectangle(game, checkbox_info);
	checkbox_info.color = 0xFF0000;
	if (game->player->invert_mouse_x)
		draw_rectangle(game, checkbox_info);
}

static void	draw_mouse_option_name(t_game *game, int i, float panel_x,
int option_y)
{
	const float	panel_width = game->screen_width * 0.9;
	const char	*options[] = {"Mouse Sensitivity", "Invert Mouse X"};
	t_draw_info	info;

	info = init_draw_info(
			game->screen_height * 0.03, options[i],
			(panel_width - panel_width * 0.3) * 0.5 + panel_x, option_y);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, info);
}

void	draw_options_mouse(t_game *game)
{
	const float	panel_width = game->screen_width * 0.9;
	const float	panel_height = game->screen_height * 0.9;
	const float	panel_x = (game->screen_width - panel_width) * 0.5;
	const float	panel_y = (game->screen_height - panel_height) * 0.5;
	float		option_y;

	option_y = panel_y + panel_height * 0.372;
	draw_mouse_header(game);
	draw_mouse_option_name(game, 0, panel_x, option_y);
	draw_slider(game, (panel_width - panel_width * 0.3) * 0.5 + panel_x,
		panel_y + panel_height * 0.4, game->menu->mouse_sensitivity / 100);
	option_y += panel_height * 0.15;
	draw_mouse_option_name(game, 1, panel_x, option_y);
	draw_mouse_checkbox(game, (panel_x + panel_width * 0.135)
		+ panel_width * 0.5, option_y - 5, 20);
}

static void	draw_back_button_highlight(t_game *game)
{
	const float	button_width = (game->screen_width * 0.9) * 0.25;
	const float	button_height = game->screen_height * 0.09;
	const float	button_x = (game->screen_width - button_width) * 0.5;
	const float	button_y = (game->screen_height - game->screen_height * 0.9)
		* 0.5 + (game->screen_height * 0.9) * 0.9 - 25;
	t_draw_info	rect_info;

	rect_info = init_draw_info(
			button_height + 8, "", button_x - 4, button_y - 4);
	rect_info.width = button_width + 8;
	rect_info.radius = 10;
	rect_info.color = MENU_BUTTON_SELECTED_COLOR;
	draw_rounded_rectangle(game, rect_info);
}

static void	draw_back_button_base(t_game *game)
{
	const float	button_width = (game->screen_width * 0.9) * 0.25;
	const float	button_height = game->screen_height * 0.09;
	const float	button_x = (game->screen_width - button_width) * 0.5;
	const float	button_y = (game->screen_height - game->screen_height * 0.9)
		* 0.5 + (game->screen_height * 0.9) * 0.9 - 25;
	t_draw_info	rect_info;

	rect_info = init_draw_info(
			button_height, "", button_x, button_y);
	rect_info.color = MENU_BUTTON_COLOR;
	rect_info.width = button_width;
	draw_rectangle(game, rect_info);
}

static void	draw_back_button_text(t_game *game)
{
	t_draw_info	info;
	const float	button_width = (game->screen_width * 0.9) * 0.25;
	const float	button_height = game->screen_height * 0.09;
	const float	button_x = (game->screen_width - button_width) * 0.5;
	const float	button_y = (game->screen_height - game->screen_height * 0.9)
		* 0.5 + (game->screen_height * 0.9) * 0.9 - 25;

	info = init_draw_info(
			button_height * 0.6, "Back", button_x + button_width * 0.5,
			button_y + button_height * 0.33 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_back_buttons(t_game *game)
{
	if (game->menu->button_selected == 4)
		draw_back_button_highlight(game);
	draw_back_button_base(game);
	draw_back_button_text(game);
}

void	draw_options_bg(t_game *game)
{
	const float	rect_width = game->screen_width * 0.9;
	const float	rect_height = game->screen_height * 0.8;
	const float	rect_x = (game->screen_width - rect_width) * 0.5;
	const float	rect_y = (game->screen_height - rect_height)
		* 0.7 - game->screen_height * 0.1 - 20;
	t_draw_info	rect_info;

	rect_info = init_draw_info(rect_height, "", rect_x, rect_y);
	rect_info.color = MENU_BUTTON_COLOR;
	rect_info.width = rect_width;
	rect_info.radius = 25;
	draw_rounded_rectangle(game, rect_info);
}

void	draw_options_button(t_game *game, float x, float y, int i)
{
	t_draw_info	info;
	const char	*labels[] = {"Keyboard", "Mouse", "Sound"};
	const float	text_width = get_text_width(game, (char *)labels[i],
			(int)(game->screen_height * 0.8 * 0.095));
	const float	width = (game->screen_width * 0.9) / 3;
	const float	text_x = x + (width - text_width) * 0.5;

	info = init_draw_info(game->screen_height * 0.8 * 0.095, labels[i], x, y);
	info.width = width;
	if (game->menu->status == (OPTIONS_KEYBOARD + i))
		info.color = MENU_BUTTON_SELECTED_COLOR;
	else if (game->menu->button_selected == (i + 1))
		info.color = MENU_BUTTON_COLOR & 0x666666;
	else
		info.color = MENU_BUTTON_COLOR;
	if (i == 1 || i == 2)
	{
		info.x += 2;
		info.width = width - 2;
	}
	draw_rectangle(game, info);
	info = init_draw_info(game->screen_height * 0.8 * 0.095,
			labels[i], text_x, 30);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text_left(game, info);
}

void	draw_section_separator(t_game *game, float x, float y, float height)
{
	t_draw_info	line_info;

	line_info = init_draw_info(2, "", x, y);
	line_info.width = 2;
	line_info.height = height;
	line_info.color = 0xFFFFFF;
	draw_rectangle(game, line_info);
}

void	draw_options_all_buttons(t_game *game, int start_y, int button_height)
{
	const float	section_width = game->screen_width * 0.9 / 3;
	const float	rect_x = (game->screen_width - game->screen_width * 0.9) * 0.5;
	const float	rect_y = (game->screen_height - game->screen_height * 0.8) - 20;
	int			i;

	i = -1;
	while (++i < 3)
	{
		draw_options_button(game, rect_x + section_width * i,
			start_y - button_height * 0.5, i);
		if (i < 3 - 1)
			draw_section_separator(game, rect_x + section_width * (i + 1),
				23, game->screen_height * 0.8 * 0.095);
	}
}

void	draw_options_menu(t_game *game)
{
	const float	rect_x = (game->screen_width - game->screen_width * 0.9) * 0.5;
	const float	rect_y = (game->screen_height - game->screen_height * 0.8)
		* 0.7 - game->screen_height * 0.1 - 20;
	const float	button_height = game->screen_height * 0.05;
	const float	start_y = rect_y + game->screen_height * 0.8 * 0.03;
	t_draw_info	line_info;

	draw_options_bg(game);
	line_info = init_draw_info(2, "", rect_x, start_y + button_height);
	line_info.width = game->screen_width * 0.9;
	line_info.color = 0xFFFFFF;
	draw_rectangle(game, line_info);
	draw_options_all_buttons(game, start_y, button_height);
	if (game->menu->status == OPTIONS_KEYBOARD)
		draw_options_keyboard(game);
	else if (game->menu->status == OPTIONS_MOUSE)
		draw_options_mouse(game);
	else if (game->menu->status == OPTIONS_SOUND)
		draw_options_sound(game);
	draw_back_buttons(game);
}
