/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithread.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 19:09:07 by ybeaucou          #+#    #+#             */
/*   Updated: 2025/01/10 09:25:10 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTITHREAD_H
# define MULTITHREAD_H

# include <pthread.h>
# include "cub3d.h"

typedef enum e_task_type
{
	RAYCAST,
	CAST_FLOOR,
	SPRITE,
	HEALTH,
	FILTER_RED,
}	t_task_type;

typedef struct s_task_sprite
{
	t_image		*texture;
	t_sprite	*sprite;
	float		sprite_dir;
	float		scale;
	float		z_offset;
}	t_task_sprite;

typedef struct s_task
{
	int				x;
	t_game			*game;
	t_raycast		*raycast;
	t_floorcast		*floorcast;
	t_task_type		type;
	t_task_sprite	*sprite;
	struct s_task	*next;
}	t_task;

typedef struct s_thread_pool
{
	int				num_threads;
	int				tasks_remaining;
	int				shutdown;
	t_task			*task_queue;
	t_task			*task_queue_tail;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	queue_cond;
	pthread_cond_t	all_tasks_done_cond;
	pthread_t		*threads;
	t_game			*game;
}	t_thread_pool;

void	*worker_thread(void *arg);
void	create_task(t_game *game, int x, t_task_type type);
void	render_multithreaded(t_game *game);
void	init_thread_pool(t_game *game, int num_threads);
void	free_all_pool(t_game *game);
void	create_threads(t_game *game, t_thread_pool *pool, int num_threads);
void	destroy_thread_pool(t_thread_pool	*pool);

#endif