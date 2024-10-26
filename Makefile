# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 10:00:01 by npigeon           #+#    #+#              #
#    Updated: 2024/10/26 04:23:56 by ybeaucou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = cub3D

PATH_SRC = ./src/
PATH_OBJ = ./objs/
OBJS = ${SRC:$(PATH_SRC)%.c=$(PATH_OBJ)%.o}
LIBS = -L$(MINILIBX_DIR) -lmlx -L$(LIBFT_DIR) -lft -L$(OPENAL_DIR)build -lopenal -lX11 -lXext -lm #-lXfixes
INCLUDES = -I$(MINILIBX_HEADERS) -I$(LIBFT_HEADERS) -I$(OPENAL_HEADERS) -I./includes/ 
CFLAGS = -g3 -O3 -march=native -mtune=native -funroll-loops -finline-functions -flto -fomit-frame-pointer -ftree-vectorize -mavx #-Wall -Wextra -Werror
LIBS_DIR = ./libs/
RM = rm -rf

SRC_GAME =	$(addprefix $(PATH_SRC)game/, \
				draw.c \
				door.c \
				wall.c \
				minimap.c \
				crosshair.c \
				head_up_display.c \
				health_point.c \
				ammo.c \
				add_letter_to_list.c \
				game_loop.c )

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
				stats.c )

SRC_ALONE =	$(addprefix $(PATH_SRC), \
				main.c \
				init.c )
			
SRC_PARSING = $(addprefix $(PATH_SRC)parsing/, \
				free.c \
				parsing.c \
				teleportation.c \
				floodfill.c \
				textures.c \
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
				send.c )

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

SRC =	$(SRC_ALONE) $(SRC_GAME) $(SRC_MENU) $(SRC_PARSING) $(SRC_SERVER) $(SRC_CLIENT) $(SRC_CHAT) $(SRC_RAYCASTER) $(SRC_INPUT) $(SRC_SPRITES) $(SRC_STATS)

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

################ OPENAL ################

OPENAL_URL = https://github.com/kcat/openal-soft.git
OPENAL_DIR = $(LIBS_DIR)openal-soft/
OPENAL_HEADERS = $(OPENAL_DIR)include/
OPENAL = $(OPENAL_DIR)libopenal.so

########################################

################ DR_WAV ################

DR_WAV_URL = https://raw.githubusercontent.com/mackron/dr_libs/master/dr_wav.h
DR_WAV = ./includes/dr_wav.h

########################################

all: $(NAME)

$(OBJS): ./includes/* Makefile

$(PATH_OBJ):
	mkdir -p $@ $@menu $@parsing $@server $@client $@game $@game/chat $@game/raycaster $@game/sprite $@input $@stats

$(PATH_OBJ)%.o: $(PATH_SRC)%.c $(DR_WAV) | $(PATH_OBJ)
	cc -c $(CFLAGS) $(INCLUDES) $< -o $@

$(NAME): $(MINILIBX) $(LIBFT) $(OPENAL) $(OBJS) ./includes/* Makefile
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

# Download openal-soft and compile it
$(OPENAL):
	@echo "Downloading openal-soft..."
	if [ ! -d $(OPENAL_DIR) ]; then \
		git clone $(OPENAL_URL) $(OPENAL_DIR); \
	fi
	cd $(OPENAL_DIR) && mkdir -p build && cd build && cmake .. && make

# Download dr_wav.h
$(DR_WAV):
	@echo "Downloading dr_wav.h..."
	if [ ! -f $(DR_WAV) ]; then \
		curl -o $(DR_WAV) $(DR_WAV_URL); \
	fi

clean:
	make clean -sC $(LIBFT_DIR)
	$(RM) $(PATH_OBJ)

fclean:
	make fclean -sC $(LIBFT_DIR)
	$(RM) $(PATH_OBJ)
	$(RM) $(NAME)
	$(RM) $(MINILIBX_DIR)
	$(RM) $(OPENAL_DIR)
	$(RM) $(DR_WAV)

re: fclean all
	
.PHONY: all clean fclean re
