/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_prosess.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:10:53 by katakada          #+#    #+#             */
/*   Updated: 2025/04/08 18:54:11 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#define ANY_ONE_PROCESS -1

void	handle_start_philo_prosess(t_philo *philo)
{
	philo->pid = fork();
	if (philo->pid < 0)
		put_error_exit("Error: fork()\n");
	else if (philo->pid == 0)
		exec_philo_prosess(philo);
}

static void	kill_all_philos(t_shared_dup *s)
{
	int	i;

	i = 0;
	while (i < s->g_dup.num_of_philos)
	{
		if (s->philos[i].pid > 0)
			kill(s->philos[i].pid, SIGKILL);
		i++;
	}
}

void	handle_terminate_all_philo_prosess(t_shared_dup *s)
{
	int		status;
	int		exit_count;
	pid_t	returned_child_pid;

	status = 0;
	exit_count = 0;
	while (exit_count < s->g_dup.num_of_philos)
	{
		returned_child_pid = waitpid(ANY_ONE_PROCESS, &status, 0);
		if (returned_child_pid > 0 && WIFEXITED(status)
			&& WEXITSTATUS(status) == EXIT_SUCCESS)
		{
			exit_count++;
			continue ;
		}
		kill_all_philos(s);
		break ;
	}
}
