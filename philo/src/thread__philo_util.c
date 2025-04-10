/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:44:35 by katakada          #+#    #+#             */
/*   Updated: 2025/04/08 19:01:02 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	right_philo_id(t_philo *philo)
{
	return ((philo->id + 1) % philo->g_s->num_of_philos);
}

int	left_philo_id(t_philo *philo)
{
	return ((philo->id + philo->g_s->num_of_philos - 1)
		% philo->g_s->num_of_philos);
}

void	sleep_until_next_time(t_lltime next_time_ms)
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

t_bool	print_log_if_alive(t_philo *philo, char *msg, t_lltime *store_time)
{
	t_lltime	now_us;
	t_lltime	now_ms;
	t_lltime	time_stamp;

	pthread_mutex_lock(&philo->g_s->monitor.m_mutex);
	now_us = m_unsafe_get_last_alive_time_us(philo);
	if (now_us == -1)
	{
		pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
		return (FALSE);
	}
	now_ms = now_us / 1000;
	if (store_time != NULL)
		*store_time = now_ms;
	time_stamp = now_ms - philo->g_s->start_time;
	if (!philo->g_s->monitor.is_finished)
	{
		printf("%lld %d %s\n", time_stamp, philo_name(philo->id), msg);
		pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
		return (TRUE);
	}
	pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
	return (FALSE);
}
