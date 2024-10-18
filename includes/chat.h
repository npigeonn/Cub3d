
#ifndef CHAT_H
# define CHAT_H

typedef struct s_draw_info
{
	int		width;
	int		height;
	char	*str;
	int		color;
	int		x;
	int		y;
	int		max_width;
	float	alpha;
}	t_draw_info;


t_draw_info	init_draw_info(int height, char *str, int x, int y);
void		add_message(t_game *game);
void		draw_wrapped_text_left(t_game *game, t_draw_info info);
void		draw_text_max_right(t_game *game, t_draw_info info);
void		chat_input(t_game *game, int keycode);
void		handle_mouse_chat(t_game *game, int x, int y, int keycode);
void		show_message(t_game *game);

#endif