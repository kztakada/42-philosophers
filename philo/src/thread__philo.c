/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:43:00 by katakada          #+#    #+#             */
/*   Updated: 2025/04/08 19:01:24 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	barrier_wait_for_philo(t_philo *philo)
{
	t_barrier		*barrier;
	pthread_mutex_t	*g_mutex;

	barrier = &philo->g_s->barrier;
	g_mutex = &philo->g_s->g_mutex;
	barrier_wait(barrier);
	pthread_mutex_lock(&philo->p_mutex);
	philo->last_meal_start_time = get_time_in_ms();
	pthread_mutex_unlock(&philo->p_mutex);
	pthread_mutex_lock(g_mutex);
	if (philo->g_s->start_time == 0)
		philo->g_s->start_time = philo->last_meal_start_time;
	pthread_mutex_unlock(g_mutex);
}

static t_lltime	calc_initial_eat_at(t_philo *philo)
{
	const int	time_to_eat_ms = philo->g_s->eating_time;
	const int	n = philo->g_s->num_of_philos;
	const int	same_time_max_eat = n / 2;
	int			offset_unit_time;
	int			offset_unit_count;

	if (same_time_max_eat == 0)
		return (philo->g_s->start_time);
	offset_unit_time = time_to_eat_ms / same_time_max_eat;
	offset_unit_count = (same_time_max_eat * philo->id) % n;
	return (philo->g_s->start_time + (offset_unit_time * offset_unit_count));
}

static void	philo_loop(t_philo *philo)
{
	while (!safe_is_finished(philo->g_s))
	{
		if (philo->state == THINKING)
		{
			if (done_thinking(philo) == FALSE)
				break ;
		}
		else if (philo->state == EATING)
		{
			done_eating(philo);
			if (is_still_hungry(philo) == FALSE)
				break ;
		}
		else if (philo->state == SLEEPING)
		{
			if (done_sleeping(philo) == FALSE)
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
	barrier_wait_for_philo(philo);
	pthread_mutex_lock(&philo->p_mutex);
	philo->next_meal_time = calc_initial_eat_at(philo);
	pthread_mutex_unlock(&philo->p_mutex);
	philo_loop(philo);
	return (NULL);
}
