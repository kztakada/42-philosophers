/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_prosess.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:10:53 by katakada          #+#    #+#             */
/*   Updated: 2025/04/02 19:30:13 by katakada         ###   ########.fr       */
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

// WIFEXITED(status)
// これは子プロセスが正常に終了したかどうかを確認するマクロです。子プロセスが exit() や return などで通常終了した場合に真となります。一方、シグナル（SIGKILL や SIGSEGV など）によって終了した場合は偽となります。
// WEXITSTATUS(status) == EXIT_SUCCESS
// WIFEXITED が真の場合、この部分は子プロセスの終了コードが EXIT_SUCCESS（通常は0）と等しいかを確認します。
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
			// TODO: ログ出力用のセマフォを解除（sem_post）をここに追加する
			exit_count++;
			printf("Child process %d exited successfully\n",
				returned_child_pid);
			continue ;
		}
		kill_all_philos(s);
		break ;
	}
	printf("All child processes exited\n");
}
