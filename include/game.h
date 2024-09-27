
#ifndef GAME_H
#define GAME_H

# include <math.h>

# include "./minilibx-linux/mlx.h"
# include "./minilibx-linux/mlx_int.h"
# include "Libft/includes/libft.h"
# include "Libft/includes/gc.h"
# include "Libft/includes/ft_printf.h"

# define WIDTH 800
# define HEIGHT 800

typedef struct s_player
{
	float	x;
	float	y;
	float	p;
}    t_player;

typedef struct s_data
{
	void	*mlx;
	void	*window;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		fct;
	int		color;
	double	x_offset;
	double	y_offset;
	double	zoom;
	int		xx;
	int		yy;
}	t_data;


#endif