/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:41:59 by katakada          #+#    #+#             */
/*   Updated: 2025/03/28 00:32:11 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <time.h>

// global variables
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;
bool			fork_in_use[NUM_PHILOSOPHERS] = {false};
bool			is_finished = false;
t_lltime		start_time = 0;
////////////////////////

t_lltime	calc_optimal_interval_ms(void)
{
	const int	n = NUM_PHILOSOPHERS;
	const int	k = NUM_PHILOSOPHERS / 2;
	int			interval;

	if (k == 0)
		return ((EATING_TIME_MS + SLEEPING_TIME_MS));
	interval = (EATING_TIME_MS)*n / k;
	if (interval < (EATING_TIME_MS + SLEEPING_TIME_MS))
		interval = (EATING_TIME_MS + SLEEPING_TIME_MS);
	return (interval);
}

int	main(void)
{
	bool	all_finished;

	pthread_mutex_init(&barrier.b_mutex, NULL);
	barrier.thread_count = NUM_PHILOSOPHERS + 1;
	barrier.arrived_count = 0;
	pthread_mutex_init(&m_mutex, NULL);
	meal_interval_time = calc_optimal_interval_ms();
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&p_mutex[i], NULL);
		philosophers[i].id = i;
		philosophers[i].state = THINKING;
		philosophers[i].meals_eaten = 0;
		philosophers[i].wait_start_us = 0;
		philosophers[i].left_fork = &forks[i];
		philosophers[i].right_fork = &forks[(i + 1) % NUM_PHILOSOPHERS];
		philosophers[i].p_mutex = &p_mutex[i];
	}
	pthread_create(&monitor_thread, NULL, monitor_routine, NULL);
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		pthread_create(&philosophers[i].thread, NULL, philosopher_routine,
			&philosophers[i]);
	}
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		pthread_join(philosophers[i].thread, NULL);
	}
	pthread_join(monitor_thread, NULL);
	all_finished = true;
	if (REQUIRED_MEALS > 0)
	{
		for (int i = 0; i < NUM_PHILOSOPHERS; i++)
		{
			if (philosophers[i].meals_eaten < REQUIRED_MEALS)
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
		pthread_mutex_destroy(&p_mutex[i]);
		pthread_mutex_destroy(&forks[i]);
	}
	pthread_mutex_destroy(&m_mutex);
	pthread_mutex_destroy(&barrier.b_mutex);
	pthread_mutex_destroy(&g_mutex);
	return (0);
}
