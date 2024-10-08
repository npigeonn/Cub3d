# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 10:00:01 by npigeon           #+#    #+#              #
#    Updated: 2024/10/10 10:30:45 by ybeaucou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

PATH_SRC = ./src/
PATH_OBJ = ./objs/
OBJS = ${SRC:$(PATH_SRC)%.c=$(PATH_OBJ)%.o}
LIBS = -L$(MINILIBX_DIR) -lmlx -lX11 -lXext -lm -L$(LIBFT_DIR) -lft
INCLUDES = -I$(MINILIBX_HEADERS) -I$(LIBFT_HEADERS) -I$(GC_HEADERS) -I./includes/
CFLAGS = -g3 #-Wall -Wextra -Werror
LIBS_DIR = ./libs/
RM = rm -rf

SRC_GAME =	$(addprefix $(PATH_SRC)game/, \
			raycaster.c \
			draw.c \
			door.c \
			wall.c \
			teleporter.c \
			sprite.c \
			enemies.c )

SRC_MENU =	$(addprefix $(PATH_SRC)/game/menu/, \
			main.c \
			option.c \
			multi.c)

SRC_ALONE =	$(addprefix $(PATH_SRC), \
			main.c )
			
SRC_PARSING =	$(addprefix $(PATH_SRC)parsing/, \
				free.c \
				parsing.c \
				teleportation.c \
				floodfill.c \
				textures.c \
				map_crafting.c )

SRC =	$(SRC_ALONE) $(SRC_GAME) $(SRC_MENU) $(SRC_PARSING)

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
	mkdir -p $@ $@game $@game/menu $@parsing

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
