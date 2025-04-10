/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:26:59 by katakada          #+#    #+#             */
/*   Updated: 2025/04/08 19:01:47 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_name(int philo_id)
{
	return (philo_id + 1);
}

void	barrier_wait(t_barrier *barrier)
{
	pthread_mutex_lock(&barrier->b_mutex);
	barrier->arrived_count++;
	pthread_mutex_unlock(&barrier->b_mutex);
	while (TRUE)
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

static void	m_unsafe_print_dead_log_only_once(t_lltime death_time_ms,
		t_philo *philo)
{
	static t_bool	has_printed = FALSE;
	t_lltime		time_stamp;

	philo->g_s->monitor.is_finished = TRUE;
	time_stamp = death_time_ms - philo->g_s->start_time;
	if (!has_printed)
	{
		printf("%lld %d %s\n", time_stamp, philo_name(philo->id), "died");
		has_printed = TRUE;
	}
}

t_bool	safe_is_finished(t_g_shared *g_s)
{
	t_bool	ret;

	pthread_mutex_lock(&g_s->monitor.m_mutex);
	ret = g_s->monitor.is_finished;
	pthread_mutex_unlock(&g_s->monitor.m_mutex);
	return (ret);
}

t_lltime	m_unsafe_get_last_alive_time_us(t_philo *philo)
{
	t_lltime	now_us;
	t_lltime	now_ms;
	t_lltime	survival_time;

	if (philo->g_s->monitor.is_finished)
		return (-1);
	now_us = get_time_in_us();
	now_ms = now_us / 1000;
	survival_time = now_ms - philo->last_meal_start_time;
	if (survival_time > philo->g_s->survival_time_per_meal)
	{
		m_unsafe_print_dead_log_only_once(now_ms, philo);
		return (-1);
	}
	return (now_us);
}
