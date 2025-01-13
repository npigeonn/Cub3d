/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:54:51 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/13 11:57:33 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include <math.h>
# include "libft.h"
# include <sys/time.h>
# include <netdb.h>
# include <pthread.h>
# include "raudio.h"

typedef struct s_server t_server;

typedef struct s_projectile
{
	float				x;
	float				y;
	float				dir_x;
	float				dir_y;
	float				speed;
	float				damage;
	float				direction;
	int					floor;
	struct s_player		*owner;
	struct s_sprite		*enemy;
	struct s_projectile	*next;
}	t_projectile;

typedef struct s_draw_info
{
	int		width;
	int		height;
	char	str[256];
	char	c;
	int		color;
	int		x;
	int		y;
	int		radius;
	int		max_width;
	float	alpha;
	int		padding;
}	t_draw_info;

enum e_weapon
{
	Pistol = 0,
	Shotgun = 1,
};

enum e_game_status
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
	GET_PSEUDO = 14,
	STATS = 15,
	GAME_SUCCESS = 16,
	OPTIONS_KEYBOARD = 17,
	OPTIONS_MOUSE = 18,
	OPTIONS_SOUND = 19,
	RED = 0x850606,
	WHITE = 16777215,
};

enum e_direction
{
	SIDE_EAST,
	SIDE_WEST,
	SIDE_NORTH,
	SIDE_SOUTH,
	SIDE_FLOOR,
	SIDE_CEILING,
};

enum e_message
{
	NOTHING,
	OPEN_DOOR,
	CLOSE_DOOR,
	TELEPORT,
	FINISH,
};

typedef struct s_stats
{
	int				nb_kills;
	int				nb_degats;
	struct timeval	temps;
	int				nb_games;
	int				nb_win;
	int				nb_shoot;
	int				nb_hit;
	float			distanc_travel;
}	t_stats;

typedef struct s_keycode
{
	int	up;
	int	up2;
	int	down;
	int	down2;
	int	left;
	int	left2;
	int	right;
	int	right2;
	int	use;
	int	use2;
	int	escape;
	int	escape2;
	int	pause;
	int	pause2;
	int	chat;
	int	chat2;
}	t_keycode;

typedef struct s_player
{
	float		x;
	float		y;
	float		dir_x;
	float		dir_y;
	float		plane_x;
	float		plane_y;
	float		height;
	float		health;
	int			life_up;
	int			injured;
	int			ammo;
	int			floor;
	float		anim_shoot;
	int			is_shooting;
	int			picking_up_ammo;
	int			begin_dir;
	bool		being_tpted;
	int			telep_signal;
	float		x_tel;
	float		y_tel;
	float		f_tel;
	bool		invert_mouse_x;
	int			selected_anim;
	float		animation;
	t_raycast	*raycast;
	t_stats		*stats;
	t_keycode	*key;
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
	int		selected_anim;
}	t_image;

typedef struct s_images
{
	t_image	*alphanum_sprite;
	t_image	*base;
}	t_images;

typedef struct s_textures
{
	t_image	*east;
	t_image	*west;
	t_image	*south;
	t_image	*north;
	t_image	*door;
	t_image	*tp;
	t_image	*floor;
	t_image	*ceil;
	t_image	*enemy;
	t_image	*enemy_fire;
	t_image	*enemy_death;
	t_image	*bullet;
	t_image	*weapon;
	t_image	*fire;
	t_image	*ammo;
	t_image	*health;
	t_image	*exit;
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

typedef struct s_sound
{
	Sound			gunshot;
	Sound			ammo;
	Sound			pain;
	Sound			life;
	Sound			telep;
	struct s_sound	*next;
}	t_sound;

typedef struct s_door
{
	int				x;
	int				y;
	int				floor;
	bool			open;
	float			animation;
	struct s_door	*next;
}	t_door;

typedef struct s_menu
{
	int		status;
	int		last_status;
	int		button_selected;
	int		dragging;
	int		message;
	float	music_volume;
	float	effects_volume;
	float	menu_music_volume;
	float	mouse_sensitivity;
	int		server_selected;
	int		text_field_selected;
	bool	error_name;
	bool	error_pseudo;
	int		width_letter[95];
	int		scroll;
	int		nb_scroll;
	int		scroll_height;
}	t_menu;

enum e_enemy_state
{
	PATROL = 0,
	CHASE = 1,
};

typedef struct s_point
{
	float	x;
	float	y;
	int		floor;
}	t_point;

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

typedef struct s_client
{
	int			sock;
	char		pseudo[20];
	char		name[20];
	char		ip[16];
	int			player_id;
	t_sprite	*sprites;
	pthread_t	thread;
}	t_client;

# define MAX_MESSAGE_LENGTH 256

typedef struct s_message
{
	char				pseudo[20];
	int					color;
	char				message[MAX_MESSAGE_LENGTH];
	struct timeval		time;
	struct s_message	*next;
}	t_message;

typedef struct s_chatbox
{
	bool		visible;
	bool		is_writting;
	char		message[MAX_MESSAGE_LENGTH];
	t_message	*messages;
}	t_chatbox;

typedef struct s_game
{
	t_menu					*menu;
	t_memory_table			*mem;
	t_sprite				*sprites;
	float					time_regen;
	void					*mlx;
	void					*win;
	int						screen_width;
	int						screen_height;
	int						map_begin;
	char					***map;
	char					***map_cy;
	bool					mouse;
	bool					check_w;
	int						x_minimap;
	int						y_minimap;
	int						clr;
	int						music_dif;
	int						arg_map;
	pthread_t				thread;
	int						nb_floor;
	t_door					*door;
	t_images				*images;
	t_player				*player;
	t_textures				*textures;
	float					*wall_distances;
	t_chatbox				*chatbox;
	float					delta_time;
	struct timeval			last_time;
	int						key_flags;
	t_projectile			*projectiles;
	t_client				*client;
	t_server_info			*servers;
	t_server				*server;
	pthread_t				discover_servers_thread;
	pthread_mutex_t			game_lock;
	float					fade_progress;
	char					**av;
	float					cen_x;
	float					cen_y;
	float					half_cen_x;
	float					half_cen_y;
	struct s_thread_pool	*pool;
	bool					is_running;
	int						bytes_received;
	int						sockfd;
	pthread_mutex_t			mutex;
	pthread_mutex_t			mutex_music;
}	t_game;

//game
int			game_loop(t_game *game);
int			handle_close(t_game *game);
void		set_direction(t_game *game, int dir);
void		set_direction2(t_game *game, int dir);
void		x_fixes_cursor(t_game *game, char to_do);
void		hooks(t_game *game);
void		show_menu_message(t_game *game);
void		reset_game(t_game *game);
void		cast_rays(t_game *game, t_raycast *r);
void		cast_floor(t_game *game, t_floorcast *floorcast);
void		free_last_game_texture(t_game *game);

//menu
void		draw_main_menu(t_game *game);
void		update_main_menu_button(t_game *game, int mouse_x, int mouse_y);
void		update_option_menu_button(t_game *game, int mouse_x, int mouse_y);
void		draw_options_menu(t_game *game);
void		update_multiplayer_menu(t_game *game, int mouse_x, int mouse_y);
void		draw_multiplayer_menu(t_game *game);
char		*get_key_name(t_game *game, int keycode);

//text
int			get_index_char(char c);
int			get_char_width_return(int left_bound, int right_bound);
int			get_char_width_opti(t_game *game, char x);
void		set_width_all_letter(t_game *game);
int			get_text_width(t_game *game, char *str, int height);

//draw
void		pixel_put(t_game *game, int x, int y, int color);
void		secure_pixel_put(t_game *game, int x, int y, int color);
void		draw_rectangle(t_game *game, t_draw_info info);
void		draw_text(t_game *data, t_draw_info info);
void		draw_text_left(t_game *game, t_draw_info info);
void		draw_text_right(t_game *game, t_draw_info info);
void		draw_char(t_game *data, t_draw_info info);
void		draw_sprite(t_game *game, t_image *texture, t_sprite *sprite);
void		draw_sprites(t_game *game);
void		draw_rounded_rectangle(t_game *game, t_draw_info info);
void		draw_success_background(t_game *game);
void		draw_main_menu_button(t_game *game, int x, int y);
void		draw_selected_button(t_game *game, int x, int y);
void		victory_screen(t_game *game);
void		draw_hud(t_game *game);
void		crosshair(t_game *game);
int			blend_colors(int bg_color, int fg_color, float alpha);
int			get_pixel_color_from_image(t_game *game, int x, int y);
void		draw_image(t_game *game, t_image *img, t_draw_info info);
void		gun_draw(t_game *game);
void		get_pos_char(char c, int *x, int *y);
void		ammo_written(t_game *game);
void		health_point_draw(t_game *game);
void		apply_fade_to(t_game *game, int color);
int			is_pixel_transparent(t_image *img, int x, int y);

//fps
void		calculate_fps(t_game *game);
void		draw_fps(t_game *game, double fps);
void		calculate_delta_time(t_game *game);

// mini map
void		mini_map(t_game *game);
void		print_wall_door_player(t_game *game);
int			x_size_floor(t_game *game);
int			y_size_floor(t_game *game);

// sound
void		*test_music(void *arg);
void		music_launch(t_game *game);

//door
int			handle_door(t_game *game, t_raycast *raycast);
void		handle_door2(t_game *game, t_door *door, float distance,
				t_raycast *raycast);
bool		visible_door(t_door *door);
void		draw_door(t_game *game, t_raycast *r);
void		send_door_msg(t_game *game, t_door *door);
void		add_door(t_game *game, int x, int y, int floor);
t_door		*get_door(t_door *door, int x, int y, int floor);
void		use_door_in_view(t_game *game);
void		update_door_animation(t_game *game);

//wall
void		draw_wall(t_game *game, t_raycast *raycast);
void		draw_vertical_line_with_texture(t_game *game, t_raycast *raycast,
				t_image *texture, float wall_x);
void		draw_vertical_line_color(t_game *game, t_raycast *raycast,
				int color);
int			check_walls(t_game *game, int x, int y, int floor);
bool		is_colored_wall(t_game *game, t_raycast *raycast);
void		calculate_wall_distance(t_game *game, t_raycast *raycast);
float		calculate_wall_xx(t_game *game, t_raycast *raycast);
t_image		*select_texture(t_game *game, t_raycast *raycast);

//ennemies
void		init_update(t_game *game, t_sprite *current);
void		update_enemies(t_game *game);
void		update_enemies_patrol_move(t_game *game, t_sprite *current);
void		update_enemies_chase(t_game *game, t_sprite *current);
void		damages_red_draw(t_game *game, int y);
void		draw_dead_screen_multiplayer(t_game *game);
void		add_enemies(t_game *game, int x, int y, int floor);
void		death_enemies_animation(t_game *game, t_sprite *current);
void		shoot_at_player(t_sprite *enemy, t_game *game);
bool		is_within_fov(float angle_diff, float fov);
bool		check_line_of_sight(t_game *game, t_sprite *sprite,
				float dx, float dy);
bool		has_line_of_sight(t_game *game, t_sprite *sprite);

//sprites
void		sort_sprites(t_game *game, t_sprite **head);
int			is_player_in_front(t_sprite *enemy, t_player *player);
void		draw_sprites_enemies(t_game *game, t_sprite *current);
void		draw_sprites2(t_game *game, t_sprite *current);
void		sort_sprites_swap(t_sprite **prev, t_sprite **current,
				t_sprite **next, int *swapped);
void		sort_sprites_not_swap(t_sprite **prev, t_sprite **current,
				t_sprite **next);
t_sprite	*merge_sorted_lists(t_game *game, t_sprite *left,
				t_sprite *right);
void		split_list(t_sprite *source, t_sprite **left, t_sprite **right);
void		init_spritecast(t_game *game, t_sprite *sprite, t_image *texture);
int			get_spritecast_info(t_game *game, t_sprite *sp, int *sprite_index);
void		get_spritecast_tex_x(t_sprite *sprite, int stripe,
				int sprite_index);
void		draw_player_pseudo(t_game *game, t_sprite *sprite);
void		draw_vertical_sprite_band(t_game *game, t_sprite *sprite, int x);
float		calculate_distance(t_game *game, t_sprite *sprite);
bool		check_collision_with_entity(t_game *game, t_projectile *projectile,
				float ray[2]);

//projectile
void		add_projectil(t_sprite *enemy, t_game *game, float angle_to_player);
void		update_projectiles(t_game *game);
void		projectile_move(t_game *game, t_projectile *p, bool *collision);
void		projectile_collision(t_game *game, t_projectile **projectile,
				t_projectile **prev, int collision);

// PARSING
int			err(char *str);
void		init_data(t_game *game);
void		parsing(char **av, t_game *game);
char		*get_next_line(int fd);
void		floodfill(t_game *game);
int			check_path(t_game *game, int x, int y, int floor);
void		door_ennemi_ammo_health_mngmt(t_game *game);
int			file_dot_xpm(char *file_textre);
void		init_data(t_game *game);
char		*switch_line(t_memory_table *mem, char *line, int fd);
int			count_spawns(t_game *game);
int			check_allowed_char(t_game *game, int floor, int x, int y);
void		search_departure_position(t_game *game);

// teleporter
int			is_a_teleporter(char c);
void		teleportation_check(t_game *game);
void		use_teleporter(t_game *game);
void		set_output_teleporter(t_game *game, int x, int y, int floor);
void		add_teleporter(t_game *game, int x, int y, int floor);
int			teleportation(t_game *game, int x, int y, int floor);
void		animation_teleportation(t_game *game);

// map
int			map_copy(t_game *game);
char		*switch_line(t_memory_table *mem, char *line, int fd);
void		map_set_up(char **av, t_game *game);

// texture check
int			textures(char *to_open, t_game *game);
void		load_texture(t_game *game, t_image *img, char *path);
void		load_txtre_globale(t_game *game, char *line, char *path);

// free
int			free_map(t_game *game);
int			free_map_copy(t_game *game);
int			free_split(t_memory_table *mem, char **str);

// init
void		init_var(t_game *game, int malloc);
void		init_menu(t_game *game, int malloc);
void		init_client(t_game *game, int malloc);
void		init_player_keycode(t_game *game, int malloc);
void		init_player(t_game	*game, int malloc);
void		init_img(t_game *game);
void		init_floorcast(t_game *game, t_floorcast *f);

// load texture
void		load_game_texture(t_game *game, int malloc);
void		load_texture(t_game *game, t_image *img, char *path);

// collectable
void		on_ammo(t_game *game);
void		on_life(t_game *game);
void		draw_anim_health(t_game *game, t_sprite *sprite,
				t_image *im_health);

#endif