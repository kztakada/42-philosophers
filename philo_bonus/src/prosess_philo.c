/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prosess_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:32:41 by katakada          #+#    #+#             */
/*   Updated: 2025/04/02 20:00:16 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	barrier_wait_for_philo(t_philo *philo)
{
	printf("%lld start child philo_id: %d\n", get_time_in_us()
		- philo->g_dup->start_time, philo->id);
	// 子プロセス
	// 準備完了を通知
	handle_e(sem_post(philo->g_dup->barrier.ready_sem), E_SEM_P);
	// 開始信号を待つ
	handle_e(sem_wait(philo->g_dup->barrier.start_sem), E_SEM_W);
	// セマフォをクローズ
	handle_e(sem_close(philo->g_dup->barrier.ready_sem), E_SEM_C);
	handle_e(sem_close(philo->g_dup->barrier.start_sem), E_SEM_C);
	printf("%lld run philo_id: %d\n", get_time_in_us()
		- philo->g_dup->start_time, philo->id);
}

void	exec_philo_prosess(t_philo *philo)
{
	barrier_wait_for_philo(philo);
	exit(EXIT_SUCCESS);
}
