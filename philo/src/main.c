/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:41:59 by katakada          #+#    #+#             */
/*   Updated: 2025/03/29 20:24:59 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <time.h>

t_lltime	calc_optimal_interval_ms(void)
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

void	set_global_parameters(t_shared *s)
{
	s->g_s.num_of_philos = NUM_PHILOSOPHERS;
	s->g_s.survival_time_per_meal = SURVIVAL_TIME_PER_MEAL;
	s->g_s.eating_time = EATING_TIME_MS;
	s->g_s.sleeping_time = SLEEPING_TIME_MS;
	s->g_s.required_meals = REQUIRED_MEALS;
}

int	main(void)
{
	pthread_t		monitor_thread;
	t_shared		s;
	pthread_t		philo_thread[NUM_PHILOSOPHERS];
	pthread_mutex_t	forks[NUM_PHILOSOPHERS];
	bool			fork_in_use[NUM_PHILOSOPHERS];
	t_philo			philos[NUM_PHILOSOPHERS];
	bool			all_finished;

	// まずは人数を確定させる
	set_global_parameters(&s);
	// mallocでメモリ確保する
	s.philos = philos;
	s.g_s.fork_in_use = fork_in_use;
	pthread_mutex_init(&s.g_s.g_mutex, NULL);
	// barrierの初期化
	pthread_mutex_init(&s.g_s.barrier.b_mutex, NULL);
	s.g_s.barrier.thread_count = NUM_PHILOSOPHERS + NUM_MONITOR_THREAD;
	s.g_s.barrier.arrived_count = 0;
	// monitorの初期化
	pthread_mutex_init(&s.g_s.monitor.m_mutex, NULL);
	s.g_s.monitor.is_finished = false;
	// blobalパラメータの初期化
	s.g_s.meal_interval_time = calc_optimal_interval_ms();
	s.g_s.start_time = 0;
	s.g_s.prioryty_wait_time = s.g_s.eating_time / 4;
	s.g_s.wait_threshold_time = s.g_s.survival_time_per_meal / 3;
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&s.philos[i].p_mutex, NULL);
		s.philos[i].id = i;
		s.philos[i].state = THINKING;
		s.philos[i].meals_eaten = 0;
		s.philos[i].wait_start_us = 0;
		s.philos[i].left_fork = &forks[left_fork(i)];
		s.philos[i].right_fork = &forks[right_fork(i)];
		s.philos[i].g_s = &s.g_s;
		s.philos[i].other_philos = s.philos;
	}
	pthread_create(&monitor_thread, NULL, monitor_routine, &s);
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		pthread_create(&philo_thread[i], NULL, philosopher_routine,
			&s.philos[i]);
	}
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		pthread_join(philo_thread[i], NULL);
	}
	pthread_join(monitor_thread, NULL);
	// 独自のテストブロック
	all_finished = true;
	if (REQUIRED_MEALS > 0)
	{
		for (int i = 0; i < NUM_PHILOSOPHERS; i++)
		{
			if (s.philos[i].meals_eaten < REQUIRED_MEALS)
			{
				all_finished = false;
				break ;
			}
		}
		if (all_finished)
		{
			printf("All philosophers have finished their meals successfully!\n");
		}
		else
		{
			printf("Some philosophers couldn't finish their meals.\n");
		}
	}
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		pthread_mutex_destroy(&s.philos[i].p_mutex);
		pthread_mutex_destroy(&forks[i]);
	}
	pthread_mutex_destroy(&s.g_s.monitor.m_mutex);
	pthread_mutex_destroy(&s.g_s.barrier.b_mutex);
	pthread_mutex_destroy(&s.g_s.g_mutex);
	return (0);
}
