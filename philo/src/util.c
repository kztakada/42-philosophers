/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:54:09 by katakada          #+#    #+#             */
/*   Updated: 2025/04/08 15:37:53 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	left_fork(t_philo *philo)
{
	return (philo->id);
}

int	right_fork(t_philo *philo)
{
	return ((philo->id + 1) % philo->g_s->num_of_philos);
}

pthread_mutex_t	*fork_1st(t_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
		return (philo->left_fork);
	else
		return (philo->right_fork);
}

pthread_mutex_t	*fork_2nd(t_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
		return (philo->right_fork);
	else
		return (philo->left_fork);
}
