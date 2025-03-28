/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:47:03 by katakada          #+#    #+#             */
/*   Updated: 2025/03/28 12:20:21 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	reserve_forks(int philo_id)
{
	pthread_mutex_lock(&g_mutex);
	if (!fork_in_use[left_fork(philo_id)] && !fork_in_use[right_fork(philo_id)])
	{
		fork_in_use[left_fork(philo_id)] = true;
		fork_in_use[right_fork(philo_id)] = true;
		pthread_mutex_unlock(&g_mutex);
		return (true);
	}
	pthread_mutex_unlock(&g_mutex);
	return (false);
}

void	unreseve_forks(int philo_id)
{
	pthread_mutex_lock(&g_mutex);
	fork_in_use[left_fork(philo_id)] = false;
	fork_in_use[right_fork(philo_id)] = false;
	pthread_mutex_unlock(&g_mutex);
}
