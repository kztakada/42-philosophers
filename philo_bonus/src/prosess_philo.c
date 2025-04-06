/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prosess_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:32:41 by katakada          #+#    #+#             */
/*   Updated: 2025/04/06 15:58:21 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	exec_philo_prosess(t_philo *philo)
{
	// pthread_mutex_init(&philo->m_mutex, NULL);
	if (pthread_create(&philo->main, NULL, philo_rutine, philo) != 0)
		put_error_exit("Error: philo pthread_create\n");
	if (pthread_create(&philo->monitor, NULL, monitor_rutine, philo) != 0)
		put_error_exit("Error: monitor pthread_create\n");
	if (pthread_join(philo->monitor, NULL) != 0)
		put_error_exit("Error: monitor pthread_join\n");
	if (pthread_join(philo->main, NULL) != 0)
		put_error_exit("Error: philo pthread_join\n");
	handle_e(sem_close(philo->can_touch_me), E_SEM_C);
	handle_e(sem_close(philo->g_dup->can_log), E_SEM_C);
	handle_e(sem_close(philo->g_dup->can_log_dead), E_SEM_C);
	handle_e(sem_close(philo->g_dup->forks), E_SEM_C);
	handle_e(sem_close(philo->g_dup->waiters), E_SEM_C);
	exit(EXIT_SUCCESS);
}
