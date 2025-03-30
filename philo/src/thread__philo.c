/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:43:00 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 16:57:00 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	finish_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->p_mutex);
	philo->meals_eaten++;
	philo->next_meal_time += philo->g_s->meal_interval_time;
	pthread_mutex_unlock(&philo->p_mutex);
	unreseve_forks(philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
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
	while (!safe_is_finished(philo->g_s))
	{
		if (philo->state == THINKING)
		{
			print_log_if_alive(philo, "is thinking");
			sleep_until_next_mealtime(philo->next_meal_time);
			while (try_to_eat(philo) == false)
			{
				if (safe_is_finished(philo->g_s))
					break ;
				usleep(RETRAY_TIME_US);
			}
			philo->state = EATING;
		}
		else if (philo->state == EATING)
		{
			sleep_from_now(philo->g_s->eating_time);
			finish_eating(philo);
			philo->state = SLEEPING;
			if (philo->g_s->required_meals > 0
				&& philo->meals_eaten >= philo->g_s->required_meals)
			{
				philo->wait_start_us = 0;
				// printf("Philosopher %d has finished all %d meals\n", id,
				// 	REQUIRED_MEALS);
				break ;
			}
		}
		else if (philo->state == SLEEPING)
		{
			print_log_if_alive(philo, "is sleeping");
			sleep_from_now(philo->g_s->sleeping_time);
			philo->state = THINKING;
		}
		if (safe_is_finished(philo->g_s))
			break ;
	}
	return (NULL);
}
