/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:28:17 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 12:28:17 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_success_background(t_game *game)
{
	t_draw_info	info;

	info = init_draw_info(game->screen_height, "", 0, 0);
	info.width = game->screen_width;
	info.color = 3289650;
	draw_rectangle(game, info);
	x_fixes_cursor(game, 's');
	info = init_draw_info(70, "MISSION SUCCEED",
			game->screen_width >> 1, game->screen_height * 0.4);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
}

void	draw_main_menu_button(t_game *game, int x, int y)
{
	t_draw_info	info;
	t_draw_info	info2;
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;

	info2 = init_draw_info(btn_height, "", x, y);
	info2.color = MENU_BUTTON_COLOR;
	info2.width = btn_width;
	draw_rectangle(game, info2);
	info = init_draw_info(0, "", x, y);
	info.color = MENU_BUTTON_TEXT_COLOR;
	ft_strcpy(info.str, "Main menu");
	info.y += btn_height * 0.33 - 5;
	info.height = btn_height * 0.5;
	info.x += btn_width / 2;
	draw_text(game, info);
}

void	draw_selected_button(t_game *game, int x, int y)
{
	t_draw_info	info;
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;

	info = init_draw_info(0, "", x - 4, y - 4);
	info.width = btn_width + 8;
	info.height = btn_height + 8;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	info.radius = 10;
	draw_rounded_rectangle(game, info);
}

void	draw_success(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25 + 2 * (btn_height + spacing);

	draw_success_background(game);
	if (game->menu->button_selected == 1)
		draw_selected_button(game, x, y);
	draw_main_menu_button(game, x, y);
	game->chatbox->visible = false;
	game->chatbox->is_writting = false;
}

void	victory_screen(t_game *game)
{
	t_player	*p;

	p = game->player;
	if (p->health <= 0 && game->menu->status != CHATING
		&& game->menu->status != MULTI_PLAYER)
		game->menu->status = GAME_OVER;
	if (game->map[p->floor][(int)p->y][(int)p->x] == 'e')
		game->menu->status = GAME_SUCCESS;
}
