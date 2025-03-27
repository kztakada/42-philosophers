/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:26:59 by katakada          #+#    #+#             */
/*   Updated: 2025/03/27 23:33:44 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
void	print_dead_log_once(t_lltime death_time_ms, int philo_id)
{
	static bool	is_printed = false;
	t_lltime	print_time;

	is_finished = true;
	print_time = death_time_ms - start_time;
	if (!is_printed)
	{
		printf("%lld %d %s\n", print_time, (philo_id + 1), "died");
		is_printed = true;
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
