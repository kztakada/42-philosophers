/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:43:00 by katakada          #+#    #+#             */
/*   Updated: 2025/03/31 00:29:23 by katakada         ###   ########.fr       */
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
	pthread_mutex_lock(&philo->p_mutex);
	philo->last_meal_satart_time = get_time_in_ms();
	pthread_mutex_unlock(&philo->p_mutex);
	pthread_mutex_lock(g_mutex);
	if (philo->g_s->start_time == 0)
		philo->g_s->start_time = philo->last_meal_satart_time;
	pthread_mutex_unlock(g_mutex);
}

static t_lltime	calc_initial_eat_at(t_philo *philo)
{
	const int	time_to_eat_ms = philo->g_s->eating_time;
	const int	n = philo->g_s->num_of_philos;
	const int	same_time_max_eat = n / 2;
	const int	offset_unit_time = time_to_eat_ms / same_time_max_eat;
	const int	offset_unit_count = (same_time_max_eat * philo->id) % n;

	// 人数が0か1の場合＝待機時間0
	if (same_time_max_eat == 0)
		return (philo->g_s->start_time);
	return (philo->g_s->start_time + (offset_unit_time * offset_unit_count));
}

static void	philo_loop(t_philo *philo)
{
	while (!safe_is_finished(philo->g_s))
	{
		if (philo->state == THINKING)
		{
			if (done_thinking(philo) == false)
				break ;
		}
		else if (philo->state == EATING)
		{
			done_eating(philo);
			if (is_still_hungry(philo) == false)
				break ;
		}
		else if (philo->state == SLEEPING)
		{
			if (done_sleeping(philo) == false)
				break ;
		}
		if (safe_is_finished(philo->g_s))
			break ;
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// バリア待機
	barrier_wait_for_philo(philo);
	// 次回の食事時間を初期化
	pthread_mutex_lock(&philo->p_mutex);
	philo->next_meal_time = calc_initial_eat_at(philo);
	pthread_mutex_unlock(&philo->p_mutex);
	philo_loop(philo);
	return (NULL);
}
