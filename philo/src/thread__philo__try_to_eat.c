/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo__try_to_eat.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:57:52 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 17:58:02 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	get_forks(t_philo *philo)
{
	if (philo->g_s->num_of_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_log_if_alive(philo, "has taken a fork");
		pthread_mutex_unlock(philo->left_fork);
		while (!safe_is_finished(philo->g_s))
			usleep(RETRAY_TIME_US);
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
			pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
			return (false);
		}
		pthread_mutex_lock(&philo->p_mutex);
		philo->state = EATING;
		philo->wait_start_us = 0;
		philo->last_meal_satart_time = get_time_in_ms();
		pthread_mutex_unlock(&philo->p_mutex);
		if (print_log_if_alive(philo, "is eating") == false)
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
			return (false);
		}
		pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
		return (true);
	}
}

bool	try_to_eat(t_philo *philo)
{
	bool		result;
	t_lltime	current_time_us;
	t_lltime	max_wait;
	int			priority_philo;
	t_lltime	wait_time;

	result = false;
	pthread_mutex_lock(&philo->p_mutex);
	if (philo->wait_start_us == 0)
	{
		philo->wait_start_us = get_time_in_us();
	}
	pthread_mutex_lock(&philo->g_s->monitor.m_mutex);
	current_time_us = m_unsafe_get_last_alive_time_us(philo);
	pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
	if (current_time_us == -1)
	{
		pthread_mutex_unlock(&philo->p_mutex);
		return (false);
	}
	pthread_mutex_unlock(&philo->p_mutex);
	// 優先度確認ブロック
	max_wait = 0;
	priority_philo = -1;
	for (int i = 0; i < philo->g_s->num_of_philos; i++)
	{
		if (philo->other_philos[i].state == THINKING
			&& philo->other_philos[i].wait_start_us != 0)
		{
			wait_time = current_time_us - philo->other_philos[i].wait_start_us;
			if (wait_time > max_wait)
			{
				max_wait = wait_time;
				priority_philo = i;
			}
		}
	}
	if (max_wait > philo->g_s->wait_threshold_time && priority_philo != -1
		&& priority_philo != philo->id
		&& (left_philo_id(&philo->other_philos[priority_philo]) == philo->id
			|| right_philo_id(&philo->other_philos[priority_philo]) == philo->id))
	{
		sleep_from_now(philo->g_s->prioryty_wait_time);
		return (false);
	}
	// フォーク取得ブロック
	if (reserve_forks(philo))
		result = get_forks(philo);
	return (result);
}
