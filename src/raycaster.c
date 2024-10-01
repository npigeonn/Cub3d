/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 23:37:48 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/10/02 00:04:56 by ybeaucou         ###   ########.fr       */
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

char map[MAP_FLOOR][MAP_WIDTH][MAP_HEIGHT] = {
	{
		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
		{'1', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F', '1'},
		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	},
	{
		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	},
	{
		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1'},
		{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
		{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	},
};

void draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
	for (int y = start; y < end; y++)
	{
		if (y >= 0 && y < SCREEN_HEIGHT && *((int *)(game->img->data + y * game->img->size_line + x * (game->img->bpp / 8))) == 0)
			*((int *)(game->img->data + y * game->img->size_line + x * (game->img->bpp / 8))) = color;
	}
}

int	draw_wall(t_game *game, int x, int mapX, int mapY, int stepX, int stepY, float rayDirX, float rayDirY, int side, int currentFloor, int hitFloor)
{
	float perpWallDist = (side == 0)
		? (mapX - game->player->x + (1 - stepX) / 2) / rayDirX
		: (mapY - game->player->y + (1 - stepY) / 2) / rayDirY;
	int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
	int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
	int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
	drawStart -= (int)((hitFloor - currentFloor) * lineHeight);
	drawEnd -= (int)((hitFloor - currentFloor) * lineHeight);
	drawStart -= (int)(game->player->height  * lineHeight);
	drawEnd -= (int)(game->player->height * lineHeight);
	if (drawStart < 0) drawStart = 0;
	if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
	int color = (side == 0) ? 0xFF0000 : 0x00FF00;
	float shadowFactor = fmax(0.1f, 1.0f - (perpWallDist / 100.0f));
	int shadowedColor = ((int)((color & 0xFF0000) * shadowFactor) & 0xFF0000) |
						((int)((color & 0x00FF00) * shadowFactor) & 0x00FF00) |
						((int)((color & 0x0000FF) * shadowFactor) & 0x0000FF);
	draw_vertical_line(game, x, drawStart - 1, drawEnd, shadowedColor);
	return drawEnd;
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
		int	floorHit[MAP_FLOOR] = {0};
		float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
		float rayDirX = game->player->dirX + game->player->planeX * cameraX;
		float rayDirY = game->player->dirY + game->player->planeY * cameraX - sin(game->player->pitch);
		rayDirY += tan(game->player->pitch);

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

		while (!allFloorsHit(floorHit))
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			for (int floor = 0; floor < MAP_FLOOR; floor++)
			{
				if (map[floor][mapX][mapY] == '1')
				{
					floorHit[floor] = 1;
					draw_wall(game, x, mapX, mapY, stepX, stepY, rayDirX, rayDirY, side, (int)game->player->floor, floor);
				}
				// if (map[floor][mapX][mapY] == '0' && floor == 0)
				// 	draw_floor(game, x, mapX, mapY, stepX, stepY, rayDirX, rayDirY, side, (int)game->player->floor, floor);
			}
		}
	}
}

int	handle_keypress(int keycode, t_game *game)
{
	if (keycode == 65307) // Echap
		exit(0);
	if (keycode == 119) // W pour avancer
	{
		game->player->x += game->player->dirX * 0.1;
		game->player->y += game->player->dirY * 0.1;
	}
	if (keycode == 115) // S pour reculer
	{
		game->player->x -= game->player->dirX * 0.1;
		game->player->y -= game->player->dirY * 0.1;
	}
	if (keycode == 100) // D pour tourner à gauche
	{
		float oldDirX = game->player->dirX;
		game->player->dirX = oldDirX * cos(ROTATION_SPEED) - game->player->dirY * sin(ROTATION_SPEED);
		game->player->dirY = oldDirX * sin(ROTATION_SPEED) + game->player->dirY * cos(ROTATION_SPEED);
		float oldPlaneX = game->player->planeX;
		game->player->planeX = oldPlaneX * cos(ROTATION_SPEED) - game->player->planeY * sin(ROTATION_SPEED);
		game->player->planeY = oldPlaneX * sin(ROTATION_SPEED) + game->player->planeY * cos(ROTATION_SPEED);
	}
	if (keycode == 97) // A pour tourner à droite
	{
		float oldDirX = game->player->dirX;
		game->player->dirX = oldDirX * cos(-ROTATION_SPEED) - game->player->dirY * sin(-ROTATION_SPEED);
		game->player->dirY = oldDirX * sin(-ROTATION_SPEED) + game->player->dirY * cos(-ROTATION_SPEED);
		float oldPlaneX = game->player->planeX;
		game->player->planeX = oldPlaneX * cos(-ROTATION_SPEED) - game->player->planeY * sin(-ROTATION_SPEED);
		game->player->planeY = oldPlaneX * sin(-ROTATION_SPEED) + game->player->planeY * cos(-ROTATION_SPEED);
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
		mlx_clear_window(game->mlx, game->win);
		clear_image(game->img, 0x000000);
		cast_rays(game);
		mlx_put_image_to_window(game->mlx, game->win, game->img->img, 0, 0);
		return (0);
}

int main()
{
	t_game		game;

	game.mlx = mlx_init();

	game.player = malloc(sizeof(t_player));
	game.player->x = 6;
	game.player->y = 6;
	game.player->height = 0;
	game.player->dirX = 1;
	game.player->dirY = 0;
	game.player->planeX = 0;
	game.player->planeY = 0.66;
	game.player->floor = 0;
	game.win = mlx_new_window(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "Raycasting 3D");
	game.img = malloc(sizeof(t_image));
	game.img->img = mlx_new_image(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	game.img->data = mlx_get_data_addr(game.img->img, &game.img->bpp, &game.img->size_line, &game.img->endian);
	mlx_hook(game.win, 2, 1L << 0, handle_keypress, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	return 0;
}
