/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:57:44 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 12:57:44 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_index_char(char c)
{
	const char	list[96] = "!#$&(),;@[]:?_\"|\\\\/*<>%-'`~£$+=+0123456789"
		"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUu"
		"VvWwXxYyZz.";
	int			i;

	i = -1;
	while (list[++i])
	{
		if (list[i] == c)
		{
			if (i > 32)
				return (i - 1);
			return (i);
		}
	}
	return (-1);
}

int	get_char_width_return(int left_bound, int right_bound)
{
	if (left_bound > right_bound)
		return (0);
	return (right_bound - left_bound + 1);
}

int	get_char_width(t_image *img, char c)
{
	int			left_bound;
	int			right_bound;
	const int	char_x = (get_index_char(c) % 10) * 480;
	const int	char_y = (get_index_char(c) / 10) * 480;
	int			pos[2];

	left_bound = 480;
	right_bound = -1;
	pos[0] = -1;
	while (++pos[0] < 480)
	{
		pos[1] = -1;
		while (++pos[1] < 480)
		{
			if (!is_pixel_transparent(img, pos[1] + char_x, pos[0] + char_y))
			{
				if (pos[1] < left_bound)
					left_bound = pos[1];
				if (pos[1] > right_bound)
					right_bound = pos[1];
			}
		}
	}
	return (get_char_width_return(left_bound, right_bound));
}

int	get_char_width_opti(t_game *game, char x)
{
	return (game->menu->width_letter[get_index_char(x)]);
}

void	set_width_all_letter(t_game *game)
{
	const char	list[96] = "!#$&(),;@[]:?_\"|\\\\/*<>%-'`~£$+=+0123456789"
		"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUu"
		"VvWwXxYyZz.";
	int			i;
	int			width;
	int			index;

	i = -1;
	while (list[++i])
	{
		width = get_char_width(game->images->alphanum_sprite, list[i]);
		index = get_index_char(list[i]);
		if (index >= 0 && index < 96)
			game->menu->width_letter[index] = width;
	}
}
