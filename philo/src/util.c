/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:54:09 by katakada          #+#    #+#             */
/*   Updated: 2025/03/27 17:54:10 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	left_fork(int philo_id)
{
	return (philo_id);
}

int	right_fork(int philo_id)
{
	return ((philo_id + 1) % NUM_PHILOSOPHERS);
}
