#ifndef GAME_H
#define GAME_H

# include <math.h>

# include "mlx.h"
# include "libft.h"
#include <sys/time.h>

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

enum Message {
	NOTHING,
	OPEN_DOOR,
	CLOSE_DOOR,
	TELEPORT,
	FINISH,
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


typedef struct s_map
{
	int		map_width;
	int		map_height;
	char	***map;
}	t_map;

typedef	struct s_textures
{
	t_image	*zekrom;
	t_image	*crefadet;
	t_image	*mewtwo;
	t_image	*artikodin;
	t_image	*door;
}	t_textures;

typedef	struct s_door
{
	int				x;
	int				y;
	int				floor;
	bool			open;
	bool			lock;
	float			animation;
	struct s_door	*next;
}	t_door;


typedef struct s_game
{
	t_memory_table	*mem;
	void		*mlx;
	void		*win;
	int			status;
	int			button_selected;
	int			screen_width;
	int			screen_height;
	float		volume;
	float		mouse_sensitivity;
	char		***map;
	char 		***map_cy;
	int			nb_floor;
	t_door		*door;
	t_images	*images;
	t_player	*player;
	t_textures	*textures;

	float		delta_time;
	struct timeval	last_time;
	int			message;
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
void	update_main_menu_button(t_game *game, int mouse_x, int mouse_y);
void	update_option_menu_slider(t_game *game, int mouse_x, int mouse_y, int keycode);
void	update_option_menu_button(t_game *game, int mouse_x, int mouse_y);
void	draw_options_menu(t_game *game);
void	update_multiplayer_menu(t_game *game, int mouse_x, int mouse_y);
void	draw_multiplayer_menu(t_game *game);

//draw
void	pixel_put(t_game *game, int x, int y, int color);
void	draw_rectangle(t_game *game, int x, int y, int width, int height, int color);
void	draw_text(t_game *data, char *str, int x, int y, int height, int color);
void	draw_text_left(t_game *game, char *str, int x, int y, int height, int color);
void	draw_text_right(t_game *game, char *str, int x, int y, int height, int color);
void	draw_char(t_game *data, int x, int y, int height, char c, int color);

//door
int		handle_door(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side, float distance);
void	add_door(t_game *game, int x, int y, int floor, bool lock);
t_door	*get_door(t_game *game, int x, int y);
void	use_door_in_view(t_game *game);
	
//wall
void	draw_wall(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side);
void	draw_vertical_line_with_texture(t_game *game, int x, int draw_start, int draw_end, t_image *texture, float wall_x, int line_height);
void	draw_floor(t_game *game);
void	draw_ceilling(t_game *game);

int		err(char *str);
void	parsing(char **av, t_game *game);
char	*get_next_line(int fd);
void	floodfill(t_game *game);
int		map_copy(t_game *game);
int 	free_map_copy(t_game *game);

//teleporter
int	is_a_teleporter(char c);

#endif