/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo__try_to_eat.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:57:52 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 19:56:15 by katakada         ###   ########.fr       */
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

static bool	setup_clocks_for_priorty_calc(t_philo *philo,
		t_lltime *current_time_us)
{
	// 現在時刻取得ブロック
	pthread_mutex_lock(&philo->p_mutex);
	// waitスタートタイム初期化
	if (philo->wait_start_us == 0)
		philo->wait_start_us = get_time_in_us();
	// 現在時刻取得
	pthread_mutex_lock(&philo->g_s->monitor.m_mutex);
	*current_time_us = m_unsafe_get_last_alive_time_us(philo);
	pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
	pthread_mutex_unlock(&philo->p_mutex);
	if (*current_time_us == -1)
		return (false);
	return (true);
}

static void	setup_max_waiter(int *priority_philo, t_lltime *max_wait,
		t_philo *philo, t_lltime current_time_us)
{
	t_lltime	wait_time;
	int			i;

	i = 0;
	while (i < philo->g_s->num_of_philos)
	{
		if (philo->other_philos[i].state == THINKING
			&& philo->other_philos[i].wait_start_us != 0)
		{
			wait_time = current_time_us - philo->other_philos[i].wait_start_us;
			if (wait_time > *max_wait)
			{
				*max_wait = wait_time;
				*priority_philo = i;
			}
		}
		i++;
	}
}

// 自分の両隣でかつ、自分よりも長く待っている哲学者を優先する
bool	has_first_priority(t_philo *philo)
{
	t_lltime	current_time_us;
	t_lltime	max_wait;
	int			priority_philo;

	// 既に死んでたらここで離脱
	if (setup_clocks_for_priorty_calc(philo, &current_time_us) == false)
		return (false);
	max_wait = 0;
	priority_philo = -1;
	// max_wait計算ブロック
	setup_max_waiter(&priority_philo, &max_wait, philo, current_time_us);
	// 判断ブロック
	if (max_wait > philo->g_s->wait_threshold_time && priority_philo != -1
		&& priority_philo != philo->id
		&& (left_philo_id(&philo->other_philos[priority_philo]) == philo->id
			|| right_philo_id(&philo->other_philos[priority_philo]) == philo->id))
	{
		usleep(PRIORITY_WAIT_TIME_US);
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
