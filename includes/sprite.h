/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:06:06 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/15 11:39:14 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_H
# define SPRITE_H

# include <stdbool.h>

enum e_sprite_type
{
	SPRITE_ENEMY,
	SPRITE_AMMO,
	SPRITE_HEALTH,
	SPRITE_FIRE,
	SPRITE_PLAYER,
	SPRITE_EXIT,
	SPRITE_TELEPORTER
};

typedef	struct s_spritecast
{
	float			r_x;
	float			r_y;
	float			inv_det;
	float			transform_x;
	float			transform_y;
	float			player_angle;
	float			relative_angle;
	int				tex_x;
	int				tex_x_base;
	int				screen_x;
	int				height;
	int				width;
	int				draw_start_y;
	int				draw_end_y;
	int				stripe_start;
	int				stripe_end;
	float			step;
	float			tex_pos;
	int				bpp;
	char			*texture_data;
	int				anim_frame;
	float			dx;
	float			dy;
	float			angle_in_radians;
	float			distance;
	float			distance_squared;
	struct s_image	*texture;
}	t_spritecast;

typedef struct s_sprite
{ 
	float			x;
	float			y;
	float			x1;
	float			y1;
	float			dir_x;
	float			dir_y;
	float			plane_x;
	float			plane_y;
	int				floor;
	int				floor1;
	float			health;
	int				state;
	int				direction;
	int				frame_count;
	float			fov;
	float			shoot_delay;
	int				type;
	bool			still_exist;
	float			animation;
	int				selected_anim;
	int				player_id;
	int				ammo;
	char			pseudo[32];
	float			dir;
	float			scale;
	float			z_offset;
	t_spritecast	*spritecast;
	struct s_sprite	*next;
}	t_sprite;

#endif