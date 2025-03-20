/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:27:51 by katakada          #+#    #+#             */
/*   Updated: 2025/03/20 20:34:29 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// static int	sum(int a, int b)
// {
// 	return (a + b);
// }

typedef struct
{
	pthread_mutex_t	mutex;
	int				thread_count;
	int				arrived_count;
}					barrier_t;

void	barrier_init(barrier_t *barrier, int thread_count)
{
	pthread_mutex_init(&barrier->mutex, NULL);
	barrier->thread_count = thread_count;
	barrier->arrived_count = 0;
}

void	barrier_destroy(barrier_t *barrier)
{
	pthread_mutex_destroy(&barrier->mutex);
}

void	barrier_wait(barrier_t *barrier)
{
	pthread_mutex_lock(&barrier->mutex);
	barrier->arrived_count++;
	pthread_mutex_unlock(&barrier->mutex);
	while (1)
	{
		pthread_mutex_lock(&barrier->mutex);
		if (barrier->arrived_count == barrier->thread_count)
		{
			pthread_mutex_unlock(&barrier->mutex);
			break ;
		}
		pthread_mutex_unlock(&barrier->mutex);
	}
}

void	*thread_function(void *arg)
{
	barrier_t	*barrier;

	barrier = (barrier_t *)arg;
	printf("Time%lu Thread %lu arrived at the barrier.\n", (long)clock(),
		pthread_self());
	barrier_wait(barrier);
	printf("Time%lu Thread %lu passed the barrier.\n", (long)clock(),
		pthread_self());
	return (NULL);
}

int	app_main(int argc, char **argv)
{
	int			thread_count;
	pthread_t	threads[thread_count];
	barrier_t	barrier;

	thread_count = 3;
	(void)argc;
	(void)argv;
	barrier_init(&barrier, thread_count);
	for (int i = 0; i < thread_count; i++)
	{
		pthread_create(&threads[i], NULL, thread_function, &barrier);
	}
	for (int i = 0; i < thread_count; i++)
	{
		pthread_join(threads[i], NULL);
	}
	barrier_destroy(&barrier);
	return (0);
}

#ifndef TEST

int	main(int argc, char *argv[])
{
	return (app_main(argc, argv));
}
#endif // TEST
