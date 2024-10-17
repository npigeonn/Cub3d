#ifndef GAME_H
#define GAME_H

# include <math.h>
# include "mlx.h"
# include "libft.h"
# include <sys/time.h>
# include <netdb.h>

enum Weapon {
	Pistol = 0,
	Shotgun = 1,
};

enum GameStatus
{
	PLAYING = 0,
	PAUSED = 1,
	MAIN_MENU = 2,
	GAME_OVER = 3,
	OPTIONS = 4,
	SERVERS = 5,
	SERVER_CREATE = 6,
	VALID_SERVER_CREATE = 7,
	JOIN_SERVER = 8,
	VALID_JOIN_SERVER = 9,
	CHATING = 10,
	MULTI_PLAYER = 11,
	SERVER_DISCONNECTED = 12,
	SERVER_FULL = 13,
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

typedef struct s_stuff
{
	int munitions;
	int nb_keys;
	int weapon;
	int kit_hp;
}	t_stuff;

typedef struct s_player
{
	float	x;
	float	y;
	float	dirX;
	float	dirY;
	float	planeX;
	float	planeY;
	float	height;
	float	health;
	int		ammo;
	int		floor;
	float	anim_shoot;
	t_stuff	*stuff;
}	t_player;


typedef struct s_image
{
	void	*img;
	char	*data;
	int		bpp;
	int		size_line;
	int		endian;
	int		width;
	int		height;
	int		nb_sprite;
	int		sprite_width;
	int		sprite_height;
}	t_image;

typedef struct s_images
{
	t_image	*alphanum_sprite;
	t_image	*base;
}	t_images;

typedef	struct s_textures
{
	t_image	*east;
	t_image	*west;
	t_image	*sud;
	t_image	*north;
	t_image	*door;
	t_image	*tp;
	t_image	*floor;
	t_image	*ceil;
	t_image	*enemies;
	int		so;
	int		no;
	int		c;
	int		f;
	int		ea;
	int		we;
	int		color_so;
	int		color_no;
	int		color_c;
	int		color_f;
	int		color_ea;
	int		color_we;
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


typedef	struct s_teleporter
{
	float				x1;
	float				y1;
	int					floor1;
	float				x2;
	float				y2;
	int					floor2;
	struct s_teleporter	*next;
}	t_teleporter;

typedef struct s_menu
{
	int		status;
	int		button_selected;
	int		dragging;
	int		message;
	float	volume;
	float	mouse_sensitivity;
	int		server_selected;
	int		text_field_selected;
	bool	error_name;
	bool	error_pseudo;
	int		width_letter[95];
}	t_menu;

enum EnemyState
{
	PATROL = 0,
	CHASE = 1,
};

typedef struct	s_point
{
	float	x;
	float	y;
	int		floor;
} t_point;

typedef struct	s_enemy
{
	float		x;
	float		y;
	float		dirX;
	float		dirY;
	int			floor;
	float		health;
	int			state;
	int			direction;
	int			frame_count;
	float		fov;
	struct s_enemy	*next;
}	t_enemy;

typedef struct s_server_info
{
	char					*name;
	char					*ip;
	int						port;
	int						players;
	int						ping;
	int						max_players;
	time_t					last_seen;
	struct s_server_info	*next;
}	t_server_info;

typedef struct	s_player_info
{
	int						player_id;
	char					pseudo[32];
	float					x;
	float					y;
	float					dir_x;
	float					dir_y;
	float					height;
	float					health;
	int						floor;
	struct s_player_info	*next;
}	t_player_info;

typedef struct s_server
{
	int				player_id;
	char			pseudo[20];
	int				sock;
	int				sock_bc;
	char			*ip;
	char			name[20];
	int				nb_player;
	t_player_info	*players;
}	t_server;

#define	MAX_MESSAGE_LENGTH 256

typedef struct	S_message
{
	char				pseudo[20];
	int					color;
    char				message[MAX_MESSAGE_LENGTH];
    struct timeval		time;
    struct s_message	*next;
} t_message;

typedef struct	s_chatbox
{
	bool		visible;
	bool		is_writting;
	char		message[MAX_MESSAGE_LENGTH];
	t_message	*messages;
}	t_chatbox;

typedef struct s_game
{
	t_menu			*menu;
	t_memory_table	*mem;
	void			*mlx;
	void			*win;
	int				screen_width;
	int				screen_height;
	int				map_begin;
	char			***map;
	char 			***map_cy;
	int				x_minimap;
	int				y_minimap;
	int				x_hp;
	int				y_hp;
	int				nb_floor;
	int				clr;
	t_door			*door;
	t_images		*images;
	t_player		*player;
	t_textures		*textures;
	float			*wall_distances;
	t_teleporter	*tp;
	t_chatbox		*chatbox;
	t_enemy			*enemies;
	float			delta_time;
	struct timeval	last_time;


	t_server		*server;
	t_server_info	*servers;
	pthread_t		discover_servers_thread;
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
void	secure_pixel_put(t_game *game, int x, int y, int color);
void	draw_rectangle(t_game *game, int x, int y, int width, int height, int color);
void	draw_text(t_game *data, char *str, int x, int y, int height, int color);
void	draw_text_left(t_game *game, char *str, int x, int y, int height, int color);
void	draw_text_right(t_game *game, char *str, int x, int y, int height, int color);
void	draw_char(t_game *data, int x, int y, int height, char c, int color);
void	draw_sprite(t_game *game, t_image *texture, float x, float y, float sprite_dir);
void	draw_sprites(t_game *game);
void	draw_rounded_rectangle(t_game *game, int x, int y, int width, int height, int radius, int color);
void	crosshair(t_game *game);
int 	blend_colors(int bg_color, int fg_color, float alpha);
int 	get_pixel_color_from_image(t_game *game, int x, int y);
void	draw_image(t_game *game, int x, int y, int height, t_image *img);
void	gun_draw(t_game *game);
void	get_pos_char(char c, int *x, int *y);
void	ammo_written(t_game *game);
void	health_point_draw(t_game *game);

// mini map
void	mini_map(t_game *game);
void	print_wall_door_player(t_game *game);

//door
int		handle_door(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side, float distance);
void	add_door(t_game *game, int x, int y, int floor, bool lock);
t_door	*get_door(t_game *game, int x, int y, int floor);
void	use_door_in_view(t_game *game);
void	update_door_animation(t_game *game);
	
//wall
void	draw_wall(t_game *game, int x, int map_x, int map_y, int step_x, int step_y, float ray_dir_x, float ray_dir_y, int side);
void	draw_vertical_line_with_texture(t_game *game, int x, int draw_start, int draw_end, t_image *texture, float wall_x, int line_height);

//enemies
void	draw_enemies(t_game *game);
void	init_enemies(t_game *game);
void	update_enemies(t_game *game);

// PARSING
int		err(char *str);
void	init_data(t_game *game);
void	parsing(char **av, t_game *game);
char	*get_next_line(int fd);
void	floodfill(t_game *game);
int		check_path(t_game *game, int x, int y, int floor);
void	door_mngmt(t_game *game);

// walls
int		check_walls(t_game *game, int x, int y, int floor);

// key looked doors
void	compare_key_n_looked_door(t_game *game);

//teleporter
int		is_a_teleporter(char c);
void	teleportation_check(t_game *game);
void	use_teleporter(t_game *game);
void	set_output_teleporter(t_game *game, int x, int y, int floor);
void	add_teleporter(t_game *game, int x, int y, int floor);
int		teleportation(t_game *game, int x, int y, int floor, char c);

// map
int		map_copy(t_game *game);
char	*switch_line(char *line, int fd);
void	map_set_up(char **av, t_game *game);

// texture check
int		textures(char *to_open, t_game *game);
void	load_texture(t_game *game, t_image *img, char *path);
void	load_txtre_globale(t_game *game, char *line, char *path);

// free
int	free_map(t_game *game);
int free_map_copy(t_game *game);
int	free_split(char **str);


//server
void	create_server(t_game *game);

//client
int		join_server(t_game *game);
void	draw_players(t_game *game);



#endif