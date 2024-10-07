#ifndef GAME_H
#define GAME_H

# include <math.h>

# include "mlx.h"
# include "libft.h"

enum Weapon {
	Pistol = 0,
	Shotgun = 1,
};

enum GameStatus {
	PLAYING = 0,
	PAUSED = 1,
	MAIN_MENU = 2,
	GAME_OVER = 3,
	OPTIONS = 4,
	SERVEURS = 5,
	SERVEUR_CREATE = 6,
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

typedef struct {
	void *img;
	int width;
	int height;
}	t_texture;

typedef struct s_image
{
	void	*img;
	char	*data;
	int		bpp;
	int		size_line;
	int		endian;
	int		width;
	int		height;
}	t_image;

typedef struct s_images
{
	t_image	*alphanum_sprite;
	t_image	*base;
}	t_images;

typedef struct s_game
{
	t_memory_table	*mem;
	void		*mlx;
	void		*win;
	char		***map;
	int			status;
	int			button_selected;
	int			map_width;
	int			map_height;
	int			screen_width;
	int			screen_height;
	float		volume;
	float		mouse_sensitivity;
	t_images	*images;
	t_player	*player;
	t_texture	wall_texture;
	t_texture	floor_texture;
	t_texture	player_texture;

	int			dragging;
}	t_game;


//game
int		game_loop(t_game *game);
int		handle_keypress(int keycode, t_game *game);
int		handle_mouse_move(int x, int y, t_game *game);
int		handle_close(t_game *game);
void	set_direction(t_game *game, int dir);
int		handle_mouse_key(int keycode, int x, int y, t_game *game);

//menu
void	draw_main_menu(t_game *game);

//draw
void	pixel_put(t_game *game, int x, int y, int color);
void	draw_rectangle(t_game *game, int x, int y, int width, int height, int color);
void	draw_text(t_game *data, char *str, int x, int y, int height, int color);
void	draw_char(t_game *data, int x, int y, int height, char c, t_image *img, int color);

int		err(char *str);
void	parsing(char **av, t_game *game);
char	*get_next_line(int fd);
void	floodfill(t_game *game);

#endif