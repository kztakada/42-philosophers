/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__monitor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:28:13 by katakada          #+#    #+#             */
/*   Updated: 2025/04/07 00:37:51 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor_rutine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sleep_until_next_time(philo->g_dup->start_time);
	while (TRUE)
	{
		handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
		printf("");
		if (philo->g_dup->is_finished)
		{
			handle_e(sem_post(philo->can_touch_me), E_SEM_P);
			break ;
		}
		handle_e(sem_wait(philo->g_dup->can_log), E_SEM_W);
		printf("");
		if (unsafe_get_last_alive_time(philo) == -1)
			break ;
		handle_e(sem_post(philo->g_dup->can_log), E_SEM_P);
		handle_e(sem_post(philo->can_touch_me), E_SEM_P);
		printf("");
		usleep(MONITOR_INTERVAL_US);
	}
	if (philo->g_dup->is_finished == FALSE)
		all_sem_close_at_thread(philo);
	return (NULL);
}
