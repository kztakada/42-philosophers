/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo__try_to_eat.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:57:52 by katakada          #+#    #+#             */
/*   Updated: 2025/03/31 01:06:30 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_one_fork_to_die(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_log_if_alive(philo, "has taken a fork");
	pthread_mutex_unlock(philo->left_fork);
	while (!safe_is_finished(philo->g_s))
		usleep(RETRAY_TIME_US);
}

t_bool	take_forks(t_philo *philo)
{
	if (philo->g_s->num_of_philos == 1)
	{
		take_one_fork_to_die(philo);
		return (FALSE);
	}
	else
	{
		// 1本目を取得する
		pthread_mutex_lock(philo->left_fork);
		if (print_log_if_alive(philo, "has taken a fork") == FALSE)
		{
			pthread_mutex_unlock(philo->left_fork);
			return (FALSE);
		}
		// 両方のフォークを取得できた場合
		pthread_mutex_lock(philo->right_fork);
		if (print_log_if_alive(philo, "has taken a fork") == FALSE)
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (FALSE);
		}
		return (TRUE);
	}
}

t_bool	start_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->p_mutex);
	philo->state = EATING;
	philo->wait_start_us = 0;
	philo->last_meal_satart_time = get_time_in_ms();
	pthread_mutex_unlock(&philo->p_mutex);
	if (print_log_if_alive(philo, "is eating") == FALSE)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (FALSE);
	}
	return (TRUE);
}

t_bool	try_to_eat(t_philo *philo)
{
	t_bool	result;

	// 優先度確認ブロック
	if (has_first_priority(philo) == FALSE)
		return (FALSE);
	result = FALSE;
	// フォーク取得ブロック
	if (reserve_forks(philo))
		result = take_forks(philo);
	if (result == TRUE)
		result = start_eating(philo);
	return (result);
}
