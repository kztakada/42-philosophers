/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo__try_to_eat__has_first_priori        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:31:46 by katakada          #+#    #+#             */
/*   Updated: 2025/04/08 19:00:30 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_bool	setup_clocks_for_priorty_calc(t_philo *philo,
		t_lltime *current_time_us)
{
	pthread_mutex_lock(&philo->p_mutex);
	if (philo->wait_start_us == 0)
		philo->wait_start_us = get_time_in_us();
	pthread_mutex_lock(&philo->g_s->monitor.m_mutex);
	*current_time_us = m_unsafe_get_last_alive_time_us(philo);
	pthread_mutex_unlock(&philo->g_s->monitor.m_mutex);
	pthread_mutex_unlock(&philo->p_mutex);
	if (*current_time_us == -1)
		return (FALSE);
	return (TRUE);
}

static void	setup_max_waiter(int *priority_philo, t_lltime *max_wait_us,
		t_philo *philo, t_lltime current_time_us)
{
	t_lltime	wait_time_us;
	int			i;

	i = 0;
	while (i < philo->g_s->num_of_philos)
	{
		pthread_mutex_lock(&philo->other_philos[i].p_mutex);
		if (philo->other_philos[i].state == THINKING
			&& philo->other_philos[i].wait_start_us != 0)
		{
			wait_time_us = current_time_us
				- philo->other_philos[i].wait_start_us;
			if (wait_time_us > *max_wait_us)
			{
				*max_wait_us = wait_time_us;
				*priority_philo = i;
			}
		}
		pthread_mutex_unlock(&philo->other_philos[i].p_mutex);
		i++;
	}
}

t_bool	has_first_priority(t_philo *philo)
{
	t_lltime	current_time_us;
	t_lltime	max_wait_us;
	int			priority_philo;
	t_philo		*other_philos;

	if (setup_clocks_for_priorty_calc(philo, &current_time_us) == FALSE)
		return (FALSE);
	max_wait_us = 0;
	priority_philo = -1;
	setup_max_waiter(&priority_philo, &max_wait_us, philo, current_time_us);
	other_philos = philo->other_philos;
	if (max_wait_us >= philo->g_s->wait_threshold_us && priority_philo != -1
		&& priority_philo != philo->id
		&& (left_philo_id(&other_philos[priority_philo]) == philo->id
			|| right_philo_id(&other_philos[priority_philo]) == philo->id))
	{
		usleep(PRIORITY_WAIT_TIME_US);
		return (FALSE);
	}
	return (TRUE);
}
