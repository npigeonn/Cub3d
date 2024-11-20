/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_key_name.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:40:04 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/19 13:18:00 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	*letter_to_name(t_game *game, int keycode)
{
	char	*name;

	name = gc_malloc(game->mem, 2);
	name[1] = '\0';
	name[0] = ft_toupper((char)keycode);
	return (name);
}

char	*get_key_name4(int keycode)
{
	if (keycode == 45)
		return ("6");
	if (keycode == 232)
		return ("7");
	if (keycode == 95)
		return ("8");
	if (keycode == 231)
		return ("9");
	if (keycode == 224)
		return ("0");
	if (keycode == 41)
		return ("-");
	if (keycode == 61)
		return ("=");
	if (keycode == 65362)
		return ("UP");
	if (keycode == 65361)
		return ("LEFT");
	if (keycode == 65364)
		return ("DOWN");
	if (keycode == 65363)
		return ("RIGHT");
	return ("NONE");
}

char	*get_key_name3(int keycode)
{
	if (keycode == 44)
		return (",");
	if (keycode == 46)
		return (".");
	if (keycode == 47)
		return ("/");
	if (keycode == 65106)
		return ("IDK");
	if (keycode == 36)
		return ("$");
	if (keycode == 249)
		return ("%%");
	if (keycode == 38)
		return ("1");
	if (keycode == 233)
		return ("2");
	if (keycode == 34)
		return ("3");
	if (keycode == 39)
		return ("4");
	if (keycode == 40)
		return ("5");
	return (get_key_name4(keycode));
}

char	*get_key_name2(t_game *game, int keycode)
{
	if (keycode == 65481)
		return ("F12");
	if (keycode == 65289)
		return ("TAB");
	if (keycode >= 97 && keycode <= 122)
		return (letter_to_name(game, keycode));
	if (keycode == 65509)
		return ("CAPS LOCK");
	if (keycode == 65505)
		return ("LEFT SHIFT");
	if (keycode == 65507)
		return ("LEFT CTRL");
	if (keycode == 65513)
		return ("LEFT ALT");
	if (keycode == 65027)
		return ("RIGHT ALT");
	if (keycode == 65383)
		return ("FN");
	if (keycode == 65508)
		return ("RIGHT CTRL");
	if (keycode == 65506)
		return ("RIGHT SHIFT");
	if (keycode == 65293)
		return ("ENTER");
	return (get_key_name3(keycode));
}

char	*get_key_name(t_game *game, int keycode)
{
	if (keycode == 65307)
		return ("ESC");
	if (keycode == 65470)
		return ("F1");
	if (keycode == 65471)
		return ("F2");
	if (keycode == 65472)
		return ("F3");
	if (keycode == 65473)
		return ("F4");
	if (keycode == 65474)
		return ("F5");
	if (keycode == 65475)
		return ("F6");
	if (keycode == 65476)
		return ("F7");
	if (keycode == 65477)
		return ("F8");
	if (keycode == 65478)
		return ("F9");
	if (keycode == 65479)
		return ("F10");
	if (keycode == 65480)
		return ("F11");
	return (get_key_name2(game, keycode));
}
