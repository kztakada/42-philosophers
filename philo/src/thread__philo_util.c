/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:44:35 by katakada          #+#    #+#             */
/*   Updated: 2025/03/28 00:28:59 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// バリア待機させる関数 for Pilo
void	barrier_wait_for_philo(t_barrier *barrier, int id)
{
	barrier_wait(barrier);
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
	t_lltime	now;
	t_lltime	next_time_us;
	t_lltime	sleep_time;

	now = get_time_in_us();
	next_time_us = next_time_ms * 1000;
	sleep_time = next_time_us - now;
	while (sleep_time > 0)
	{
		usleep((useconds_t)(sleep_time / 2));
		now = get_time_in_us();
		sleep_time = next_time_us - now;
	}
}

void	sleep_from_now(t_lltime sleep_time_ms)
{
	t_lltime	now;
	t_lltime	next_time_us;

	now = get_time_in_us();
	next_time_us = now + (sleep_time_ms * 1000);
	sleep_until_next_mealtime(next_time_us / 1000);
}

// ログを出力する関数
// bool	print_log_if_alive(int philo_id, char *msg)
// {
// 	t_lltime	now;
// 	t_lltime	survival_time;
// 	bool		ret;

// 	pthread_mutex_lock(&m_mutex);
// 	if (!is_finished)
// 	{
// 		now = get_time_in_ms();
// 		survival_time = now - philosophers[philo_id].last_meal_satart_time;
// 		if (survival_time < SURVIVAL_TIME_PER_MEAL)
// 		{
// 			printf("%lld %d %s\n", now - start_time, (philo_id + 1), msg);
// 			ret = true;
// 		}
// 		else
// 		{
// 			print_dead_log_once(now, philo_id);
// 			ret = false;
// 		}
// 	}
// 	else
// 		ret = false;
// 	pthread_mutex_unlock(&m_mutex);
// 	return (ret);
// }

void	print_log_if_alive(int philo_id, char *msg)
{
	t_lltime	now;
	t_lltime	survival_time;

	now = get_time_in_ms();
	survival_time = now - philosophers[philo_id].last_meal_satart_time;
	if (survival_time > SURVIVAL_TIME_PER_MEAL)
	{
		// 死亡ログを出力していない場合、死亡ログを出力する
		pthread_mutex_lock(&m_mutex);
		// printf("philo %d call\n", philo_id + 1);
		print_dead_log_once(now, philo_id);
		pthread_mutex_unlock(&m_mutex);
	}
	if (!safe_is_finished())
		printf("%lld %d %s\n", now - start_time, (philo_id + 1), msg);
}
