# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 10:00:01 by npigeon           #+#    #+#              #
#    Updated: 2024/10/19 14:53:33 by ybeaucou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = cub3D

PATH_SRC = ./src/
PATH_OBJ = ./objs/
OBJS = ${SRC:$(PATH_SRC)%.c=$(PATH_OBJ)%.o}
LIBS = -L$(MINILIBX_DIR) -lmlx -lX11 -lXext -lm -L$(LIBFT_DIR) -lft
INCLUDES = -I$(MINILIBX_HEADERS) -I$(LIBFT_HEADERS) -I$(GC_HEADERS) -I./includes/
CFLAGS = -g3 -march=native -O3 -flto -pipe #-Wall -Wextra -Werror
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
				game_loop.c )

SRC_MENU =	$(addprefix $(PATH_SRC)menu/, \
				option.c \
				multi.c \
				main.c \
				create_server.c \
				join_server.c \
				server_disconnected.c )

SRC_ALONE =	$(addprefix $(PATH_SRC), \
				main.c )
			
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
				keyboard_utils.c )

SRC =	$(SRC_ALONE) $(SRC_GAME) $(SRC_MENU) $(SRC_PARSING) $(SRC_SERVER) $(SRC_CLIENT) $(SRC_CHAT) $(SRC_RAYCASTER) $(SRC_INPUT) $(SRC_SPRITES)

############### MINILIBX ###############

MINILIBX_URL = https://github.com/42Paris/minilibx-linux.git
MINILIBX_DIR = $(LIBS_DIR)minilibx-linux/
MINILIBX_HEADERS = $(MINILIBX_DIR)
MINILIBX = $(MINILIBX_DIR)/libmlx.a

################ LIBFT #################

LIBFT_DIR = $(LIBS_DIR)Libft/
LIBFT_HEADERS = $(LIBFT_DIR)includes/
LIBFT = $(LIBFT_DIR)libft.a

########################################

all: $(NAME)

$(OBJS): ./includes/* Makefile

$(PATH_OBJ):
	mkdir -p $@ $@menu $@parsing $@server $@client $@game $@game/chat $@game/raycaster $@game/sprite $@input

$(PATH_OBJ)%.o: $(PATH_SRC)%.c | $(PATH_OBJ)
	cc -c $(CFLAGS) $(INCLUDES) $< -o $@

$(NAME): $(MINILIBX) $(LIBFT) $(GC) $(OBJS) ./includes/* Makefile 
	cc $(CFLAGS) $(OBJS) $(LIBS) $(INCLUDES) -o $(NAME)

$(LIBFT):
	make -sC $(LIBFT_DIR)

$(MINILIBX):
	@echo "Downloading minilibx-linux..."
	if [ ! -d $(MINILIBX_DIR) ]; then \
		git clone $(MINILIBX_URL) $(MINILIBX_DIR); \
	fi
	make -sC $(MINILIBX_DIR)

clean:
	make clean -sC $(LIBFT_DIR)
	$(RM) $(PATH_OBJ)

fclean:
	make fclean -sC $(LIBFT_DIR)
	$(RM) $(PATH_OBJ)
	$(RM) $(NAME)
	$(RM) $(MINILIBX_DIR)

re: fclean all
	
.PHONY: all clean fclean re
