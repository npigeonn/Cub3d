
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

typedef struct s_sprite
{
	float			x;
	float			y;
	float			x1;
	float			y1;
	float			dir_x;
	float			dir_y;
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
	struct s_sprite	*next;
}	t_sprite;

#endif