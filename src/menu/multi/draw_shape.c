/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_shape.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:40:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/03 17:40:19 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_arc(t_game *game, float start_angle, float end_angle,
t_draw_info info)
{
	float	angle;
	int		x;
	int		y;

	angle = start_angle;
	while (angle <= end_angle)
	{
		x = info.x + info.radius * cos(angle);
		y = info.y + info.radius * sin(angle);
		pixel_put(game, x, y, info.color);
		angle += 0.01;
	}
}

void	draw_all_arc(t_game *game, t_draw_info info)
{
	t_draw_info	info2;

	info2 = init_draw_info(0, "", info.x + info.radius, info.y + info.radius);
	info2.radius = info.radius;
	info2.color = info.color;
	draw_arc(game, M_PI, M_PI * 1.5, info2);
	info2 = init_draw_info(0, "", info.x + info.width - info.radius,
			info.y + info.radius);
	info2.radius = info.radius;
	info2.color = info.color;
	draw_arc(game, M_PI * 1.5, 0, info2);
	info2 = init_draw_info(0, "", info.x + info.width - info.radius,
			info.y + info.height - info.radius);
	info2.radius = info.radius;
	info2.color = info.color;
	draw_arc(game, 0, M_PI * 0.5, info2);
	info2 = init_draw_info(0, "", info.x + info.radius,
			info.y + info.height - info.radius);
	info2.radius = info.radius;
	info2.color = info.color;
	draw_arc(game, M_PI * 0.5, M_PI, info2);
}

void	draw_full_rounded_rectangle(t_game *game, t_draw_info info)
{
	int			i;
	int			j;
	const int	radius_squared = info.radius * info.radius;

	i = -1;
	while (++i < info.radius)
	{
		j = -1;
		while (++j < info.radius)
		{
			if (!(i * i + j * j <= radius_squared))
				continue ;
			pixel_put(game, info.x + info.radius - i, info.y + info.radius - j,
				info.color);
			pixel_put(game, info.x + info.width - info.radius + i,
				info.y + info.radius - j, info.color);
			pixel_put(game, info.x + info.width - info.radius + i,
				info.y + info.height - info.radius + j, info.color);
			pixel_put(game, info.x + info.radius - i,
				info.y + info.height - info.radius + j, info.color);
		}
	}
}

void	draw_rounded_rectangle(t_game *game, t_draw_info info)
{
	t_draw_info	info2;

	if (info.radius > info.width * 0.5)
		info.radius = info.width * 0.5;
	if (info.radius > info.height * 0.5)
		info.radius = info.height * 0.5;
	info2 = init_draw_info(0, "", info.x + info.radius, info.y);
	info2.color = info.color;
	info2.width = info.width - 2 * info.radius;
	info2.height = info.height;
	draw_rectangle(game, info2);
	info2 = init_draw_info(0, "", info.x, info.y + info.radius);
	info2.height = info.height - 2 * info.radius;
	info2.width = info.width;
	info2.color = info.color;
	draw_rectangle(game, info2);
	draw_all_arc(game, info);
	draw_full_rounded_rectangle(game, info);
}
