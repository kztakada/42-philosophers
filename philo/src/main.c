/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:41:59 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 00:36:38 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <time.h>

bool	parse_argv(t_shared *s, char *argv[])
{
	(void)argv;
	s->g_s.num_of_philos = NUM_PHILOSOPHERS;
	s->g_s.survival_time_per_meal = SURVIVAL_TIME_PER_MEAL;
	s->g_s.eating_time = EATING_TIME_MS;
	s->g_s.sleeping_time = SLEEPING_TIME_MS;
	s->g_s.required_meals = REQUIRED_MEALS;
	return (true);
}

void	create_threads(t_shared *s, pthread_t *monitor_thread,
		pthread_t *philo_thread)
{
	int	i;

	pthread_create(monitor_thread, NULL, monitor_routine, s);
	i = 0;
	while (i < s->g_s.num_of_philos)
	{
		pthread_create(&philo_thread[i], NULL, philosopher_routine,
			&s->philos[i]);
		i++;
	}
	i = 0;
	while (i < s->g_s.num_of_philos)
	{
		pthread_join(philo_thread[i], NULL);
		i++;
	}
	pthread_join(*monitor_thread, NULL);
}

int	app_main(int argc, char *argv[])
{
	t_shared		s;
	pthread_mutex_t	*forks;
	pthread_t		monitor_thread;
	pthread_t		*philo_thread;

	(void)argc;
	if (parse_argv(&s, argv) == false)
		return (1);
	if (init_memory_space(&s, &philo_thread, &forks) == false)
	{
		printf("malloc error\n");
		return (1);
	}
	setup_global_params(&s);
	init_philos(&s, forks);
	create_threads(&s, &monitor_thread, philo_thread);
	all_mutex_destroy(&s, forks);
	all_free(&s, forks, philo_thread);
	return (0);
}

#ifndef TEST

int	main(int argc, char *argv[])
{
	return (app_main(argc, argv));
}
#endif // TEST

// // 独自のテストブロック
// all_finished = true;
// if (REQUIRED_MEALS > 0)
// {
// 	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
// 	{
// 		if (s.philos[i].meals_eaten < REQUIRED_MEALS)
// 		{
// 			all_finished = false;
// 			break ;
// 		}
// 	}
// 	if (all_finished)
// 	{
// 		printf("All philosophers have finished their meals successfully!\n");
// 	}
// 	else
// 	{
// 		printf("Some philosophers couldn't finish their meals.\n");
// 	}
// }