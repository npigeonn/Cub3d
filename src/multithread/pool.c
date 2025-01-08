/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:16:27 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/03 16:48:36 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	destroy_thread_pool(t_block_info *param)
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
