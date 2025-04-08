/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 00:13:04 by katakada          #+#    #+#             */
/*   Updated: 2025/04/08 16:05:51 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_bool	init_memory_space(t_shared *s, pthread_t **philo_thread,
		pthread_mutex_t **forks)
{
	s->philos = malloc(sizeof(t_philo) * s->g_s.num_of_philos);
	if (s->philos == NULL)
		return (FALSE);
	s->g_s.fork_in_use = malloc(sizeof(t_bool) * s->g_s.num_of_philos);
	if (s->g_s.fork_in_use == NULL)
	{
		free(s->philos);
		return (FALSE);
	}
	*philo_thread = malloc(sizeof(pthread_t) * s->g_s.num_of_philos);
	if (philo_thread == NULL)
	{
		free(s->philos);
		free(s->g_s.fork_in_use);
		return (FALSE);
	}
	*forks = malloc(sizeof(pthread_mutex_t) * s->g_s.num_of_philos);
	if (forks == NULL)
	{
		free(s->philos);
		free(s->g_s.fork_in_use);
		free(philo_thread);
		return (FALSE);
	}
	return (TRUE);
}

void	init_philos(t_shared *s, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < s->g_s.num_of_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&s->philos[i].p_mutex, NULL);
		s->philos[i].g_s = &s->g_s;
		s->philos[i].id = i;
		s->philos[i].state = THINKING;
		s->philos[i].meals_eaten = 0;
		s->philos[i].wait_start_us = 0;
		s->philos[i].last_sleep_start_time = 0;
		s->philos[i].left_fork = &forks[left_fork(&s->philos[i])];
		s->philos[i].right_fork = &forks[right_fork(&s->philos[i])];
		s->philos[i].other_philos = s->philos;
		s->g_s.fork_in_use[i] = FALSE;
		i++;
	}
}

static t_lltime	calc_wait_threshold_time_us(t_shared *s)
{
	const int	n = s->g_s.num_of_philos;
	const int	same_time_max_eat = n / 2;
	int			offset_unit_time;
	int			required_offset_time;

	if (same_time_max_eat == 0)
		return (0);
	offset_unit_time = s->g_s.eating_time / same_time_max_eat;
	required_offset_time = offset_unit_time * n;
	if (required_offset_time < (s->g_s.eating_time + s->g_s.sleeping_time))
		return (0);
	else
		return (offset_unit_time * 1000);
}

static t_lltime	calc_optimal_interval_ms(t_shared *s)
{
	const int	n = s->g_s.num_of_philos;
	const int	same_time_max_eat = s->g_s.num_of_philos / 2;
	int			offset_unit_time;
	int			required_offset_time;

	// 人数が0か1の場合＝待機時間0
	if (same_time_max_eat == 0)
		return ((s->g_s.eating_time + s->g_s.sleeping_time));
	offset_unit_time = s->g_s.eating_time / same_time_max_eat;
	required_offset_time = offset_unit_time * n;
	// 人数が2以上の場合＝待機時間必要
	if (required_offset_time < (s->g_s.eating_time + s->g_s.sleeping_time))
		return (s->g_s.eating_time + s->g_s.sleeping_time);
	else
		return (required_offset_time);
}

void	setup_global_params(t_shared *s)
{
	pthread_mutex_init(&s->g_s.g_mutex, NULL);
	// barrierの初期化
	pthread_mutex_init(&s->g_s.barrier.b_mutex, NULL);
	s->g_s.barrier.thread_count = s->g_s.num_of_philos + NUM_MONITOR_THREAD;
	s->g_s.barrier.arrived_count = 0;
	// monitorの初期化
	pthread_mutex_init(&s->g_s.monitor.m_mutex, NULL);
	s->g_s.monitor.is_finished = FALSE;
	// blobalパラメータの初期化
	s->g_s.meal_interval_time = calc_optimal_interval_ms(s);
	s->g_s.wait_threshold_us = calc_wait_threshold_time_us(s);
	s->g_s.start_time = 0;
}
