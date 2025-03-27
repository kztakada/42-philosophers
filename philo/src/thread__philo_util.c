/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:44:35 by katakada          #+#    #+#             */
/*   Updated: 2025/03/27 19:50:17 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// バリア待機させる関数
void	barrier_wait(t_barrier *barrier, int id)
{
	pthread_mutex_lock(&barrier->b_mutex);
	barrier->arrived_count++;
	pthread_mutex_unlock(&barrier->b_mutex);
	while (1)
	{
		pthread_mutex_lock(&barrier->b_mutex);
		if (barrier->arrived_count == barrier->thread_count)
		{
			pthread_mutex_unlock(&barrier->b_mutex);
			break ;
		}
		pthread_mutex_unlock(&barrier->b_mutex);
	}
	philosophers[id].last_meal_satart_time = get_time_in_ms();
	pthread_mutex_lock(&g_mutex);
	if (start_time == 0)
		start_time = philosophers[id].last_meal_satart_time;
	pthread_mutex_unlock(&g_mutex);
}

// 右隣の哲学者のidを取得する関数
int	get_right_philosopher_id(int philo_id)
{
	return ((philo_id + 1) % NUM_PHILOSOPHERS);
}

// 左隣の哲学者のidを取得する関数
int	get_left_philosopher_id(int philo_id)
{
	return ((philo_id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);
}

void	sleep_until_next_mealtime(t_lltime next_time_ms)
{
	t_lltime	current_time;
	t_lltime	next_time_us;
	t_lltime	sleep_time;

	current_time = get_time_in_us();
	next_time_us = next_time_ms * 1000;
	sleep_time = next_time_us - current_time;
	while (sleep_time > 0)
	{
		usleep((useconds_t)(sleep_time / 2));
		current_time = get_time_in_us();
		sleep_time = next_time_us - current_time;
	}
}

void	sleep_from_now(t_lltime sleep_time_ms)
{
	t_lltime	current_time;
	t_lltime	next_time_us;

	current_time = get_time_in_us();
	next_time_us = current_time + (sleep_time_ms * 1000);
	sleep_until_next_mealtime(next_time_us / 1000);
}

// ログを出力する関数
void	print_log_if_alive(int philo_id, char *msg)
{
	t_lltime	current_time;

	current_time = get_time_in_ms();
	printf("%lld %d %s\n", current_time, (philo_id + 1), msg);
}
