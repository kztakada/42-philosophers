/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:41:59 by katakada          #+#    #+#             */
/*   Updated: 2025/03/31 01:08:43 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <time.h>

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

	if (parse_argv(&s, argc, argv) == FALSE)
		return (1);
	if (init_memory_space(&s, &philo_thread, &forks) == FALSE)
		return (put_error(MALLOC_ERR_MSG), 1);
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
// 			all_finished = FALSE;
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