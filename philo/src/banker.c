/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:47:03 by katakada          #+#    #+#             */
/*   Updated: 2025/03/31 01:00:58 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_bool	reserve_forks(t_philo *philo)
{
	t_bool	*fork_in_use;

	fork_in_use = philo->g_s->fork_in_use;
	pthread_mutex_lock(&philo->g_s->g_mutex);
	if (!fork_in_use[left_fork(philo)] && !fork_in_use[right_fork(philo)])
	{
		fork_in_use[left_fork(philo)] = TRUE;
		fork_in_use[right_fork(philo)] = TRUE;
		pthread_mutex_unlock(&philo->g_s->g_mutex);
		return (TRUE);
	}
	pthread_mutex_unlock(&philo->g_s->g_mutex);
	return (FALSE);
}

void	unreseve_forks(t_philo *philo)
{
	t_bool	*fork_in_use;

	fork_in_use = philo->g_s->fork_in_use;
	pthread_mutex_lock(&philo->g_s->g_mutex);
	fork_in_use[left_fork(philo)] = FALSE;
	fork_in_use[right_fork(philo)] = FALSE;
	pthread_mutex_unlock(&philo->g_s->g_mutex);
}
