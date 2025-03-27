/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__monitor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:21:02 by katakada          #+#    #+#             */
/*   Updated: 2025/03/28 00:01:12 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_lltime	now;
	t_lltime	survival_time;
	bool		is_anyone_dead;
	bool		can_still_eat;
	int			i;

	(void)arg;
	barrier_wait(&barrier);
	i = 0;
	is_anyone_dead = false;
	can_still_eat = true;
	while (!is_anyone_dead && can_still_eat)
	{
		can_still_eat = false;
		usleep(MONITOR_INTERVAL_US);
		while (i < NUM_PHILOSOPHERS)
		{
			pthread_mutex_lock(philosophers[i].p_mutex);
			// 食事回数が設定されているか or 指定回数以内かを確認
			if (REQUIRED_MEALS < 0
				|| philosophers[i].meals_eaten < REQUIRED_MEALS)
			{
				can_still_eat = true;
				// 死亡確認
				now = get_time_in_ms();
				survival_time = now - philosophers[i].last_meal_satart_time;
				if (survival_time > SURVIVAL_TIME_PER_MEAL)
				{
					// 死亡ログを出力していない場合、死亡ログを出力する
					pthread_mutex_lock(&m_mutex);
					// printf("monitor call\n");
					print_dead_log_once(now, i);
					pthread_mutex_unlock(&m_mutex);
					is_anyone_dead = true;
				}
			}
			pthread_mutex_unlock(philosophers[i].p_mutex);
			i++;
		};
		i = 0;
	}
	return (NULL);
}
