/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:23:37 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/09 10:08:57 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	create_task(t_game *game, int x, t_task_type type)
{
	t_task	*task;

	task = (t_task *)gc_malloc(game->mem, sizeof(t_task));
	task->x = x;
	task->game = game;
	task->next = NULL;
	if (type == RAYCAST)
		task->raycast = gc_malloc(game->mem, sizeof(t_raycast));
	else if (type == CAST_FLOOR)
		task->floorcast = gc_malloc(game->mem, sizeof(t_floorcast));
	task->type = type;
	pthread_mutex_lock(&game->pool->queue_mutex);
	if (game->pool->task_queue == NULL)
	{
		game->pool->task_queue = task;
		game->pool->task_queue_tail = task;
	}
	else
	{
		game->pool->task_queue_tail->next = task;
		game->pool->task_queue_tail = task;
	}
	game->pool->tasks_remaining++;
	pthread_mutex_unlock(&game->pool->queue_mutex);
}

void	wait_for_all_tasks(t_thread_pool *pool)
{
	pthread_mutex_lock(&pool->queue_mutex);
	while (pool->tasks_remaining > 0)
		pthread_cond_wait(&pool->all_tasks_done_cond, &pool->queue_mutex);
	pthread_mutex_unlock(&pool->queue_mutex);
}

void	render_multithreaded(t_game *game)
{
	int	x;

	x = -1;
	while (++x < game->screen_width)
		create_task(game, x, RAYCAST);
	x = -1;
	while (++x < game->screen_height)
		create_task(game, x, CAST_FLOOR);
	x = -1;
	while (++x < game->screen_height)
		create_task(game, x, FILTER_RED);
	pthread_mutex_lock(&game->pool->queue_mutex);
	pthread_cond_broadcast(&game->pool->queue_cond);
	pthread_mutex_unlock(&game->pool->queue_mutex);
	wait_for_all_tasks(game->pool);
	free_all_pool(game);
	draw_sprites(game);
	chat_draw(game);
	show_message(game);
	draw_hud(game);
}

void	create_threads(t_game *game, t_thread_pool *pool, int num_threads)
{
	int	i;

	i = -1;
	while (++i < num_threads)
	{
		if (pthread_create(&pool->threads[i], NULL, worker_thread, pool) != 0)
		{
			game->is_running = false;
			gc_exit(game->mem, 1);
			return ;
		}
	}
}
