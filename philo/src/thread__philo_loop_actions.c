/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo_loop_actions.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:26:42 by katakada          #+#    #+#             */
/*   Updated: 2025/03/31 01:06:35 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_bool	done_thinking(t_philo *philo)
{
	if (print_log_if_alive(philo, "is thinking") == FALSE)
		return (FALSE);
	sleep_until_next_mealtime(philo->next_meal_time);
	while (try_to_eat(philo) == FALSE)
	{
		if (safe_is_finished(philo->g_s))
			return (FALSE);
		usleep(RETRAY_TIME_US);
	}
	pthread_mutex_lock(&philo->p_mutex);
	philo->state = EATING;
	pthread_mutex_unlock(&philo->p_mutex);
	return (TRUE);
}

static void	finish_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->p_mutex);
	philo->meals_eaten++;
	philo->next_meal_time += philo->g_s->meal_interval_time;
	pthread_mutex_unlock(&philo->p_mutex);
	unreseve_forks(philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	done_eating(t_philo *philo)
{
	sleep_from_now(philo->g_s->eating_time);
	finish_eating(philo);
	pthread_mutex_lock(&philo->p_mutex);
	philo->state = SLEEPING;
	pthread_mutex_unlock(&philo->p_mutex);
}

t_bool	is_still_hungry(t_philo *philo)
{
	pthread_mutex_lock(&philo->p_mutex);
	if (philo->g_s->required_meals > 0
		&& philo->meals_eaten >= philo->g_s->required_meals)
	{
		philo->wait_start_us = 0;
		pthread_mutex_unlock(&philo->p_mutex);
		return (FALSE);
	}
	pthread_mutex_unlock(&philo->p_mutex);
	return (TRUE);
}

t_bool	done_sleeping(t_philo *philo)
{
	if (print_log_if_alive(philo, "is sleeping") == FALSE)
		return (FALSE);
	sleep_from_now(philo->g_s->sleeping_time);
	pthread_mutex_lock(&philo->p_mutex);
	philo->state = THINKING;
	pthread_mutex_unlock(&philo->p_mutex);
	return (TRUE);
}
