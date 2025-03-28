/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:26:59 by katakada          #+#    #+#             */
/*   Updated: 2025/03/28 16:22:13 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// philosopherの名前を取得する
int	philo_name(int philo_id)
{
	return (philo_id + 1);
}

// バリア待機させる関数
void	barrier_wait(t_barrier *barrier)
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
}

// 死亡したログを１度だけ表示させる
void	print_dead_log_only_once(t_lltime death_time_ms, int philo_id)
{
	static bool	has_printed = false;
	t_lltime	time_stamp;

	is_finished = true;
	time_stamp = death_time_ms - start_time;
	if (!has_printed)
	{
		printf("%lld %d %s\n", time_stamp, philo_name(philo_id), "died");
		has_printed = true;
	}
}

bool	safe_is_finished(void)
{
	bool	ret;

	pthread_mutex_lock(&m_mutex);
	ret = is_finished;
	pthread_mutex_unlock(&m_mutex);
	return (ret);
}

t_lltime	get_last_alive_time_us(int philo_id)
{
	t_lltime	now_us;
	t_lltime	now_ms;
	t_lltime	survival_time;

	pthread_mutex_lock(&m_mutex);
	now_us = get_time_in_us();
	now_ms = now_us / 1000;
	survival_time = now_ms - philosophers[philo_id].last_meal_satart_time;
	if (survival_time > SURVIVAL_TIME_PER_MEAL)
	{
		// 死亡ログを出力していない場合、死亡ログを出力する
		print_dead_log_only_once(now_ms, philo_id);
		pthread_mutex_unlock(&m_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&m_mutex);
	return (now_us);
}
