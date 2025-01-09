# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/03 18:08:11 by ybeaucou          #+#    #+#              #
#    Updated: 2025/01/08 14:45:19 by ybeaucou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

# Paths
PATH_SRC = ./src/
PATH_OBJ = ./objs/
LIBS_DIR = ./libs/
INCLUDES = -I$(MINILIBX_HEADERS) -I$(LIBFT_HEADERS) -I$(RAUDIO_HEADERS) -I./includes/

# Compilation
CFLAGS = -g3 -O0 -Wall -Wextra -Werror
RM = rm -rf
OBJS = $(SRC:$(PATH_SRC)%.c=$(PATH_OBJ)%.o)

# Libraries
LIBS = -L$(MINILIBX_DIR) -lmlx \
		-L$(LIBFT_DIR) -lft \
		-L$(RAUDIO_SRC) -lraudio \
		-lX11 -lXfixes -lXext -lm 

# Sources
SRC = $(SRC_ALONE) $(SRC_GAME) $(SRC_MENU) $(SRC_MENU_CREATE_SERVER) \
		$(SRC_MENU_MAIN) $(SRC_MENU_MULTI) $(SRC_MENU_OPTION) $(SRC_MENU_STATS) \
		$(SRC_PARSING) $(SRC_SERVER) $(SRC_CLIENT) $(SRC_CHAT) $(SRC_RAYCASTER) \
		$(SRC_INPUT) $(SRC_SPRITES) $(SRC_STATS) $(SRC_MULTITHREAD) $(SRC_SOUND)

# Source Groups
SRC_GAME = $(addprefix $(PATH_SRC)game/, draw.c draw2.c draw3.c draw4.c \
				door.c door2.c door3.c wall.c wall2.c minimap.c minimap2.c \
				crosshair.c health_point.c ammo.c add_letter_to_list.c \
				game_loop.c game_loop2.c game_loop_utils.c dead.c)
SRC_STATS = $(addprefix $(PATH_SRC)stats/, get_stats.c save_stats.c)
SRC_MENU = $(addprefix $(PATH_SRC)menu/, join_server.c join_server2.c \
				server_disconnected.c game_over.c get_pseudo.c \
				get_key_name.c game_over_draw.c get_pseudo_handle.c func.c)
SRC_MENU_CREATE_SERVER = $(addprefix $(PATH_SRC)menu/create_server/, create_server.c \
							handle.c text_field.c update.c utils.c)
SRC_MENU_MAIN = $(addprefix $(PATH_SRC)menu/main/, main.c draw.c check_update.c)
SRC_MENU_MULTI = $(addprefix $(PATH_SRC)menu/multi/, discovery_delete.c discovery.c \
					draw_shape.c multi.c update_check.c utils.c server_list.c)
SRC_MENU_OPTION = $(addprefix $(PATH_SRC)menu/option/, keyboard.c keyboard2.c \
						mouse.c mouse2.c option.c option2.c sound.c sound2.c update.c draw.c)
SRC_MENU_STATS = $(addprefix $(PATH_SRC)menu/stats/, draw_menu.c scroll.c stats.c update.c)
SRC_ALONE = $(addprefix $(PATH_SRC), main.c mlx_func.c init_texture.c init.c init2.c)
SRC_PARSING = $(addprefix $(PATH_SRC)parsing/, free.c parsing.c teleportation.c \
					floodfill.c textures.c lil_functions.c begin_pos.c map_crafting.c)
SRC_SERVER = $(addprefix $(PATH_SRC)server/, broadcast.c enemies_shoot.c enemies.c \
					find_player.c handle.c logic.c loop_server.c new_player.c \
					notification.c player_node.c projectile.c queue.c \
					send.c server.c utils.c)
SRC_CLIENT = $(addprefix $(PATH_SRC)client/, client.c elements.c message.c \
					player.c health_ammo.c get_map.c)
SRC_CHAT = $(addprefix $(PATH_SRC)game/chat/, chat.c draw.c handle.c show_message.c)
SRC_RAYCASTER = $(addprefix $(PATH_SRC)game/raycaster/, raycaster.c raycaster2.c \
					ray.c floor.c)
SRC_SPRITES = $(addprefix $(PATH_SRC)game/sprite/, teleporter.c sprite.c \
					sprites2.c draw_sprite.c sorting_sprites.c enemies.c \
					ennemies2.c add_ennemies.c projectiles.c updates.c)
SRC_INPUT = $(addprefix $(PATH_SRC)input/, mouse.c keyboard.c keyboard_move.c \
					keyboard_utils.c key.c)
SRC_SOUND = $(addprefix $(PATH_SRC)game/sound/, sound_global.c sound_global2.c)
SRC_MULTITHREAD = $(addprefix $(PATH_SRC)multithread/, init.c pool.c worker.c)

# Libraries Configuration
MINILIBX_URL = https://github.com/42Paris/minilibx-linux.git
MINILIBX_DIR = $(LIBS_DIR)minilibx-linux/
MINILIBX_HEADERS = $(MINILIBX_DIR)
MINILIBX = $(MINILIBX_DIR)/libmlx.a

LIBFT_DIR = $(LIBS_DIR)Libft/
LIBFT_HEADERS = $(LIBFT_DIR)includes/
LIBFT = $(LIBFT_DIR)libft.a

RAUDIO_URL = https://github.com/raysan5/raudio.git
RAUDIO_DIR = $(LIBS_DIR)raudio/
RAUDIO_SRC = $(RAUDIO_DIR)src/
RAUDIO_HEADERS = $(RAUDIO_SRC)
RAUDIO = $(RAUDIO_SRC)/libraudio.a
RAUDIO_DEFINE = -D RAUDIO_STANDALONE -D SUPPORT_MODULE_RAUDIO \
					-D SUPPORT_FILEFORMAT_MP3 -D SUPPORT_FILEFORMAT_WAV \
					-D 'TRACELOG(level, ...)'

# Rules
all: $(NAME)

$(PATH_OBJ):
	mkdir -p $@ $@menu $@menu/create_server $@menu/main $@menu/multi $@menu/option \
		$@menu/stats $@parsing $@server $@client \
		$@game $@game/sound $@game/chat $@game/raycaster $@game/sprite \
		$@input $@stats $@multithread

$(PATH_OBJ)%.o: $(PATH_SRC)%.c | $(PATH_OBJ)
	cc -c $(CFLAGS) $(INCLUDES) $< -o $@

$(NAME): $(MINILIBX) $(RAUDIO) $(LIBFT) $(OBJS)
	cc $(CFLAGS) $(OBJS) $(LIBS) $(INCLUDES) -o $(NAME)

# Libraries Rules
$(LIBFT):
	make -sC $(LIBFT_DIR)

$(MINILIBX):
	@echo "Downloading minilibx-linux..."
	if [ ! -d $(MINILIBX_DIR) ]; then \
		git clone $(MINILIBX_URL) $(MINILIBX_DIR); \
	fi
	make -sC $(MINILIBX_DIR)

$(RAUDIO):
	@echo "Downloading raudio..."
	@if [ ! -d "$(RAUDIO_DIR)" ]; then \
		git clone $(RAUDIO_URL) $(RAUDIO_DIR); \
	else \
		cd $(RAUDIO_DIR) && git pull; \
	fi
	gcc -c $(RAUDIO_SRC)raudio.c $(RAUDIO_DEFINE) -o $(RAUDIO_SRC)raudio.o
	ar rcs $(RAUDIO) $(RAUDIO_SRC)raudio.o
	rm -f $(RAUDIO_SRC)raudio.o

clean:
	make clean -sC $(LIBFT_DIR)
	$(RM) $(PATH_OBJ)

fclean:
	make fclean -sC $(LIBFT_DIR)
	$(RM) $(PATH_OBJ)
	$(RM) $(NAME)
	$(RM) $(MINILIBX_DIR)
	$(RM) $(RAUDIO_DIR)

re: fclean all

.PHONY: all clean fclean re
