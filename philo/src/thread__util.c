/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:26:59 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 16:31:29 by katakada         ###   ########.fr       */
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
static void	print_dead_log_only_once(t_lltime death_time_ms, t_philo *philo)
{
	static bool	has_printed = false;
	t_lltime	time_stamp;

	philo->g_s->monitor.is_finished = true;
	time_stamp = death_time_ms - philo->g_s->start_time;
	if (!has_printed)
	{
		printf("%lld %d %s\n", time_stamp, philo_name(philo->id), "died");
		has_printed = true;
	}
}

bool	safe_is_finished(t_g_shared *g_s)
{
	bool	ret;

	pthread_mutex_lock(&g_s->monitor.m_mutex);
	ret = g_s->monitor.is_finished;
	pthread_mutex_unlock(&g_s->monitor.m_mutex);
	return (ret);
}

t_lltime	get_last_alive_time_us(t_philo *philo)
{
	t_lltime		now_us;
	t_lltime		now_ms;
	t_lltime		survival_time;
	pthread_mutex_t	*m_mutex;

	m_mutex = &philo->g_s->monitor.m_mutex;
	pthread_mutex_lock(m_mutex);
	if (philo->g_s->monitor.is_finished)
	{
		pthread_mutex_unlock(m_mutex);
		return (-1);
	}
	now_us = get_time_in_us();
	now_ms = now_us / 1000;
	survival_time = now_ms - philo->last_meal_satart_time;
	if (survival_time > philo->g_s->survival_time_per_meal)
	{
		// 死亡ログを出力していない場合、死亡ログを出力する
		print_dead_log_only_once(now_ms, philo);
		pthread_mutex_unlock(m_mutex);
		return (-1);
	}
	pthread_mutex_unlock(m_mutex);
	return (now_us);
}
