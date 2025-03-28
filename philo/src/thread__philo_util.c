/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:44:35 by katakada          #+#    #+#             */
/*   Updated: 2025/03/29 00:55:51 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// バリア待機させる関数 for Pilo
void	barrier_wait_for_philo(t_philo *philo)
{
	t_barrier		*barrier;
	pthread_mutex_t	*g_mutex;

	barrier = &philo->g_s->barrier;
	g_mutex = &philo->g_s->g_mutex;
	barrier_wait(barrier);
	philo->last_meal_satart_time = get_time_in_ms();
	pthread_mutex_lock(g_mutex);
	if (philo->g_s->start_time == 0)
		philo->g_s->start_time = philo->last_meal_satart_time;
	pthread_mutex_unlock(g_mutex);
}

// 右隣の哲学者のidを取得する関数
int	right_philo_id(int philo_id)
{
	return ((philo_id + 1) % NUM_PHILOSOPHERS);
}

// 左隣の哲学者のidを取得する関数
int	left_philo_id(int philo_id)
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

void	print_log_if_alive(t_philo *philo, char *msg)
{
	t_lltime	now_us;
	t_lltime	now_ms;
	t_lltime	time_stamp;

	now_us = get_last_alive_time_us(philo);
	if (now_us == -1)
		return ;
	now_ms = now_us / 1000;
	time_stamp = now_ms - philo->g_s->start_time;
	pthread_mutex_lock(&philo->g_s->monitor.m_mutex);
	if (!philo->g_s->monitor.is_finished)
		printf("%lld %d %s\n", time_stamp, philo_name(philo->id), msg);
	pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
}
