
#ifndef GAME_H
#define GAME_H

# include <math.h>

enum Weapon {
	Pistol = 0,
	Shotgun = 1,
};

typedef struct s_player
{
	float	x;
	float	y;
	float	dx;
	float	dy;
	float	px;
	float	py;
	float	speed;
	int		armo;
	float	health;
	int		weapon;
}	t_player;

#endif