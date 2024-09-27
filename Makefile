# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 10:00:01 by npigeon           #+#    #+#              #
#    Updated: 2024/09/27 10:23:23 by npigeon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

PATH_SRC = ./src/
PATH_OBJ = ./objs/
OBJS = ${SRC:$(PATH_SRC)%.c=$(PATH_OBJ)%.o}
INCLUDES = -I$(LIBFT_HEADERS) -I./includes/
LIBS = -L$(LIBFT_DIR) -lft
CFLAGS = -g3 -Wall -Wextra -Werror
RM = rm -rf

SRC_PARSING =	$(addprefix $(PATH_SRC)parsing/, \
				parsing.c) \


SRC_ERROR =	$(addprefix $(PATH_SRC)error/, \
			error.c \

SRC_EXEC =	$(addprefix $(PATH_SRC)exec/, \
			split.c \
			split_utils.c)

SRC_ALONE =	$(addprefix $(PATH_SRC), \
			ansii.c \
			main.c)

SRC =	$(SRC_ALONE) $(SRC_ERROR) $(SRC_EXEC) $(SRC_PARSING)


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

$(NAME): $(LIBFT) $(OBJS) ./includes/* Makefile 
	cc $(CFLAGS) $(OBJS) $(LIBS) $(INCLUDES) -o $(NAME)

$(LIBFT):
	make -sC $(LIBFT_DIR)

clean:
	make clean -sC $(LIBFT_DIR)
	$(RM) $(PATH_OBJ)

fclean:
	make fclean -sC $(LIBFT_DIR)
	$(RM) $(PATH_OBJ)
	$(RM) $(NAME)

re: fclean all

valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./$(NAME)
	
.PHONY: all clean fclean re




LDFLAGS = -L./include/Libft -lftLibft -L./minilibx-linux/ -lmlx_Linux\
		-lX11 -lXext -lm -march=native -O3 -flto -pipe


CFLAGS = -Werror -Wall -Wextra -g3

all: libftLibft.a minilibx.a $(NAME)

$(NAME): $(SRC)
	$(CC) $(SRC) -o $(NAME) $(LDFLAGS) $(CFLAGS)

clean:
	rm -f $(NAME)
	make -C ./include/Libft clean
	make -C ./minilibx-linux clean

fclean: clean
	rm -f $(NAME)
	make fclean -C include/Libft 
	make clean -C ./minilibx-linux

re: fclean all

libftLibft.a:
	make -C ./include/libft

minilibx.a :
	make -C ./minilibx-linux

.PHONY: all clean fclean re

