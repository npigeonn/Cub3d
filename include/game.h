#ifndef GAME_H
#define GAME_H

# include <math.h>

# include "../minilibx-linux/mlx.h"
# include "../minilibx-linux/mlx_int.h"
# include "../Libft/includes/libft.h"
# include "../Libft/includes/gc.h"
# include "../Libft/includes/ft_printf.h"

# define WIDTH 800
# define HEIGHT 800

enum Weapon {
	Pistol = 0,
	Shotgun = 1,
};

enum Direction {
	SIDE_EAST,
	SIDE_WEST,
	SIDE_NORTH,
	SIDE_SOUTH,
	SIDE_FLOOR,
	SIDE_CEILING,
};

typedef struct s_player
{
	float	x;
	float	y;
	float	dirX;
	float	dirY;
	float	planeX;
	float	planeY;
	float	speed;
	int		armo;
	float	height;
	float	health;
	int		weapon;
	int		floor;
	float	pitch;
}	t_player;

#define TILE_SIZE 50
// #define MAP_WIDTH 640
// #define MAP_HEIGHT 480

typedef struct {
	void *img;
	int width;
	int height;
} t_texture;

typedef struct s_image
{
	void	*img;
	char	*data;
	int		bpp;
	int		size_line;
	int		endian;
} t_image;

typedef struct s_game
{
	void *mlx;
	void *win;
	t_image		*img;
	t_player	*player;
	t_texture wall_texture;
	t_texture floor_texture;
	t_texture player_texture;
} t_game;

// typedef struct s_data
// {
// 	void	*mlx;
// 	void	*window;
// 	void	*img;
// 	char	*addr;
// 	int		bits_per_pixel;
// 	int		line_length;
// 	int		endian;
// 	int		fct;
// 	int		color;
// 	double	x_offset;
// 	double	y_offset;
// 	double	zoom;
// 	int		xx;
// 	int		yy;
// }	t_data;

// void raycasting(t_game *data);

#endif