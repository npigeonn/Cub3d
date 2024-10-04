/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 23:37:48 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/04 16:33:45 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MAP_FLOOR 3

typedef struct s_mlx {
	void *mlx;
	void *window;
	void *image;
	char *data;
	int bpp;
	int size_line;
	int endian;
} t_mlx;

// char map[MAP_FLOOR][MAP_WIDTH][MAP_HEIGHT] = {
// 	{
// 		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
// 		{'1', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', '1'},
// 		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
// 	},
// 	{
// 		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
// 	},
// 	{
// 		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
// 		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
// 		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
// 	},
// };

void draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
	if (x > SCREEN_WIDTH || x < 0)
		return;
	for (int y = start; y < end; y++)
	{
		if (y >= 0 && y < SCREEN_HEIGHT && *((int *)(game->img->data + y * game->img->size_line + x * (game->img->bpp / 8))) == 0)
			*((int *)(game->img->data + y * game->img->size_line + x * (game->img->bpp / 8))) = color;
	}
}

void draw_ceilling(t_game *game)
{
	int horizon = (SCREEN_HEIGHT / 2) + game->player->height;
	if (horizon < 0)
		horizon = 0;
	else if (horizon > SCREEN_HEIGHT)
		horizon = SCREEN_HEIGHT;
	for (int y = horizon; y > 0; y--)
	{
		int color = 0x896542;
		int shadowFactor = (y + horizon) * 255 / (SCREEN_HEIGHT + horizon);
		shadowFactor = fmin(1.0f, shadowFactor);
		int shadowedColor = ((int)((color & 0xFF0000) * shadowFactor) & 0xFF0000) |
						((int)((color & 0x00FF00) * shadowFactor) & 0x00FF00) |
						((int)((color & 0x0000FF) * shadowFactor) & 0x0000FF);
	   for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			if (y >= 0 && y < SCREEN_HEIGHT && *(int *)(game->img->data + y * game->img->size_line + x * (game->img->bpp / 8)) == 0)
				*((int *)(game->img->data + y * game->img->size_line + x * (game->img->bpp / 8))) = shadowedColor;
		}
	}
}

void draw_floor(t_game *game)
{
	int horizon = (SCREEN_HEIGHT / 2) + game->player->height;
	if (horizon < 0)
		horizon = 0;
	else if (horizon > SCREEN_HEIGHT)
		horizon = SCREEN_HEIGHT;
	for (int y = horizon; y < SCREEN_HEIGHT; y++)
	{
		int color = 0xaba89f;
		int shadowFactor = (y - horizon) * 255 / (SCREEN_HEIGHT - horizon);
		shadowFactor = fmin(1.0f, shadowFactor);
		int shadowedColor = ((int)((color & 0xFF0000) * shadowFactor) & 0xFF0000) |
						((int)((color & 0x00FF00) * shadowFactor) & 0x00FF00) |
						((int)((color & 0x0000FF) * shadowFactor) & 0x0000FF);
	   for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			if (y >= 0 && y < SCREEN_HEIGHT && *(int *)(game->img->data + y * game->img->size_line + x * (game->img->bpp / 8)) == 0)
				*((int *)(game->img->data + y * game->img->size_line + x * (game->img->bpp / 8))) = shadowedColor;
		}
	}
}

void draw_wall(t_game *game, int x, int mapX, int mapY, int stepX, int stepY, float rayDirX, float rayDirY, int side)
{
	float perpWallDist = (side == SIDE_EAST) || (side == SIDE_WEST)
		? (mapX - game->player->x + (1 - stepX) / 2) / rayDirX
		: (mapY - game->player->y + (1 - stepY) / 2) / rayDirY;
		
	int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
	int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
	int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
	drawStart -= (int)(game->player->height * lineHeight);
	drawEnd -= (int)(game->player->height * lineHeight);

	if (drawStart < 0) drawStart = 0;
	if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

	int color;
	if (side == SIDE_EAST)
		color = 0x00FFFF; // Cyan
	else if (side == SIDE_NORTH)
		color = 0xFF0000; // Rouge
	else if (side == SIDE_SOUTH)
		color = 0x0000FF; // Bleu
	else
		color = 0xFFFF00; // Jaune

	float shadowFactor = fmax(0.1f, 1.0f - (perpWallDist / 100.0f));
	int shadowedColor = ((int)((color & 0xFF0000) * shadowFactor) & 0xFF0000) |
						((int)((color & 0x00FF00) * shadowFactor) & 0x00FF00) |
						((int)((color & 0x0000FF) * shadowFactor) & 0x0000FF);
	draw_vertical_line(game, x, drawStart - 1, drawEnd, shadowedColor);
}

bool	allFloorsHit(int *floorHit)
{
	for (int floor = 0; floor < MAP_FLOOR; floor++)
	{
		if (floorHit[floor] == 0)
			return false;
	}
	return true;
}

void	cast_rays(t_game *game)
{
	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
		float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
		float rayDirX = game->player->dirX + game->player->planeX * cameraX;
		float rayDirY = game->player->dirY + game->player->planeY * cameraX;
		// rayDirY += tan(game->player->pitch);

		int mapX = (int)game->player->x;
		int mapY = (int)game->player->y;

		float deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
		float deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

		int stepX, stepY;
		float sideDistX, sideDistY;

		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (game->player->x - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - game->player->x) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (game->player->y - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - game->player->y) * deltaDistY;
		}
		int side;

		while (1)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				if (stepX > 0) 
					side = SIDE_EAST;
				else 
					side = SIDE_WEST;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;

				if (stepY > 0)
					side = SIDE_SOUTH;
				else
					side = SIDE_NORTH;
			}
			if (!game->map[game->player->floor][mapX] || !game->map[game->player->floor][mapX][mapY])
				break;
			if (game->map[game->player->floor][mapX][mapY] == '1')
			{
				draw_wall(game, x, mapX, mapY, stepX, stepY, rayDirX, rayDirY, side);
				break;
			}
		}
	}
}
int	handle_close(t_game *game)
{
	mlx_destroy_image(game->mlx, game->img->img);
	mlx_destroy_window(game->mlx, game->win);
	mlx_destroy_display(game->mlx);
	free(game->mlx);
	exit(0);
}

int	handle_mouse_move(int x, int y, t_game *game)
{

    float deltaX = -(x - SCREEN_WIDTH / 2) * ROTATION_SPEED;

    float oldDirX = game->player->dirX;
    game->player->dirX = oldDirX * cos(-deltaX) - game->player->dirY * sin(-deltaX);
    game->player->dirY = oldDirX * sin(-deltaX) + game->player->dirY * cos(-deltaX);
    float oldPlaneX = game->player->planeX;
    game->player->planeX = oldPlaneX * cos(-deltaX) - game->player->planeY * sin(-deltaX);
    game->player->planeY = oldPlaneX * sin(-deltaX) + game->player->planeY * cos(-deltaX);
	mlx_mouse_move(game->mlx, game->win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	(void)y;
	return (0);
}


int	handle_keypress(int keycode, t_game *game)
{
	float x, y;
	
	if (keycode == 65307) // TODO a proteger pour l'instant ca segfault
		handle_close(game);
	if (keycode == 65362 || keycode == 119) // W pour avancer
	{
		x = game->player->x + game->player->dirX * 0.1;
		y = game->player->y + game->player->dirY* 0.1;
		if (game->map[game->player->floor][(int)x][(int)y] == '1')
			return (0);
		game->player->x = x;
		game->player->y = y;
	}
	if (keycode == 65364 || keycode == 115) // S pour reculer
	{
		game->player->x -= game->player->dirX * 0.1;
		game->player->y -= game->player->dirY * 0.1;
	}
	if (keycode == 65363 || keycode == 100) // D pour aller à droite
	{
		game->player->x += game->player->planeX * 0.1;
		game->player->y += game->player->planeY * 0.1;
	}

	if (keycode == 65361 || keycode == 97) // A pour aller à gauche
	{
		game->player->x -= game->player->planeX * 0.1;
		game->player->y -= game->player->planeY * 0.1;
	}
	if (keycode == 32) // Espace pour sauter
		game->player->height -= 0.1;
	if (keycode == 98) // b pour s'accroupir
		game->player->height += 0.1;
	return (0);
}

void clear_image(t_image *img, int color)
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			int *pixel = (int *)(img->data + y * img->size_line + x * (img->bpp / 8));
			*pixel = color;
		}
	}	
}

int	game_loop(t_game *game)
{
	if (game->map[0][(int)game->player->x][(int)game->player->y] == 'e')
	{
		ft_printf("VICTORY\n");
		exit(EXIT_SUCCESS);
	}
	mlx_clear_window(game->mlx, game->win);
	clear_image(game->img, 0x000000);
	cast_rays(game);
	draw_ceilling(game);
	draw_floor(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img->img, 0, 0);
	return (0);
}

void	set_dir(t_game *game, int dir)
{
	if (dir == 0) // Nord
	{
		game->player->dirX = 0;
		game->player->dirY = -1;
		game->player->planeX = 0.66; // Largeur du plan perpendiculaire
		game->player->planeY = 0;
	}
	else if (dir == 1) // Sud
	{
		game->player->dirX = 0;
		game->player->dirY = 1;
		game->player->planeX = -0.66; // Inversé car Sud
		game->player->planeY = 0;
	}
	else if (dir == 2) // Est
	{
		game->player->dirX = 1;
		game->player->dirY = 0;
		game->player->planeX = 0;
		game->player->planeY = 0.66; // Plan perpendiculaire à droite (Y+)
	}
	else if (dir == 3) // Ouest
	{
		game->player->dirX = -1;
		game->player->dirY = 0;
		game->player->planeX = 0;
		game->player->planeY = -0.66; // Plan perpendiculaire à gauche (Y-)
	}
}


int main(int ac, char **av)
{
	t_game		game;
	(void)ac;
	// (void)**av;
	
	game.mlx = mlx_init();

	game.player = malloc(sizeof(t_player));
	game.player->x = 0;
	game.player->y = 0;
	parsing(av, &game);
	game.player->x = game.player->x + 0.5;
	game.player->y = game.player->y + 0.5;
	game.player->height = 0;
	game.player->dirX = 1;
	game.player->dirY = 0;
	game.player->planeX = 0;
	set_dir(&game, 0);
	game.win = mlx_new_window(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "Raycasting 3D");
	game.img = malloc(sizeof(t_image));
	game.img->img = mlx_new_image(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	game.img->data = mlx_get_data_addr(game.img->img, &game.img->bpp, &game.img->size_line, &game.img->endian);
	mlx_mouse_move(game.mlx, game.win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	mlx_mouse_hide(game.mlx, game.win);
	mlx_hook(game.win, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.win, 6, 1L << 6, handle_mouse_move, &game);
	mlx_hook(game.win, 33, 0, handle_close, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	return 0;
}
