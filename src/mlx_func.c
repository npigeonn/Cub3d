/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_func.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:11:33 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/08 14:50:34 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	destroy_mlx_img(t_block_info *params)
{
	mlx_destroy_image(params->ptr, params->ptr2);
}

void	destroy_mlx_display(t_block_info *params)
{
	mlx_destroy_display(params->ptr);
	free(params->ptr);
}

void	hooks(t_game *game)
{
	mlx_mouse_move(game->mlx, game->win, game->screen_width >> 1,
		game->screen_height >> 1);
	mlx_hook(game->win, 6, 1L << 6, handle_mouse_move, game);
	mlx_hook(game->win, 4, 1L << 2, handle_mouse_key_press, game);
	mlx_hook(game->win, 5, 1L << 3, handle_mouse_key_release, game);
	mlx_hook(game->win, 33, 0, handle_close, game);
	mlx_hook(game->win, 2, 1L << 0, handle_key_press, game);
	mlx_hook(game->win, 3, 1L << 1, handle_key_release, game);
	mlx_loop_hook(game->mlx, game_loop, game);
	mlx_loop(game->mlx);
}

void	x_fixes_cursor(t_game *game, char to_do)
{
	if (to_do == 'h' && game->mouse)
	{
		XFixesHideCursor(((t_xvar *)(game->mlx))->display,
			((t_win_list *)(game->win))->window);
		game->mouse = 0;
		return ;
	}
	else if ((to_do == 'h' && !game->mouse)
		|| (to_do == 's' && game->mouse))
		return ;
	else if (to_do == 's' && !game->mouse)
	{
		XFixesShowCursor(((t_xvar *)(game->mlx))->display,
			((t_win_list *)(game->win))->window);
		game->mouse = 1;
		return ;
	}
}
