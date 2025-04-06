/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prosess_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:32:41 by katakada          #+#    #+#             */
/*   Updated: 2025/04/06 19:55:26 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	exec_philo_prosess(t_philo *philo)
{
	if (pthread_create(&philo->main, NULL, philo_rutine, philo) != 0)
		put_error_exit("Error: philo pthread_create\n");
	if (pthread_create(&philo->monitor, NULL, monitor_rutine, philo) != 0)
		put_error_exit("Error: monitor pthread_create\n");
	if (pthread_join(philo->main, NULL) != 0)
		put_error_exit("Error: philo pthread_join\n");
	if (pthread_join(philo->monitor, NULL) != 0)
		put_error_exit("Error: monitor pthread_join\n");
	exit(EXIT_SUCCESS);
}
