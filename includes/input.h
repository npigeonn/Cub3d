
#ifndef INPUT_H
#define INPUT_H

# define KEY_W 119
# define KEY_S 115
# define KEY_A 97
# define KEY_D 100
# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_SPACE 32

struct	t_game;

//utils
void	chatting_mode(t_game *game);
int		can_move(t_game *game, float x, float y);
void	use_item(t_game *game);

//keyboard
void	send_update_position(t_game *game);
int		handle_keypress(int keycode, t_game *game);

//mouse
int		handle_mouse_move(int x, int y, t_game *game);
int		handle_mouse_key(int keycode, int x, int y, t_game *game);

#endif