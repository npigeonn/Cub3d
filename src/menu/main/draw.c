/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:49:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/02 18:50:46 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_line_params(int *error, int *x, int *y, t_line *line)
{
	if (*error > -line->delta_x)
	{
		*error -= line->delta_y;
		*x += line->step_x;
	}
	if (*error < line->delta_y)
	{
		*error += line->delta_x;
		*y += line->step_y;
	}
}

void	draw_line(t_game *game, t_line line, int color)
{
	int		error;

	line.delta_x = abs(line.x2 - line.x1);
	line.delta_y = abs(line.y2 - line.y1);
	if (line.x1 < line.x2)
		line.step_x = 1;
	else
		line.step_x = -1;
	if (line.y1 < line.y2)
		line.step_y = 1;
	else
		line.step_y = -1;
	if (line.delta_x > line.delta_y)
		error = line.delta_x / 2;
	else
		error = -line.delta_y / 2;
	while (1)
	{
		pixel_put(game, line.x1, line.y1, color);
		if (line.x1 == line.x2 && line.y1 == line.y2)
			break ;
		update_line_params(&error, &line.x1, &line.y1, &line);
	}
}

void	draw_circle(t_game *game, t_draw_info info)
{
	int		x;
	int		y;
	double	angle;

	angle = 0;
	while (angle < 2 * M_PI)
	{
		x = info.x + (int)(info.radius * cos(angle) + 0.5);
		y = info.y + (int)(info.radius * sin(angle) + 0.5);
		pixel_put(game, x, y, info.color);
		angle += 0.01;
	}
}

t_line	init_line(int x1, int y1, int x2, int y2)
{
	t_line	line;

	line.x1 = x1;
	line.y1 = y1;
	line.x2 = x2;
	line.y2 = y2;
	return (line);
}

void	draw_selected_button_main(t_game *game)
{
	t_draw_info	info;
	const int	btn_width = game->screen_width * 0.25 + 4;
	const int	btn_height = game->screen_height * 0.1 + 4;
	const int	spacing = game->screen_height * 0.05;

	info = init_draw_info(0, "", (game->screen_width - btn_width) * 0.5 - 2,
			game->screen_height * 0.22 - 4);
	info.width = btn_width + 4;
	info.height = btn_height + 4;
	info.color = MENU_BUTTON_SELECTED_COLOR;
	info.radius = 10;
	if (game->menu->button_selected == 1)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + spacing - 4;
	if (game->menu->button_selected == 2)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + spacing - 4;
	if (game->menu->button_selected == 3)
		draw_rounded_rectangle(game, info);
	info.y += btn_height + spacing - 4;
	if (game->menu->button_selected == 4)
		draw_rounded_rectangle(game, info);
}
