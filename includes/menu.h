/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:24:35 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 22:19:40 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_H
# define MENU_H

typedef struct s_line
{
	int	delta_x;
	int	delta_y;
	int	step_x;
	int	step_y;
	int	x1;
	int	y1;
	int	x2;
	int	y2;
}	t_line;

t_line			init_line(int x1, int y1, int x2, int y2);

// create_server
char			*get_local_ip(t_game *game);
char			*get_field(t_game *game);
void			draw_create_server_menu(t_game *game);
void			draw_labeled_text_field(t_game *game, char *content,
					t_draw_info info2);
void			valid_name_server(t_game *game);
void			update_server_name(t_game *game, int keycode, int len,
					char *field);
void			handle_create_server(t_game *game);
void			update_create_server_menu_text(t_game *game, int mouse_x,
					int mouse_y, int keycode);
void			update_create_server_menu_button(t_game *game, int mouse_x,
					int mouse_y);

// options
t_key_binding	*get_binding(t_game *game);
void			draw_slider(t_game *game, int x, int y, float value);
void			draw_sound_slider(t_game *game, int i, float slider_x,
					float slider_y);
void			update_buton_option_click(t_game *game, int mouse_x,
					int mouse_y, int info[4]);
void			draw_options_keyboard(t_game *game);
void			draw_options_mouse(t_game *game);
void			draw_options_sound(t_game *game);
void			update_option_menu_click_keyboard(t_game *game, int mouse_x,
					int mouse_y);
void			update_option_menu_click(t_game *game, int mouse_x, int mouse_y,
					int keycode);
void			update_mouse_options_interaction(t_game *game, int mouse_x,
					int mouse_y);
void			update_sound_options_interaction(t_game *game, int mouse_x,
					int mouse_y);
void			update_option_menu_button_mouse(t_game *game, int mouse_x,
					int mouse_y);
void			update_option_menu_button_sound(t_game *game, int mouse_x,
					int mouse_y);
void			draw_back_option_buttons(t_game *game);
void			update_option_menu_key_keyboard(t_game *game, int keycode);
void			draw_options_button(t_game *game, float x, float y, int i);

// main
void			draw_selected_button_main(t_game *game);
void			update_main_menu_click(t_game *game, int keycode);

// join_server
void			draw_join_server_menu(t_game *game);
void			update_join_server_menu_text(t_game *game, int mouse_x,
					int mouse_y);
void			update_join_server_menu_button(t_game *game, int mouse_x,
					int mouse_y);

// game_over
void			draw_game_over(t_game *game);
void			draw_success(t_game *game);
void			draw_game_over_bg(t_game *game);
void			draw_game_over_text(t_game *game);
void			update_game_over_click(t_game *game, int keycode);
void			update_game_over_button(t_game *game, int mouse_x, int mouse_y);

// pseudo
void			draw_get_pseudo_menu(t_game *game);
void			update_get_pseudo_click(t_game *game, int mouse_x, int mouse_y,
					int keycode);
void			update_get_pseudo_button(t_game *game, int mouse_x,
					int mouse_y);
void			handle_pseudo_input(t_game *game, int keycode);

// disconnected
void			draw_server_error_menu(t_game *game);
void			update_server_error_click(t_game *game, int keycode);
void			update_server_error_button(t_game *game, int mouse_x,
					int mouse_y);

// stats
void			draw_stats_menu(t_game *game);
void			draw_back_stats_button(t_game *game, int y, int stats_height);
void			draw_player_stats_row(t_game *game, t_player_stats player,
					t_draw_info info, int index);
void			draw_stats_menu_bg(t_game *game, int stats_width,
					int stats_height);
void			draw_stats_menu_header(t_game *game, int stats_width,
					int stats_height);
int				draw_stats_menu_player_scroll(t_game *game, int x, int y,
					int stats_height);
void			is_dragging_scroll_bar(t_game *game, int y, int scroll_bar_y);
void			update_stats_menu_click(t_game *game, int x, int y,
					int keycode);
void			update_stats_menu(t_game *game, int x, int y);
bool			is_a_player(char *line);

// multi
void			purge_old_servers(t_game *game, time_t now);
void			show_list_servers(t_game *game, int list_x, int list_y);
void			init_info(t_game *game, t_draw_info *info, t_draw_info *info2,
					int list_x);
void			get_ip_server(t_game *game);
void			show_bg_servers_btn(t_game *game, int list_x,
					int server_y_offset, int i);
void			update_multiplayer_click(t_game *game, int keycode);

// utils
void			draw_text_field(t_game *game, t_draw_info info, char *text);
void			draw_gear_icon(t_game *game, int x, int y, int size);
void			check_mouse_on_gear(t_game *game, int mouse_x, int mouse_y);
char			get_keycode_pad(int keycode);

// sharpe
void			draw_line(t_game *game, t_line line, int color);
void			draw_circle(t_game *game, t_draw_info info);

#endif