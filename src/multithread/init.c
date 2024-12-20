/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:23:37 by ybeaucou          #+#    #+#             */
/*   Updated: 2024/11/29 17:20:49 by ybeaucou         ###   ########.fr       */
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

void	worker_use_task(t_thread_pool *pool, t_task *task)
{
	pthread_mutex_unlock(&pool->queue_mutex);
	if (task != NULL)
	{
		if (task->type == RAYCAST)
		{
			task->raycast->x = task->x;
			cast_rays(task->game, task->raycast);
		}
		else if (task->type == CAST_FLOOR)
		{
			task->floorcast->y = task->x;
			cast_floor(task->game, task->floorcast);
		}
		else if (task->type == FILTER_RED)
			damages_red_draw(task->game, task->x);
	}
	pthread_mutex_lock(&pool->queue_mutex);
}

void	worker_send_signal(t_thread_pool *pool)
{
	if (pool->tasks_remaining == 0)
		pthread_cond_broadcast(&pool->all_tasks_done_cond);
}

void	*worker_thread(void *arg)
{
	t_thread_pool	*pool;
	t_task			*task;

	pool = (t_thread_pool *)arg;
	while (pool->game->is_running)
	{
		pthread_mutex_lock(&pool->queue_mutex);
		while (pool->task_queue == NULL && !pool->shutdown)
			pthread_cond_wait(&pool->queue_cond, &pool->queue_mutex);
		if (pool->shutdown && pool->task_queue == NULL)
		{
			pthread_mutex_unlock(&pool->queue_mutex);
			break ;
		}
		task = pool->task_queue;
		if (task != NULL)
		{
			pool->task_queue = task->next;
			pool->tasks_remaining--;
		}
		worker_use_task(pool, task);
		worker_send_signal(pool);
		pthread_mutex_unlock(&pool->queue_mutex);
	}
	return (NULL);
}

void	destroy_thread_pool(t_block_info *param)
{
	t_thread_pool	*pool;
	int				i;

	pool = (t_thread_pool *)param->ptr;
	pthread_mutex_lock(&pool->queue_mutex);
	pool->shutdown = 1;
	pthread_cond_broadcast(&pool->queue_cond);
	pthread_mutex_unlock(&pool->queue_mutex);
	i = -1;
	while (++i < pool->num_threads)
		pthread_join(pool->threads[i], NULL);
	pthread_mutex_destroy(&pool->queue_mutex);
	pthread_cond_destroy(&pool->queue_cond);
	pthread_cond_destroy(&pool->all_tasks_done_cond);
	gc_free(pool->game->mem, pool->threads);
	gc_free(pool->game->mem, pool);
}

void	wait_for_all_tasks(t_thread_pool *pool)
{
	pthread_mutex_lock(&pool->queue_mutex);
	while (pool->tasks_remaining > 0)
		pthread_cond_wait(&pool->all_tasks_done_cond, &pool->queue_mutex);
	pthread_mutex_unlock(&pool->queue_mutex);
}

void	free_all_pool(t_game *game)
{
	t_task	*task;
	t_task	*next;

	pthread_mutex_lock(&game->pool->queue_mutex);
	task = game->pool->task_queue;
	while (task)
	{
		next = task->next;
		if (task->type == SPRITE)
		{
			gc_free(game->mem, task->sprite->sprite);
			gc_free(game->mem, task->sprite);
		}
		else if (task->type == HEALTH)
			gc_free(game->mem, task->sprite);
		else if (task->type == CAST_FLOOR)
			gc_free(game->mem, task->floorcast);
		else if (task->type == RAYCAST)
			gc_free(game->mem, task->raycast);
		gc_free(game->mem, task);
		task = next;
	}
	game->pool->task_queue = NULL;
	game->pool->task_queue_tail = NULL;
	pthread_mutex_unlock(&game->pool->queue_mutex);
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
			perror("pthread_create");
			game->is_running = false;
			gc_exit(game->mem, 1);
			return ;
		}
	}
}

void	init_thread_pool(t_game *game, int num_threads)
{
	t_thread_pool	*pool;
	t_block_info	*param;

	pool = gc_malloc(game->mem, sizeof(t_thread_pool));
	pool->num_threads = num_threads;
	pool->task_queue = NULL;
	pool->tasks_remaining = 0;
	pool->game = game;
	pool->shutdown = 0;
	pthread_mutex_init(&pool->queue_mutex, NULL);
	pthread_cond_init(&pool->queue_cond, NULL);
	pthread_cond_init(&pool->all_tasks_done_cond, NULL);
	pool->threads = gc_malloc(game->mem, sizeof(pthread_t) * num_threads);
	create_threads(game, pool, num_threads);
	game->pool = pool;
	param = gc_malloc(pool->game->mem, sizeof(t_block_info));
	param->ptr = pool;
	gc_add_memory_block(pool->game->mem, NULL, destroy_thread_pool, param);
	gc_free(pool->game->mem, param);
}
