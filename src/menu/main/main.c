/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 20:22:05 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 15:21:29 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/cub3d.h"

static void	draw_gear_icon_loop(t_game *game, t_draw_info info, int i,
const double angle_step)
{
	const double	angle = i * angle_step;
	const double	next_angle = angle + angle_step;
	int				coords[8];
	t_line			line;

	coords[0] = info.x + (int)(cos(angle) * (info.radius + info.width) + 0.5);
	coords[1] = info.y + (int)(sin(angle) * (info.radius + info.width) + 0.5);
	coords[2] = info.x + (int)(cos(next_angle)
			* (info.radius + info.width) + 0.5);
	coords[3] = info.y + (int)(sin(next_angle)
			* (info.radius + info.width) + 0.5);
	coords[4] = info.x + (int)(cos(angle) * info.radius + 0.5);
	coords[5] = info.y + (int)(sin(angle) * info.radius + 0.5);
	coords[6] = info.x + (int)(cos(next_angle) * info.radius + 0.5);
	coords[7] = info.y + (int)(sin(next_angle) * info.radius + 0.5);
	line = init_line(coords[4], coords[5], coords[0], coords[1]);
	draw_line(game, line, info.color);
	line = init_line(coords[0], coords[1], coords[2], coords[3]);
	draw_line(game, line, info.color);
	line = init_line(coords[2], coords[3], coords[6], coords[7]);
	draw_line(game, line, info.color);
	line = init_line(coords[6], coords[7], coords[4], coords[5]);
	draw_line(game, line, info.color);
}

void	draw_gear_icon(t_game *game, int x, int y, int size)
{
	t_draw_info		info;
	const double	angle_step = M_PI * 2 / 8;
	const int		tooth_length = size / 8;
	int				i;

	info = init_draw_info(size, "", x + size / 2, y + size / 2);
	info.radius = size / 2;
	info.width = tooth_length;
	if (game->menu->button_selected == 5)
		info.color = 0xFF0000;
	else
		info.color = 0xFFFFFF;
	i = -1;
	while (++i < 8)
		draw_gear_icon_loop(game, info, i, angle_step);
	info.radius = size / 3;
	draw_circle(game, info);
}

void	draw_title_main_menu(t_game *game, t_draw_info *info,
t_draw_info *info2)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	x = (game->screen_width - btn_width) * 0.5;
	const int	y = game->screen_height * 0.22;

	draw_selected_button_main(game);
	*info = init_draw_info(btn_height * 0.5, "Solo", x + btn_width * 0.5,
			y + btn_height * 0.33 - 5);
	info->color = MENU_BUTTON_TEXT_COLOR;
	*info2 = init_draw_info(0, "", x, y);
	info2->color = MENU_BUTTON_COLOR;
	info2->height = btn_height;
	info2->width = btn_width;
	draw_rectangle(game, *info2);
	draw_text(game, *info);
}

void	draw_main_menu(t_game *game)
{
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	t_draw_info	info2;
	t_draw_info	info;

	draw_title_main_menu(game, &info, &info2);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Multi");
	info.y += btn_height + spacing;
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Stats");
	info.y += btn_height + spacing;
	draw_text(game, info);
	info2.y += btn_height + spacing;
	draw_rectangle(game, info2);
	ft_strcpy(info.str, "Exit");
	info.y += btn_height + spacing;
	draw_text(game, info);
	draw_gear_icon(game, game->screen_width - game->screen_width * 0.035 - 17,
		15, game->screen_width * 0.035);
}
