/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:49:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/19 16:23:14 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# define KEY_W 119
# define KEY_S 115
# define KEY_A 97
# define KEY_D 100

# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

# define KEY_SPACE 32
# define KEY_SHIFT 65505
# define KEY_RIGHT_CLICK 1
# define KEY_ESC 65307
# define KEY_P 112
# define KEY_T 116
# define KEY_F 102

typedef struct	s_key_binding
{
	const char	*action;
	const char	*key1;
	const char	*key2;
}	t_key_binding;

//utils
void	chatting_mode(t_game *game);
int		can_move(char ***map, t_door *door, float x, float y, int floor);
void	use_item(t_game *game);

//keyboard
void	send_update_position(t_game *game);
void	handle_key(t_game *game);
int		handle_key_press(int keycode, t_game *game);
int		handle_key_release(int keycode, t_game *game);

//mouse
int		handle_mouse_move(int x, int y, t_game *game);
int		handle_mouse_key_press(int keycode, int x, int y, t_game *game);
int		handle_mouse_key_release(int keycode, int x, int y, t_game *game);

//key
int		is_key_pressed(t_game *game, int keycode);
void	set_key_flag(t_game *game, int keycode, int is_pressed);

#endif