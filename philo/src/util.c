/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:54:09 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 02:02:17 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	left_fork(t_philo *philo)
{
	return (philo->id);
}

int	right_fork(t_philo *philo)
{
	return ((philo->id + 1) % philo->g_s->num_of_philos);
}
