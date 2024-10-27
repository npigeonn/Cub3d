/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 14:42:20 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/27 21:57:33 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_option_menu_slider(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	const int		btn_width = game->screen_width * 0.25;
	const int		btn_height = game->screen_height * 0.1;
	const int		spacing = game->screen_height * 0.05;
	const int		x = (game->screen_width - btn_width) * 0.5;
	const int		y = game->screen_height * 0.25;

	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
	{
		game->menu->volume = (mouse_x - x) * 100 / btn_width;
		if (game->menu->volume < 0) game->menu->volume = 0;
		if (game->menu->volume > 100) game->menu->volume = 100;
		game->menu->dragging = true;
	}
	else if (game->menu->button_selected == 2)
	{
		game->menu->mouse_sensitivity = (mouse_x - x) * 100 / btn_width;
		if (game->menu->mouse_sensitivity < 0.0001) game->menu->mouse_sensitivity = 0.0001;
		if (game->menu->mouse_sensitivity > 100) game->menu->mouse_sensitivity = 100;
		game->menu->dragging = true;
	}
	else
		game->menu->dragging = false;
}

void	update_option_menu_click(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
	{
		game->menu->status = OPTIONS_KEYBOARD;
		game->menu->button_selected = 0;
		return ;
	}
	if (game->menu->button_selected == 2)
	{
		game->menu->status = OPTIONS_MOUSE;
		game->menu->button_selected = 0;
		return ;
	}
	if (game->menu->button_selected == 3)
	{
		game->menu->status = OPTIONS_SOUND;
		game->menu->button_selected = 0;
		return ;
	}
	if (game->menu->button_selected == 4)
	{
		game->menu->status = game->menu->last_status;
		game->menu->button_selected = 0;
		return ;
	}
	update_option_menu_slider(game, mouse_x, mouse_y, keycode);
}

void	update_option_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	float panel_width = game->screen_width * 0.9;
	float panel_height = game->screen_height * 0.8;
	float panel_x = (game->screen_width - panel_width) * 0.5;
	float panel_y = (game->screen_height - panel_height) * 0.7 - game->screen_height * 0.1;

	float btn_width = panel_width * 0.25;
	float btn_height = game->screen_height * 0.1;
	float x = (game->screen_width - btn_width) * 0.5;
	float y = panel_y + panel_height + 20;

	int section_width = panel_width * 0.33;

	if (mouse_x >= panel_x && mouse_x <= panel_x + panel_width && mouse_y >= panel_y && mouse_y <= panel_y + panel_height * 0.095)
	{
		if (mouse_x <= panel_x + section_width)
			game->menu->button_selected = 1;
		else if (mouse_x >= panel_x + section_width && mouse_x <= panel_x + 2 * section_width)
			game->menu->button_selected = 2;
		else if (mouse_x >= panel_x + 2 * section_width)
			game->menu->button_selected = 3;
	}
	else if (mouse_x >= x && mouse_x <= x + btn_width && mouse_y >= y && mouse_y <= y + btn_height)
		game->menu->button_selected = 4;
	else
	{
		game->menu->button_selected = 0;
		game->menu->dragging = false;
	}
}

static void	draw_slider(t_game *game, int x, int y, int width, int height, float value)
{
	const int	filled_width = (int)(width * value);
	t_draw_info	info;

	info = init_draw_info(height, "", x, y);
	info.width = width;
	info.color = 0x333333;
	draw_rectangle(game, info);
	info.width = filled_width;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	draw_rectangle(game, info);
}

typedef struct	s_key_binding
{
	const char	*action;
	const char	*key1;
	const char	*key2;
}	t_key_binding;

const char *get_key_name(Display *display, int key_code)
{
    // Créer un événement XKeyEvent temporaire pour XLookupString
    char buffer[32];
    KeySym keysym;
    XKeyEvent event;
    
    // Initialiser l'événement
    event.display = display;
    event.keycode = key_code;
    event.state = 0; // Sans aucun modificateur

	keysym = XKeycodeToKeysym(display, key_code, 0);
    const char *key_name = XKeysymToString(keysym);
	printf("key_name: %s\n", key_name);
	printf("keysym: %d\n", keysym);
    return key_name ? key_name : "none";
}

t_key_binding	*get_binding(t_game *game)
{
	t_key_binding *binding = malloc(sizeof(t_key_binding) * 9);
	t_keycode *key = game->player->key;

	binding[0] = (t_key_binding){"Move Forward", get_key_name(((t_xvar *)game->mlx)->display, key->up), get_key_name(((t_xvar *)game->mlx)->display, key->up2)};
	binding[1] = (t_key_binding){"Move Backward", get_key_name(((t_xvar *)game->mlx)->display, key->down), get_key_name(((t_xvar *)game->mlx)->display, key->down2)};
	binding[2] = (t_key_binding){"Move Left", get_key_name(((t_xvar *)game->mlx)->display, key->left), get_key_name(((t_xvar *)game->mlx)->display, key->left2)};
	binding[3] = (t_key_binding){"Move Right", get_key_name(((t_xvar *)game->mlx)->display, key->right), get_key_name(((t_xvar *)game->mlx)->display, key->right2)};
	binding[4] = (t_key_binding){"Jump", get_key_name(((t_xvar *)game->mlx)->display, key->jump), get_key_name(((t_xvar *)game->mlx)->display, key->jump2)};
	binding[5] = (t_key_binding){"Use", get_key_name(((t_xvar *)game->mlx)->display, key->use), get_key_name(((t_xvar *)game->mlx)->display, key->use2)};
	binding[6] = (t_key_binding){"Quit", get_key_name(((t_xvar *)game->mlx)->display, key->escape), get_key_name(((t_xvar *)game->mlx)->display, key->escape2)};
	binding[7] = (t_key_binding){"Pause", get_key_name(((t_xvar *)game->mlx)->display, key->pause), get_key_name(((t_xvar *)game->mlx)->display, key->pause2)};
	binding[8] = (t_key_binding){"Chat", get_key_name(((t_xvar *)game->mlx)->display, key->chat), get_key_name(((t_xvar *)game->mlx)->display, key->chat2)};
	return (binding);
}

void draw_options_keyboard(t_game *game)
{
	float panel_width = game->screen_width * 0.9;
	float panel_height = game->screen_height * 0.9;
	float panel_x = (game->screen_width - panel_width) * 0.5;
	float panel_y = (game->screen_height - panel_height) * 0.5;

	t_key_binding *bindings = get_binding(game);

	t_draw_info header_info = init_draw_info(game->screen_height * 0.06, "Key Bindings", panel_x + panel_width * 0.5, panel_y + 50 + panel_height * 0.1);
	header_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, header_info);

	int option_x = (panel_width - panel_width * 0.5) * 0.47 + panel_x;
	int option_y = panel_y + panel_height * 0.30;

	for (int i = 0; i < 9; i++)
	{
		t_draw_info key_info = init_draw_info(game->screen_height * 0.03, bindings[i].action, option_x, option_y + 8);
		key_info.color = MENU_BUTTON_TEXT_COLOR;
		draw_text_left(game, key_info);
		int key_x1 =(panel_width - panel_width * 0.5) * 0.5 + panel_width * 0.24;
		int key_x2 = key_x1 + panel_width * 0.2;
		int key_height = game->screen_height * 0.03 + 10;
		int key_width = panel_width * 0.15;
		key_info = init_draw_info(key_height, "", key_x1 - 5,  option_y - 5);
		key_info.width = key_width;
		key_info.color = MENU_BUTTON_SELECTED_COLOR;
		draw_rectangle(game, key_info);
		key_info.x = key_x2 - 5;
		draw_rectangle(game, key_info);
		key_info = init_draw_info(game->screen_height * 0.03, bindings[i].key1, key_x1, option_y + 8);
		key_info.color = MENU_BUTTON_TEXT_COLOR;
		draw_text_left(game, key_info);
		key_info = init_draw_info(game->screen_height * 0.03, bindings[i].key2, key_x2, option_y + 8);
		key_info.color = MENU_BUTTON_TEXT_COLOR;
		draw_text_left(game, key_info);
		option_y += 50;
	}
}

void draw_options_sound(t_game *game)
{
	float panel_width = game->screen_width * 0.9;
	float panel_height = game->screen_height * 0.9;
	float panel_x = (game->screen_width - panel_width) * 0.5;
	float panel_y = (game->screen_height - panel_height) * 0.5;

	int slider_width = panel_width * 0.3;
	int slider_height = game->screen_height * 0.025;

	float music_volume = 0.7;
	float effects_volume = 0.5;
	float menu_music_volume = 0.8;

	const char *labels[] = {"Music Volume", "Effects Volume", "Menu Music Volume"};
	float values[] = {music_volume, effects_volume, menu_music_volume};

	t_draw_info header_info = init_draw_info(game->screen_height * 0.06, "Sound Settings", panel_x + panel_width * 0.5, panel_y + 50 + panel_height * 0.1);
	header_info.color = 0xFFFFFF;
	draw_text(game, header_info);

	int slider_x = (panel_width - panel_width * 0.3) * 0.5 + panel_x;
	int slider_y = panel_y + panel_height * 0.4;

	for (int i = 0; i < 3; i++)
	{
		t_draw_info label_info = init_draw_info(game->screen_height * 0.03, labels[i], slider_x, slider_y - slider_height);
		label_info.color = 0xFFFFFF;
		draw_text_left(game, label_info);
		draw_slider(game, slider_x, slider_y, slider_width, slider_height, values[i]);
		slider_y += panel_height * 0.15;
	}
}

void draw_options_mouse(t_game *game) 
{
	float panel_width = game->screen_width * 0.9;
	float panel_height = game->screen_height * 0.9;
	float panel_x = (game->screen_width - panel_width) * 0.5;
	float panel_y = (game->screen_height - panel_height) * 0.5;

	t_draw_info header_info = init_draw_info(game->screen_height * 0.06, "Mouse Options", panel_x + panel_width * 0.5, panel_y + 50 + panel_height * 0.1);
	header_info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, header_info);

	const char *options[] = {"Mouse Sensitivity", "Invert Mouse X"};
	float sensitivity_value = 0.5;
	int invert_x = 1;
	int option_count = sizeof(options) / sizeof(options[0]);
	float option_y = panel_y + panel_height * 0.35;

	for (int i = 0; i < option_count; i++)
	{
		t_draw_info info = init_draw_info(game->screen_height * 0.03, options[i], (panel_width - panel_width * 0.3) * 0.5 + panel_x, option_y);
		info.color = MENU_BUTTON_TEXT_COLOR;
		draw_text_left(game, info);

		if (i == 0)
		{
			int slider_x = (panel_width - panel_width * 0.3) * 0.5 + panel_x;
			int slider_y = panel_y + panel_height * 0.4;
			int slider_width = panel_width * 0.3;
			int slider_height = game->screen_height * 0.025;

			draw_slider(game, slider_x, slider_y, slider_width, slider_height, sensitivity_value);
		}
		if (i == 1)
		{
			int checkbox_size = 20;
			int checkbox_x = (panel_x + panel_width * 0.1) + panel_width * 0.5 - checkbox_size * 0.5;
			int checkbox_y = option_y - 5;

			t_draw_info checkbox_info = init_draw_info(checkbox_size, "", checkbox_x, checkbox_y);
			checkbox_info.color = MENU_BUTTON_COLOR;
			checkbox_info.width = checkbox_size;
			draw_rectangle(game, checkbox_info);
			checkbox_info.color = 0xFF0000;
			if (invert_x)
				draw_rectangle(game, checkbox_info);
		}
		option_y += panel_height * 0.15;
	}
}

void	draw_back_buttons(t_game *game)
{
	float panel_width = game->screen_width * 0.9;
	float panel_height = game->screen_height * 0.9;
	float panel_x = (game->screen_width - panel_width) * 0.5;
	float panel_y = (game->screen_height - panel_height) * 0.5;

	float button_width = panel_width * 0.25;
	float button_height = game->screen_height * 0.1;
	float button_x = (game->screen_width - button_width) * 0.5;
	float button_y = panel_y + panel_height * 0.9;

	t_draw_info rect_info = init_draw_info(button_height + 8, "", button_x - 4, button_y - 4);
	rect_info.width = button_width + 8;
	rect_info.radius = 10;
	rect_info.color = MENU_BUTTON_SELECTED_COLOR;
	if (game->menu->button_selected == 4)
		draw_rounded_rectangle(game, rect_info);
	rect_info = init_draw_info(button_height, "", button_x, button_y);
	rect_info.color = MENU_BUTTON_COLOR;
	rect_info.width = button_width;
	draw_rectangle(game, rect_info);
	t_draw_info info = init_draw_info(button_height * 0.5, "Back", button_x + button_width * 0.5, button_y + button_height * 0.33 - 5);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_options_menu(t_game *game)
{
	float rect_width = game->screen_width * 0.9;
	float rect_height = game->screen_height * 0.8;
	float rect_x = (game->screen_width - rect_width) * 0.5;
	float rect_y = (game->screen_height - rect_height) * 0.7 - game->screen_height * 0.1;

	t_draw_info rect_info = init_draw_info(rect_height, "", rect_x, rect_y);
	rect_info.color = MENU_BUTTON_COLOR;
	rect_info.width = rect_width;
	rect_info.radius = 25;
	draw_rounded_rectangle(game, rect_info);

	const char *buttons[] = {"Keyboard", "Mouse", "Sound"};
	int button_count = sizeof(buttons) / sizeof(buttons[0]);
	float button_height = game->screen_height * 0.05;

	float section_width = rect_width * 0.33; 
	float start_y = rect_y + rect_height * 0.03;

	for (int i = 0; i < button_count; i++)
	{
		float section_center_x = rect_x + (section_width * i) + section_width * 0.5;
		float text_width = get_text_width(game, (char *)buttons[i], (int)button_height);
		float pos_x = section_center_x - text_width * 0.5;

		if (i == 0)
		{
			t_draw_info info = init_draw_info(rect_height * 0.095, "", rect_x + section_width * i, start_y - button_height * 0.5);
			info.width = section_width;
			if (game->menu->status == OPTIONS_KEYBOARD)
				info.color = MENU_BUTTON_SELECTED_COLOR;
			else if (game->menu->button_selected == 1)
				info.color = MENU_BUTTON_COLOR & 0x666666;
			else
				info.color = MENU_BUTTON_COLOR;
			draw_rectangle(game, info);
		}
		else if (i == 1)
		{
			t_draw_info info = init_draw_info(rect_height * 0.095, "", rect_x + section_width * i + 2, start_y - button_height * 0.5);
			info.width = section_width;
			if (game->menu->status == OPTIONS_MOUSE)
				info.color = MENU_BUTTON_SELECTED_COLOR;
			else if (game->menu->button_selected == 2)
				info.color = MENU_BUTTON_COLOR & 0x666666;
			else
				info.color = MENU_BUTTON_COLOR;
			draw_rectangle(game, info);
		}
		else if (i == 2)
		{
			t_draw_info info = init_draw_info(rect_height * 0.095, "", rect_x + section_width * i + 2, start_y - button_height * 0.5);
			info.width = rect_width - section_width * 2 - 1;
			if (game->menu->status == OPTIONS_SOUND)
				info.color = MENU_BUTTON_SELECTED_COLOR;
			else if (game->menu->button_selected == 3)
				info.color = MENU_BUTTON_COLOR & 0x666666;
			else
				info.color = MENU_BUTTON_COLOR;
			draw_rectangle(game, info);
		}
		if (i < button_count -1)
		{
			t_draw_info line_info = init_draw_info(2, "", rect_x + section_width * (i + 1), rect_y);
			line_info.width = 2;
			line_info.height = rect_height * 0.095;
			line_info.color = 0xFFFFFF;
			draw_rectangle(game, line_info);
		}
		t_draw_info info = init_draw_info(button_height, buttons[i], pos_x, start_y);
		info.color = MENU_BUTTON_TEXT_COLOR;
		draw_text_left(game, info);
	}
	t_draw_info line_info = init_draw_info(2, "", rect_x, start_y + button_height);
	line_info.width = rect_width;
	line_info.color = 0xFFFFFF;
	draw_rectangle(game, line_info);
	if (game->menu->status == OPTIONS_MOUSE)
		draw_options_mouse(game);
	else if (game->menu->status == OPTIONS_SOUND)
		draw_options_sound(game);
	else if (game->menu->status == OPTIONS_KEYBOARD)
		draw_options_keyboard(game);
	draw_back_buttons(game);
}
