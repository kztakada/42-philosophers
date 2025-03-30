/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo__try_to_eat.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:57:52 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 20:32:05 by katakada         ###   ########.fr       */
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

bool	take_forks(t_philo *philo)
{
	if (philo->g_s->num_of_philos == 1)
	{
		take_one_fork_to_die(philo);
		return (false);
	}
	else
	{
		// 1本目を取得する
		pthread_mutex_lock(philo->left_fork);
		if (print_log_if_alive(philo, "has taken a fork") == false)
		{
			pthread_mutex_unlock(philo->left_fork);
			return (false);
		}
		// 両方のフォークを取得できた場合
		pthread_mutex_lock(philo->right_fork);
		if (print_log_if_alive(philo, "has taken a fork") == false)
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (false);
		}
		return (true);
	}
}

bool	start_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->p_mutex);
	philo->state = EATING;
	philo->wait_start_us = 0;
	philo->last_meal_satart_time = get_time_in_ms();
	pthread_mutex_unlock(&philo->p_mutex);
	if (print_log_if_alive(philo, "is eating") == false)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (false);
	}
	return (true);
}

bool	try_to_eat(t_philo *philo)
{
	bool	result;

	// 優先度確認ブロック
	if (has_first_priority(philo) == false)
		return (false);
	result = false;
	// フォーク取得ブロック
	if (reserve_forks(philo))
		result = take_forks(philo);
	if (result == true)
		result = start_eating(philo);
	return (result);
}
