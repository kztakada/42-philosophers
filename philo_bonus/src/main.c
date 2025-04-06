/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:27:51 by katakada          #+#    #+#             */
/*   Updated: 2025/04/06 16:30:30 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	all_sem_close(t_shared_dup *s)
{
	int	i;

	i = 0;
	while (i < s->g_dup.num_of_philos)
	{
		handle_e(sem_close(s->philos[i].can_touch_me), E_SEM_C);
		i++;
	}
	handle_e(sem_close(s->g_dup.forks), E_SEM_C);
	handle_e(sem_close(s->g_dup.waiters), E_SEM_C);
	handle_e(sem_close(s->g_dup.can_log), E_SEM_C);
	handle_e(sem_close(s->g_dup.can_log_dead), E_SEM_C);
}

int	main(int argc, char **argv)
{
	t_shared_dup	s;
	int				i;

	if (parse_argv(&s, argc, argv) == FALSE)
		return (1);
	if (init_memory_space(&s) == FALSE)
		return (1);
	init_shared_dup(&s);
	i = 0;
	while (i < s.g_dup.num_of_philos)
	{
		handle_start_philo_prosess(&s.philos[i]);
		i++;
	}
	handle_terminate_all_philo_prosess(&s);
	all_sem_close(&s);
	free(s.philos);
	return (0);
}
