/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:27:29 by katakada          #+#    #+#             */
/*   Updated: 2025/04/07 00:26:15 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static t_bool	safe_is_hungry(t_philo *philo)
{
	t_bool	result;

	handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	if (philo->g_dup->required_meals < 0)
		result = TRUE;
	else
		result = (philo->meals_eaten < philo->g_dup->required_meals);
	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	return (result);
}

static t_bool	safe_is_in_state(t_philo *philo, enum e_philo_state state)
{
	t_bool	result;

	handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	result = (philo->state == state);
	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	return (result);
}

void	*philo_rutine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sleep_until_next_time(philo->g_dup->start_time);
	while (safe_is_hungry(philo) == TRUE)
	{
		if (safe_is_in_state(philo, THINKING))
			done_thinking(philo);
		else if (safe_is_in_state(philo, EATING))
			done_eating(philo);
		else if (safe_is_in_state(philo, SLEEPING))
			done_sleeping(philo);
		else
			put_error_exit("Error: Invalid philo state\n");
	}
	if (philo->g_dup->is_finished == FALSE)
		all_sem_close_at_thread(philo);
	return (NULL);
}
