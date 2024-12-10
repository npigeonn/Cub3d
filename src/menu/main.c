/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 20:22:05 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/12/10 12:38:31 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/cub3d.h"

void	update_main_menu_click(t_game *game, int mouse_x, int mouse_y,
int keycode)
{
	if (keycode != 1)
		return ;
	if (game->menu->button_selected == 1)
	{
		game->menu->status = PLAYING;
		x_fixes_cursor(game, 'h');
	}
	else if (game->menu->button_selected == 2)
	{
		game->menu->status = SERVERS;
		pthread_create(&game->discover_servers_thread, NULL,
			discover_servers_thread, game);
	}
	else if (game->menu->button_selected == 3)
		game->menu->status = STATS;
	else if (game->menu->button_selected == 4)
		handle_close(game);
	else if (game->menu->button_selected == 5)
	{
		game->menu->status = OPTIONS_KEYBOARD;
		game->menu->last_status = MAIN_MENU;
	}
	else
		return ;
	game->menu->button_selected = 0;
}

void	check_mouse_on_gear(t_game *game, int mouse_x, int mouse_y)
{
	const int	gear_size = game->screen_width * 0.035;
	const int	dx = mouse_x - (game->screen_width - gear_size / 2 - 17);
	const int	dy = mouse_y - (gear_size / 2 + 15);
	const int	distance_squared = dx * dx + dy * dy;

	if (distance_squared >= (gear_size / 2 - 5) * (gear_size / 2 - 5)
		&& distance_squared <= (gear_size / 2 + 5) * (gear_size / 2 + 5))
		game->menu->button_selected = 5;
	else if (distance_squared <= (gear_size / 2) * (gear_size / 2))
		game->menu->button_selected = 5;
}

void	update_main_menu_button(t_game *game, int mouse_x, int mouse_y)
{
	const int	btn_width = game->screen_width * 0.25;
	const int	btn_height = game->screen_height * 0.1;
	const int	spacing = game->screen_height * 0.05;
	const int	y = game->screen_height * 0.22;

	game->menu->button_selected = 0;
	if (mouse_x >= (game->screen_width - btn_width) * 0.5
		&& mouse_x <= (game->screen_width + btn_width) * 0.5)
	{
		if (mouse_y >= y && mouse_y <= y + btn_height)
			game->menu->button_selected = 1;
		else if (mouse_y >= y + btn_height + spacing
			&& mouse_y <= y + btn_height + spacing + btn_height)
			game->menu->button_selected = 2;
		else if (mouse_y >= y + 2 * (btn_height + spacing)
			&& mouse_y <= y + 2 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 3;
		else if (mouse_y >= y + 3 * (btn_height + spacing)
			&& mouse_y <= y + 3 * (btn_height + spacing) + btn_height)
			game->menu->button_selected = 4;
	}
	check_mouse_on_gear(game, mouse_x, mouse_y);
}

static void	draw_selected_button_main(t_game *game)
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

static void	update_line_params(int *error, int *x, int *y, t_line *line)
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

void	draw_gear_icon_loop(t_game *game, t_draw_info info, int i,
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
	const int		outer_radius = size / 2;
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
