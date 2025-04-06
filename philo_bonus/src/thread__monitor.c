/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__monitor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:28:13 by katakada          #+#    #+#             */
/*   Updated: 2025/04/06 16:13:49 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#define MONITOR_INTERVAL_US 1000

void	handle_e_m(int func_result, char *err_msg)
{
	// printf("func_result: %d\n", func_result);
	if (func_result < 0)
		put_error_exit(err_msg);
}

// static void	is_alive(t_philo *philo)
// {
// 	// t_lltime	now;
// 	// handle_e_m(sem_wait(philo->can_touch_me), E_SEM_W);
// 	// printf("monitor wait \n");
// 	// if (philo->state != EATING)
// 	unsafe_get_last_alive_time(philo);
// 	// handle_e(sem_post(philo->can_touch_me), E_SEM_P);
// 	printf("monitor post id:%d\n", philo_name(philo->id));
// 	// if (now == -1)
// 	// 	return (FALSE);
// 	// return (TRUE);
// }

void	*monitor_rutine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sleep_until_next_mealtime(philo->g_dup->start_time);
	// printf("%lld wait monitor id:%d\n", get_time_in_us(),
	// 	philo_name(philo->id));
	// handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	// printf("%lld start monitor id:%d\n", get_time_in_us(),
	// 	philo_name(philo->id));
	while (TRUE)
	{
		handle_e_m(sem_wait(philo->can_touch_me), E_SEM_W);
		handle_e(sem_wait(philo->g_dup->can_log), E_SEM_W);
		printf("");
		unsafe_get_last_alive_time(philo);
		handle_e(sem_post(philo->g_dup->can_log), E_SEM_P);
		handle_e(sem_post(philo->can_touch_me), E_SEM_P);
		printf("");
		usleep(MONITOR_INTERVAL_US);
	}
	return (NULL);
}
