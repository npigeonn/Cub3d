/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 23:32:56 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/14 08:29:20 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/cub3d.h"

void	update_main_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	y = game->screen_height * 0.25;

	if (mouse_x >= (game->screen_width - btn_width) * 0.5 && mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y && mouse_y <= y + btn_height)
			game->menu->button_selected = 1;
		else if (mouse_y >= y + btn_height + spacing && mouse_y <= y + btn_height + spacing + btn_height)
			game->menu->button_selected = 2;
		else if (mouse_y >= y + 2 * (btn_height + spacing) && mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 3;
		else if (mouse_y >= y + 3 * (btn_height + spacing) && mouse_y <= y + 3 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 4;
		else 
			game->menu->button_selected = 0;
	}
	else
		game->menu->button_selected = 0;
}

void	draw_main_menu(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;

	if (game->menu->button_selected == 1)
		draw_rectangle(game, x - 2, y - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, x, y, btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Solo", x + btn_width * 0.5, y + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);

	if (game->menu->button_selected == 2)
		draw_rectangle(game, x - 2, y + btn_height + spacing - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, x, y + btn_height + spacing, btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Multi", x + btn_width * 0.5, y + btn_height + spacing + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);

	if (game->menu->button_selected == 3)
		draw_rectangle(game, x - 2, y + 2 * (btn_height + spacing) - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, x, y + 2 * (btn_height + spacing), btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Options", x + btn_width * 0.5, y + 2 * (btn_height + spacing) + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);

	if (game->menu->button_selected == 4)
		draw_rectangle(game, x - 2, y + 3 * (btn_height + spacing) - 2, btn_width + 4, btn_height + 4, MENU_BUTTON_SELECTED_COLOR);
	draw_rectangle(game, x, y + 3 * (btn_height + spacing), btn_width, btn_height, MENU_BUTTON_COLOR);
	draw_text(game, "Exit", x + btn_width * 0.5, y + 3 * (btn_height + spacing) + btn_height * 0.33 - 5, btn_height * 0.5, MENU_BUTTON_TEXT_COLOR);
}
