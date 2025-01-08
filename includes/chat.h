/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chat.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:10:02 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 16:05:02 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAT_H
# define CHAT_H

t_draw_info	init_draw_info(int height, char *str, int x, int y);
void		add_message(t_game *game);
void		draw_wrapped_text_left(t_game *game, t_draw_info info);
void		draw_text_max_right(t_game *game, t_draw_info info);
void		chat_input(t_game *game, int keycode);
void		handle_mouse_chat(t_game *game, int x, int y, int keycode);
void		show_message(t_game *game);
void		chat_draw(t_game *game);
void		draw_transparent_rectangle(t_game *game, t_draw_info info);

#endif