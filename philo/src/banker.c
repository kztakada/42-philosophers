/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:47:03 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 01:54:44 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	reserve_forks(t_philo *philo)
{
	bool	*fork_in_use;

	fork_in_use = philo->g_s->fork_in_use;
	pthread_mutex_lock(&philo->g_s->g_mutex);
	if (!fork_in_use[left_fork(philo)] && !fork_in_use[right_fork(philo)])
	{
		fork_in_use[left_fork(philo)] = true;
		fork_in_use[right_fork(philo)] = true;
		pthread_mutex_unlock(&philo->g_s->g_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->g_s->g_mutex);
	return (false);
}

void	unreseve_forks(t_philo *philo)
{
	bool	*fork_in_use;

	fork_in_use = philo->g_s->fork_in_use;
	pthread_mutex_lock(&philo->g_s->g_mutex);
	fork_in_use[left_fork(philo)] = false;
	fork_in_use[right_fork(philo)] = false;
	pthread_mutex_unlock(&philo->g_s->g_mutex);
}
