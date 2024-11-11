# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 10:00:01 by npigeon           #+#    #+#              #
#    Updated: 2024/11/09 18:56:03 by ybeaucou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = cub3D

PATH_SRC = ./src/
PATH_OBJ = ./objs/
OBJS = ${SRC:$(PATH_SRC)%.c=$(PATH_OBJ)%.o}
LIBS = -L$(MINILIBX_DIR) -lmlx -L$(LIBFT_DIR) -lft -L$(RAUDIO_SRC) -lraudio -lX11 -lXfixes -lXext -lm 
INCLUDES = -I$(MINILIBX_HEADERS) -I$(LIBFT_HEADERS) -I$(RAUDIO_HEADERS) -I./includes/
CFLAGS = -g3 -O0
LIBS_DIR = ./libs/
RM = rm -rf

SRC_GAME =	$(addprefix $(PATH_SRC)game/, \
				draw.c \
				door.c \
				wall.c \
				minimap.c \
				crosshair.c \
				health_point.c \
				ammo.c \
				add_letter_to_list.c \
				game_loop.c \
				dead.c )

SRC_STATS =	$(addprefix $(PATH_SRC)stats/, \
				get_stats.c )

SRC_MENU =	$(addprefix $(PATH_SRC)menu/, \
				option.c \
				multi.c \
				main.c \
				create_server.c \
				join_server.c \
				server_disconnected.c \
				game_over.c \
				get_pseudo.c \
				stats.c \
				get_key_name.c )

SRC_ALONE =	$(addprefix $(PATH_SRC), \
				main.c \
				init.c )
			
SRC_PARSING = $(addprefix $(PATH_SRC)parsing/, \
				free.c \
				parsing.c \
				teleportation.c \
				floodfill.c \
				textures.c \
				lil_functions.c \
				begin_pos.c \
				map_crafting.c )

SRC_SERVER = $(addprefix $(PATH_SRC)server/, \
				server.c \
				broadcast.c \
				new_player.c \
				find_player.c \
				handle.c \
				logic.c \
				notification.c \
				player_node.c \
				queue.c \
				send.c \
				projectile.c )

SRC_CLIENT = $(addprefix $(PATH_SRC)client/, \
				client.c \
				elements.c \
				message.c \
				player.c)

SRC_CHAT =	$(addprefix $(PATH_SRC)game/chat/, \
				chat.c \
				draw.c \
				handle.c \
				show_message.c )

SRC_RAYCASTER = $(addprefix $(PATH_SRC)game/raycaster/, \
				raycaster.c \
				ray.c \
				floor.c )

SRC_SPRITES = $(addprefix $(PATH_SRC)game/sprite/, \
				teleporter.c \
				sprite.c \
				enemies.c )
	
SRC_INPUT = $(addprefix $(PATH_SRC)input/, \
				mouse.c \
				keyboard.c \
				keyboard_utils.c \
				key.c )

SRC_SOUND = $(addprefix $(PATH_SRC)game/sound/, \
				sound_global.c \
				side_sounds.c )

SRC_MULTITHREAD = $(addprefix $(PATH_SRC)multithread/, \
				init.c )

SRC =	$(SRC_SOUND) $(SRC_ALONE) $(SRC_GAME) $(SRC_MENU) $(SRC_PARSING) $(SRC_SERVER) $(SRC_CLIENT) $(SRC_CHAT) $(SRC_RAYCASTER) $(SRC_INPUT) $(SRC_SPRITES) $(SRC_STATS) $(SRC_MULTITHREAD)

############### MINILIBX ###############

MINILIBX_URL = https://github.com/42Paris/minilibx-linux.git
MINILIBX_DIR = $(LIBS_DIR)minilibx-linux/
MINILIBX_HEADERS = $(MINILIBX_DIR)
MINILIBX = $(MINILIBX_DIR)/libmlx.a

########################################

################ LIBFT #################

LIBFT_DIR = $(LIBS_DIR)Libft/
LIBFT_HEADERS = $(LIBFT_DIR)includes/
LIBFT = $(LIBFT_DIR)libft.a

########################################

################ RAUDIO ################

RAUDIO_URL = https://github.com/raysan5/raudio.git
RAUDIO_DIR = $(LIBS_DIR)raudio/
RAUDIO_SRC = $(RAUDIO_DIR)src/
RAUDIO_HEADERS = $(RAUDIO_SRC)
RAUDIO = $(RAUDIO_SRC)/libraudio.a
RAUDIO_DEFINE = -D RAUDIO_STANDALONE -D SUPPORT_MODULE_RAUDIO -D SUPPORT_FILEFORMAT_MP3 -D SUPPORT_FILEFORMAT_WAV -D 'TRACELOG(level, ...)'

########################################

all: $(NAME)

$(OBJS): ./includes/* Makefile

$(PATH_OBJ):
	mkdir -p $@ $@menu $@parsing $@server $@client $@game $@game/sound $@game/chat $@game/raycaster $@game/sprite $@input $@stats $@multithread

$(PATH_OBJ)%.o: $(PATH_SRC)%.c | $(PATH_OBJ)
	cc -c $(CFLAGS) $(INCLUDES) $< -o $@

$(NAME): $(MINILIBX) $(RAUDIO) $(LIBFT) $(OBJS) ./includes/* Makefile
	cc $(CFLAGS) $(OBJS) $(LIBS) $(INCLUDES) -o $(NAME)

# Compile libft
$(LIBFT):
	make -sC $(LIBFT_DIR)

# Download minilibx-linux and compile it
$(MINILIBX):
	@echo "Downloading minilibx-linux..."
	if [ ! -d $(MINILIBX_DIR) ]; then \
		git clone $(MINILIBX_URL) $(MINILIBX_DIR); \
	fi
	make -sC $(MINILIBX_DIR)

# Download raudio and compile it
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
