/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:24:35 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/07 18:00:24 by ybeaucou         ###   ########.fr       */
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

//CREATE SERVER
char			*get_local_ip(t_game *game);
char			*get_field(t_game *game);

//OPTIONS
t_key_binding	*get_binding(t_game *game);
void			draw_slider(t_game *game, int x, int y, float value);
void			draw_sound_slider(t_game *game, int i, float slider_x, float slider_y);


#endif