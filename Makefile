# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 10:00:01 by npigeon           #+#    #+#              #
#    Updated: 2024/09/27 10:27:53 by ybeaucou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

PATH_SRC = ./src/
PATH_OBJ = ./objs/
OBJS = ${SRC:$(PATH_SRC)%.c=$(PATH_OBJ)%.o}
LIBS = -L$(MINILIBX_DIR) -lmlx -lX11 -lXext -lm -L$(LIBFT_DIR) -lft
INCLUDES = -I$(MINILIBX_HEADERS) -I$(LIBFT_HEADERS) -I$(GC_HEADERS) -I./includes/
CFLAGS = -g3 -Wall -Wextra -Werror
RM = rm -rf

SRC_PARSING =	$(addprefix $(PATH_SRC)parsing/, \
				parsing.c) \


SRC_ERROR =	$(addprefix $(PATH_SRC)error/, \
			error.c )

SRC_EXEC =	$(addprefix $(PATH_SRC)exec/, \
			split.c \
			split_utils.c)

SRC_ALONE =	$(addprefix $(PATH_SRC), \
			ansii.c \
			main.c)

SRC =	$(SRC_ALONE) $(SRC_ERROR) $(SRC_EXEC) $(SRC_PARSING)

############### MINILIBX ###############

MINILIBX_URL = https://github.com/42Paris/minilibx-linux.git
MINILIBX_DIR = $(LIBS_DIR)minilibx-linux/
MINILIBX_HEADERS = $(MINILIBX_DIR)
MINILIBX = $(MINILIBX_DIR)/libmlx.a

################ LIBFT #################

LIBFT_DIR = ./Libft/
LIBFT_HEADERS = $(LIBFT_DIR)includes/
LIBFT = $(LIBFT_DIR)libft.a

########################################

all: $(NAME)

$(OBJS): ./includes/* Makefile

$(PATH_OBJ):
	mkdir -p $@ $@parsing $@exec  $@error
	
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

re: fclean all
	
.PHONY: all clean fclean re
