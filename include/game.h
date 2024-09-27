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
}    t_player;

#define TILE_SIZE 50
#define MAP_WIDTH 640
#define MAP_HEIGHT 480
typedef struct {
    void *img;
    int width;
    int height;
} t_texture;

typedef struct {
    void *mlx;
    void *win;
	t_player	*player;
    t_texture wall_texture;
    t_texture floor_texture;
    t_texture player_texture;
    char map[MAP_HEIGHT][MAP_WIDTH + 1];
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

void raycast(t_player *player, void *mlx_ptr, void *win_ptr);

#endif