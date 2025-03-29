/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 00:13:04 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 00:29:45 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	init_memory_space(t_shared *s, pthread_t **philo_thread,
		pthread_mutex_t **forks)
{
	s->philos = malloc(sizeof(t_philo) * s->g_s.num_of_philos);
	if (s->philos == NULL)
		return (false);
	s->g_s.fork_in_use = malloc(sizeof(bool) * s->g_s.num_of_philos);
	if (s->g_s.fork_in_use == NULL)
	{
		free(s->philos);
		return (false);
	}
	*philo_thread = malloc(sizeof(pthread_t) * s->g_s.num_of_philos);
	if (philo_thread == NULL)
	{
		free(s->philos);
		free(s->g_s.fork_in_use);
		return (false);
	}
	*forks = malloc(sizeof(pthread_mutex_t) * s->g_s.num_of_philos);
	if (forks == NULL)
	{
		free(s->philos);
		free(s->g_s.fork_in_use);
		free(philo_thread);
		return (false);
	}
	return (true);
}

void	init_philos(t_shared *s, pthread_mutex_t *forks)
{
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&s->philos[i].p_mutex, NULL);
		s->philos[i].id = i;
		s->philos[i].state = THINKING;
		s->philos[i].meals_eaten = 0;
		s->philos[i].wait_start_us = 0;
		s->philos[i].left_fork = &forks[left_fork(i)];
		s->philos[i].right_fork = &forks[right_fork(i)];
		s->philos[i].g_s = &s->g_s;
		s->philos[i].other_philos = s->philos;
	}
}

static t_lltime	calc_optimal_interval_ms(void)
{
	const int	n = NUM_PHILOSOPHERS;
	const int	same_time_max_eat = NUM_PHILOSOPHERS / 2;
	const int	offset_unit_time = EATING_TIME_MS / same_time_max_eat;
	const int	required_offset_time = offset_unit_time * n;

	// 人数が0か1の場合＝待機時間0
	if (same_time_max_eat == 0)
		return ((EATING_TIME_MS + SLEEPING_TIME_MS));
	// 人数が2以上の場合＝待機時間必要
	if (required_offset_time < (EATING_TIME_MS + SLEEPING_TIME_MS))
		return (EATING_TIME_MS + SLEEPING_TIME_MS);
	else
		return (required_offset_time);
}

void	setup_global_params(t_shared *s)
{
	pthread_mutex_init(&s->g_s.g_mutex, NULL);
	// barrierの初期化
	pthread_mutex_init(&s->g_s.barrier.b_mutex, NULL);
	s->g_s.barrier.thread_count = NUM_PHILOSOPHERS + NUM_MONITOR_THREAD;
	s->g_s.barrier.arrived_count = 0;
	// monitorの初期化
	pthread_mutex_init(&s->g_s.monitor.m_mutex, NULL);
	s->g_s.monitor.is_finished = false;
	// blobalパラメータの初期化
	s->g_s.meal_interval_time = calc_optimal_interval_ms();
	s->g_s.start_time = 0;
	s->g_s.prioryty_wait_time = s->g_s.eating_time / 4;
	s->g_s.wait_threshold_time = s->g_s.survival_time_per_meal / 3;
}
