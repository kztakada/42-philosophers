/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:43:00 by katakada          #+#    #+#             */
/*   Updated: 2025/03/29 00:59:40 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	try_to_eat(t_philo *philo)
{
	bool		result;
	t_lltime	current_time_us;
	t_lltime	max_wait;
	int			priority_phil;
	t_lltime	wait_time;

	result = false;
	pthread_mutex_lock(&philo->p_mutex);
	if (philo->wait_start_us == 0)
	{
		philo->wait_start_us = get_time_in_us();
	}
	current_time_us = get_last_alive_time_us(philo);
	if (current_time_us == -1)
		return (false);
	pthread_mutex_unlock(&philo->p_mutex);
	// 優先度確認ブロック
	max_wait = 0;
	priority_phil = -1;
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		if (philo->other_philos[i].state == THINKING
			&& philo->other_philos[i].wait_start_us != 0)
		{
			wait_time = current_time_us - philo->other_philos[i].wait_start_us;
			if (wait_time > max_wait)
			{
				max_wait = wait_time;
				priority_phil = i;
			}
		}
	}
	if (max_wait > philo->g_s->wait_threshold_time && priority_phil != -1
		&& priority_phil != philo->id
		&& (left_philo_id(priority_phil) == philo->id
			|| right_philo_id(priority_phil) == philo->id))
	{
		sleep_from_now(philo->g_s->prioryty_wait_time);
		return (false);
	}
	// フォーク取得ブロック
	if (reserve_forks(philo))
	{
		// 1本目を取得する
		pthread_mutex_lock(philo->left_fork);
		print_log_if_alive(philo, "has taken a fork");
		if (get_last_alive_time_us(philo) != -1)
		{
			// 両方のフォークを取得できた場合
			pthread_mutex_lock(philo->right_fork);
			print_log_if_alive(philo, "has taken a fork");
			pthread_mutex_lock(&philo->p_mutex);
			philo->state = EATING;
			philo->wait_start_us = 0;
			philo->last_meal_satart_time = get_time_in_ms();
			pthread_mutex_unlock(&philo->p_mutex);
			print_log_if_alive(philo, "is eating");
			result = true;
		}
		else
		{
			// １本目を取った後、２本目を取る前に死んだ場合
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_lock(&philo->g_s->monitor.m_mutex);
			philo->g_s->monitor.is_finished = true;
			pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
			result = false;
		}
	}
	return (result);
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

static t_lltime	calc_initial_eat_at(t_philo *philo)
{
	const int	time_to_eat_ms = EATING_TIME_MS;
	const int	n = NUM_PHILOSOPHERS;
	const int	same_time_max_eat = NUM_PHILOSOPHERS / 2;
	const int	offset_unit_time = time_to_eat_ms / same_time_max_eat;
	const int	offset_unit_count = (same_time_max_eat * philo->id) % n;

	// 人数が0か1の場合＝待機時間0
	if (same_time_max_eat == 0)
		return (philo->g_s->start_time);
	return (philo->g_s->start_time + (offset_unit_time * offset_unit_count));
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// バリア待機
	barrier_wait_for_philo(philo);
	// 次回の食事時間を初期化
	pthread_mutex_lock(&philo->p_mutex);
	philo->next_meal_time = calc_initial_eat_at(philo);
	pthread_mutex_unlock(&philo->p_mutex);
	while (!safe_is_finished(philo->g_s))
	{
		if (philo->state == THINKING)
		{
			print_log_if_alive(philo, "is thinking");
			sleep_until_next_mealtime(philo->next_meal_time);
			while (try_to_eat(philo) == false)
			{
				if (safe_is_finished(philo->g_s))
					break ;
				usleep(RETRAY_TIME_US);
			}
			philo->state = EATING;
		}
		else if (philo->state == EATING)
		{
			sleep_from_now(EATING_TIME_MS);
			finish_eating(philo);
			philo->state = SLEEPING;
			if (REQUIRED_MEALS > 0 && philo->meals_eaten >= REQUIRED_MEALS)
			{
				philo->wait_start_us = 0;
				// printf("Philosopher %d has finished all %d meals\n", id,
				// 	REQUIRED_MEALS);
				break ;
			}
		}
		else if (philo->state == SLEEPING)
		{
			print_log_if_alive(philo, "is sleeping");
			sleep_from_now(SLEEPING_TIME_MS);
			philo->state = THINKING;
		}
		if (safe_is_finished(philo->g_s))
			break ;
	}
	return (NULL);
}
