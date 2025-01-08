/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:15:54 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/02 20:16:11 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
