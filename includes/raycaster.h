/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:04:30 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/14 11:05:01 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTER_H
# define RAYCASTER_H

typedef struct s_floorcast
{
	float	ray_dir_x0;
	float	ray_dir_y0;
	float	ray_dir_x1;
	float	ray_dir_y1;
	float	pos_z;
	float	diff_ray_dir_x;
	float	diff_ray_dir_y;
	float	floor_step_x;
	float	floor_step_y;
	float	floor_x;
	float	floor_y;
	float	row_distance;
	int		f_tex_width;
	int		f_tex_height;
	char	*f_tex_data;
	int		f_bpp;
	int		c_tex_width;
	int		c_tex_height;
	char	*c_tex_data;
	int		c_bpp;
	int		screen_mid;
	float	screen_width_inv;
	float	diff_screen_x;
	float	diff_screen_y;
	int		p;
	int		y;
}	t_floorcast;

typedef struct s_raycast
{
	float		dir_x;
	float		dir_y;
	float		plane_x;
	float		plane_y;
	float		camera_x;
	float		ray_dir_x;
	float		ray_dir_y;
	int			map_x;
	int			map_y;
	float		side_dist_x;
	float		side_dist_y;
	float		delta_dist_x;
	float		delta_dist_y;
	float		perp_wall_dist;
	int			step_x;
	int			step_y;
	int			side;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			x;
	t_floorcast	floorcast;
}	t_raycast;

#endif