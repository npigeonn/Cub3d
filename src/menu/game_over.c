/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_over.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:35:41 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/25 17:11:51 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_game_over_click(t_game *game, int mouse_x, int mouse_y, int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
		reset_game(game);
	game->menu->button_selected = 0;
	game->menu->text_field_selected = 0;
}

void	update_game_over_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	y = game->screen_height * 0.25;

	game->menu->button_selected = 0;
	if (mouse_x >= (game->screen_width - btn_width) * 0.5 && mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y + 2 * (btn_height + spacing) && mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 1;
	}
}

void	apply_fade_to(t_game *game, int color)
{
	int	x;
	int	y;
	int	clr;
	int	new_color;
	const float alpha = game->fade_progress;

	y = 0;
	while (y < game->screen_height)
	{
		x = 0;
		while (x < game->screen_width)
		{
			clr = get_pixel_color_from_image(game, x, y);
			new_color = blend_colors(clr, color, alpha); // color
			pixel_put(game, x, y, new_color);
			x++;
		}
		y++;
	}
	game->fade_progress += 0.005;
}


void	draw_game_over(t_game *game)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.25;
	t_draw_info	info2;
	t_draw_info	info;

	info = init_draw_info(game->screen_height, "", 0, 0);
	info.width = game->screen_width;
	info.color = 0x850606;
	draw_rectangle(game, info);
	mlx_mouse_show(game->mlx, game->win);
	info = init_draw_info(70, "You are dead.", game->screen_width >> 1, game->screen_height * 0.4);
	info.color = MENU_BUTTON_TEXT_COLOR;
	draw_text(game, info);
	info2 = init_draw_info(0, "", x - 4, y + 2 * (btn_height + spacing) - 4);
	info2.width = btn_width + 8;
	info2.height = btn_height + 8;
	info2.color = MENU_BUTTON_SELECTED_COLOR;
	info2.radius = 10;
	if (game->menu->button_selected == 1)
		draw_rounded_rectangle(game, info2);
	info = init_draw_info(0, "", x, y + 2 * (btn_height + spacing));
	info2 = init_draw_info(btn_height, "", x, y + 2 * (btn_height + spacing));
	info2.color = MENU_BUTTON_COLOR;
	info2.width = btn_width;
	draw_rectangle(game, info2);
	info.color = MENU_BUTTON_TEXT_COLOR;
	ft_strcpy(info.str, "Main menu");
	info.y = y + 2 * (btn_height + spacing) + btn_height * 0.33 - 5;
	info.height = btn_height * 0.5;
	info.x = x + btn_width / 2;
	draw_text(game, info);
	game->chatbox->visible = false;
	game->chatbox->is_writting = false;
}
