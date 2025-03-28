/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:47:03 by katakada          #+#    #+#             */
/*   Updated: 2025/03/29 00:20:06 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	reserve_forks(t_philo *philo)
{
	int		philo_id;
	bool	*fork_in_use;

	philo_id = philo->id;
	fork_in_use = philo->g_s->fork_in_use;
	pthread_mutex_lock(&philo->g_s->g_mutex);
	if (!fork_in_use[left_fork(philo_id)] && !fork_in_use[right_fork(philo_id)])
	{
		fork_in_use[left_fork(philo_id)] = true;
		fork_in_use[right_fork(philo_id)] = true;
		pthread_mutex_unlock(&philo->g_s->g_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->g_s->g_mutex);
	return (false);
}

void	unreseve_forks(t_philo *philo)
{
	int		philo_id;
	bool	*fork_in_use;

	philo_id = philo->id;
	fork_in_use = philo->g_s->fork_in_use;
	pthread_mutex_lock(&philo->g_s->g_mutex);
	fork_in_use[left_fork(philo_id)] = false;
	fork_in_use[right_fork(philo_id)] = false;
	pthread_mutex_unlock(&philo->g_s->g_mutex);
}
