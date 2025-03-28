/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:43:00 by katakada          #+#    #+#             */
/*   Updated: 2025/03/28 22:22:04 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	try_to_eat(int philo_id)
{
	bool		result;
	t_lltime	current_time_us;
	t_lltime	max_wait;
	int			priority_phil;
	t_lltime	wait_time;

	result = false;
	pthread_mutex_lock(philosophers[philo_id].p_mutex);
	if (philosophers[philo_id].wait_start_us == 0)
	{
		philosophers[philo_id].wait_start_us = get_time_in_us();
	}
	current_time_us = get_last_alive_time_us(philo_id);
	if (current_time_us == -1)
		return (false);
	pthread_mutex_unlock(philosophers[philo_id].p_mutex);
	// 優先度確認ブロック
	max_wait = 0;
	priority_phil = -1;
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		if (philosophers[i].state == THINKING
			&& philosophers[i].wait_start_us != 0)
		{
			wait_time = current_time_us - philosophers[i].wait_start_us;
			if (wait_time > max_wait)
			{
				max_wait = wait_time;
				priority_phil = i;
			}
		}
	}
	if (max_wait > WAIT_THRESHOLD_MS && priority_phil != -1
		&& priority_phil != philo_id
		&& (left_philo_id(priority_phil) == philo_id
			|| right_philo_id(priority_phil) == philo_id))
	{
		printf("Philosopher %d is waiting for too long, \
			giving priority to philosopher %d\n",
				philo_id,
				priority_phil);
		sleep_from_now(PRIORITY_WAIT_TIME_MS);
		return (false);
	}
	// フォーク取得ブロック
	if (reserve_forks(philo_id))
	{
		// 1本目を取得する
		pthread_mutex_lock(philosophers[philo_id].left_fork);
		print_log_if_alive(philo_id, "has taken a fork");
		if (get_last_alive_time_us(philo_id) != -1)
		{
			// 両方のフォークを取得できた場合
			pthread_mutex_lock(philosophers[philo_id].right_fork);
			print_log_if_alive(philo_id, "has taken a fork");
			pthread_mutex_lock(philosophers[philo_id].p_mutex);
			philosophers[philo_id].state = EATING;
			philosophers[philo_id].wait_start_us = 0;
			philosophers[philo_id].last_meal_satart_time = get_time_in_ms();
			pthread_mutex_unlock(philosophers[philo_id].p_mutex);
			print_log_if_alive(philo_id, "is eating");
			result = true;
		}
		else
		{
			// １本目を取った後、２本目を取る前に死んだ場合
			pthread_mutex_unlock(philosophers[philo_id].left_fork);
			pthread_mutex_lock(&m_mutex);
			is_finished = true;
			pthread_mutex_unlock(&m_mutex);
			result = false;
		}
	}
	return (result);
}

static void	finish_eating(int philo_id)
{
	pthread_mutex_lock(philosophers[philo_id].p_mutex);
	philosophers[philo_id].meals_eaten++;
	philosophers[philo_id].next_meal_time += meal_interval_time;
	pthread_mutex_unlock(philosophers[philo_id].p_mutex);
	unreseve_forks(philo_id);
	pthread_mutex_unlock(philosophers[philo_id].left_fork);
	pthread_mutex_unlock(philosophers[philo_id].right_fork);
}

static t_lltime	calc_initial_eat_at(int philo_id)
{
	const int	time_to_eat_ms = EATING_TIME_MS;
	const int	n = NUM_PHILOSOPHERS;
	const int	same_time_max_eat = NUM_PHILOSOPHERS / 2;
	const int	offset_unit_time = time_to_eat_ms / same_time_max_eat;
	const int	offset_unit_count = (same_time_max_eat * philo_id) % n;

	// 人数が0か1の場合＝待機時間0
	if (same_time_max_eat == 0)
		return (start_time);
	return (start_time + (offset_unit_time * offset_unit_count));
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	int		philo_id;

	philo = (t_philo *)arg;
	philo_id = philo->id;
	// バリア待機
	barrier_wait_for_philo(&philo->g_s->barrier, philo_id);
	// 次回の食事時間を初期化
	pthread_mutex_lock(philosophers[philo_id].p_mutex);
	philosophers[philo_id].next_meal_time = calc_initial_eat_at(philo_id);
	pthread_mutex_unlock(philosophers[philo_id].p_mutex);
	while (!safe_is_finished())
	{
		if (philosophers[philo_id].state == THINKING)
		{
			print_log_if_alive(philo_id, "is thinking");
			sleep_until_next_mealtime(philosophers[philo_id].next_meal_time);
			while (try_to_eat(philo_id) == false)
			{
				if (safe_is_finished())
					break ;
				usleep(RETRAY_TIME_US);
			}
			philosophers[philo_id].state = EATING;
		}
		else if (philosophers[philo_id].state == EATING)
		{
			sleep_from_now(EATING_TIME_MS);
			finish_eating(philo_id);
			philosophers[philo_id].state = SLEEPING;
			if (REQUIRED_MEALS > 0
				&& philosophers[philo_id].meals_eaten >= REQUIRED_MEALS)
			{
				philosophers[philo_id].wait_start_us = 0;
				// printf("Philosopher %d has finished all %d meals\n", id,
				// 	REQUIRED_MEALS);
				break ;
			}
		}
		else if (philosophers[philo_id].state == SLEEPING)
		{
			print_log_if_alive(philo_id, "is sleeping");
			sleep_from_now(SLEEPING_TIME_MS);
			philosophers[philo_id].state = THINKING;
		}
		if (safe_is_finished())
			break ;
	}
	return (NULL);
}
