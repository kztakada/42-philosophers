/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:27:51 by katakada          #+#    #+#             */
/*   Updated: 2025/04/02 19:43:44 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	barrier_wait_for_main(t_shared_dup *s)
{
	int	i;

	i = 0;
	// 全ての子プロセスが準備完了になるのを待つ
	while (i < s->g_dup.num_of_philos)
	{
		handle_e(sem_wait(s->g_dup.barrier.ready_sem), E_SEM_W);
		i++;
	}
	// 全ての子プロセスに開始信号を送る
	i = 0;
	while (i < s->g_dup.num_of_philos)
	{
		handle_e(sem_post(s->g_dup.barrier.start_sem), E_SEM_P);
		i++;
	}
	// セマフォをクローズ
	handle_e(sem_close(s->g_dup.barrier.ready_sem), E_SEM_C);
	handle_e(sem_close(s->g_dup.barrier.start_sem), E_SEM_C);
}

int	app_main(int argc, char **argv)
{
	t_shared_dup	s;
	int				i;

	if (parse_argv(&s, argc, argv) == FALSE)
		return (1);
	if (init_memory_space(&s) == FALSE)
		return (1);
	if (init_shared_dup(&s) == FALSE)
		return (1);
	s.g_dup.start_time = get_time_in_us();
	i = 0;
	while (i < s.g_dup.num_of_philos)
	{
		handle_start_philo_prosess(&s.philos[i]);
		i++;
	}
	i = 0;
	// 全ての子プロセスが終了するのを待つ
	barrier_wait_for_main(&s);
	handle_terminate_all_philo_prosess(&s);
	handle_e(sem_unlink("/ready_sem"), E_SEM_U);
	handle_e(sem_unlink("/start_sem"), E_SEM_U);
	return (0);
}

#ifndef TEST

int	main(int argc, char *argv[])
{
	return (app_main(argc, argv));
}
#endif // TEST
