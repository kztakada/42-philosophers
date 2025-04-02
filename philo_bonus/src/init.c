/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:28:21 by katakada          #+#    #+#             */
/*   Updated: 2025/04/02 20:33:04 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#define OWNER_ONLY_WRITE 0644

t_bool	init_memory_space(t_shared_dup *s)
{
	s->philos = malloc(sizeof(t_philo) * s->g_dup.num_of_philos);
	if (s->philos == NULL)
		return (put_error_exit("Error: malloc error\n"), FALSE);
	return (TRUE);
}

static sem_t	*get_sem(const char *sem_name, int sem_count)
{
	sem_t	*sem;

	handle_e(sem_unlink(sem_name), E_SEM_U);
	sem = sem_open(sem_name, O_CREAT | O_EXCL, OWNER_ONLY_WRITE, sem_count);
	if (sem == SEM_FAILED)
		put_error_exit("Error: sem_open\n");
	handle_e(sem_unlink(sem_name), E_SEM_U);
	return (sem);
}

static t_bool	init_barrier_sem(t_shared_dup *s)
{
	s->g_dup.barrier.ready_sem = get_sem(B_SEM_READY, 0);
	s->g_dup.barrier.start_sem = get_sem(B_SEM_START, 0);
	if (s->g_dup.barrier.ready_sem == SEM_FAILED
		|| s->g_dup.barrier.start_sem == SEM_FAILED)
	{
		put_error_exit("Error: sem_open\n");
		return (put_error_exit("Error: init_shared_dup\n"), FALSE);
	}
	return (TRUE);
}

static t_bool	init_philo(t_shared_dup *s)
{
	int	i;

	i = 0;
	while (i < s->g_dup.num_of_philos)
	{
		s->philos[i].id = i;
		s->philos[i].state = THINKING;
		s->philos[i].meals_eaten = 0;
		s->philos[i].last_meal_start_time = 0;
		s->philos[i].next_meal_time = 0;
		s->philos[i].g_dup = &s->g_dup;
		i++;
	}
	return (TRUE);
}

t_bool	init_shared_dup(t_shared_dup *s)
{
	if (init_barrier_sem(s) == FALSE)
		return (FALSE);
	if (init_philo(s) == FALSE)
		return (FALSE);
	return (TRUE);
}
