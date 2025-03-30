/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__monitor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:21:02 by katakada          #+#    #+#             */
/*   Updated: 2025/03/31 01:06:15 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	monitor_loop(t_shared *s, t_bool is_anyone_dead,
		t_bool can_still_eat)
{
	int	i;

	i = 0;
	while (!is_anyone_dead && can_still_eat)
	{
		can_still_eat = FALSE;
		usleep(MONITOR_INTERVAL_US);
		while (i < s->g_s.num_of_philos)
		{
			pthread_mutex_lock(&s->philos[i].p_mutex);
			// 食事回数が設定されているか or 指定回数以内かを確認
			if (s->g_s.required_meals < 0
				|| s->philos[i].meals_eaten < s->g_s.required_meals)
			{
				can_still_eat = TRUE;
				// 死亡確認
				pthread_mutex_lock(&s->g_s.monitor.m_mutex);
				if (m_unsafe_get_last_alive_time_us(&s->philos[i]) == -1)
					is_anyone_dead = TRUE;
				pthread_mutex_unlock(&s->g_s.monitor.m_mutex);
			}
			pthread_mutex_unlock(&s->philos[i].p_mutex);
			i++;
		}
		i = 0;
	}
}

void	*monitor_routine(void *arg)
{
	t_bool		is_anyone_dead;
	t_bool		can_still_eat;
	t_shared	*s;

	s = (t_shared *)arg;
	barrier_wait(&s->g_s.barrier);
	is_anyone_dead = FALSE;
	can_still_eat = TRUE;
	monitor_loop(s, is_anyone_dead, can_still_eat);
	return (NULL);
}
