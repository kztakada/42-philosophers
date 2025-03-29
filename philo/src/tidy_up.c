/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tidy_up.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 00:19:25 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 00:39:46 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	all_mutex_destroy(t_shared *s, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < s->g_s.num_of_philos)
	{
		pthread_mutex_destroy(&s->philos[i].p_mutex);
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&s->g_s.monitor.m_mutex);
	pthread_mutex_destroy(&s->g_s.barrier.b_mutex);
	pthread_mutex_destroy(&s->g_s.g_mutex);
}

void	all_free(t_shared *s, pthread_mutex_t *forks, pthread_t *philo_thread)
{
	free(s->philos);
	free(s->g_s.fork_in_use);
	free(philo_thread);
	free(forks);
}
