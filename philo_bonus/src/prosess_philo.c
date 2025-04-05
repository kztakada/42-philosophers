/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prosess_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:32:41 by katakada          #+#    #+#             */
/*   Updated: 2025/04/04 15:57:15 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static t_lltime	calc_initial_eat_at(t_philo *philo)
{
	const int	time_to_eat_ms = philo->g_dup->eating_time;
	const int	n = philo->g_dup->num_of_philos;
	const int	same_time_max_eat = n / 2;
	const int	offset_unit_time = time_to_eat_ms / same_time_max_eat;
	const int	offset_unit_count = (same_time_max_eat * philo->id) % n;

	// 人数が0か1の場合＝待機時間0
	if (same_time_max_eat == 0)
		return (philo->g_dup->start_time);
	return (philo->g_dup->start_time + (offset_unit_time * offset_unit_count));
}

void	barrier_wait_for_philo(t_philo *philo)
{
	// printf("%lld start child philo_id: %d\n", get_time_in_us()
	// 	- philo->g_dup->start_time * 1000, philo->id);
	// 子プロセス
	// 準備完了を通知
	handle_e(sem_post(philo->g_dup->barrier.ready_sem), E_SEM_P);
	// 開始信号を待つ
	handle_e(sem_wait(philo->g_dup->barrier.start_sem), E_SEM_W);
	philo->next_meal_time = calc_initial_eat_at(philo);
	philo->last_meal_start_time = philo->g_dup->start_time;
	// セマフォをクローズ
	handle_e(sem_close(philo->g_dup->barrier.ready_sem), E_SEM_C);
	handle_e(sem_close(philo->g_dup->barrier.start_sem), E_SEM_C);
	// printf("%lld run philo_id: %d\n", get_time_in_us()
	// 	- philo->g_dup->start_time * 1000, philo->id);
}

void	exec_philo_prosess(t_philo *philo)
{
	barrier_wait_for_philo(philo);
	pthread_mutex_init(&philo->m_mutex, NULL);
	if (pthread_create(&philo->main, NULL, philo_rutine, philo) != 0)
		put_error_exit("Error: philo pthread_create\n");
	if (pthread_create(&philo->monitor, NULL, monitor_rutine, philo) != 0)
		put_error_exit("Error: monitor pthread_create\n");
	if (pthread_join(philo->monitor, NULL) != 0)
		put_error_exit("Error: monitor pthread_join\n");
	if (pthread_join(philo->main, NULL) != 0)
		put_error_exit("Error: philo pthread_join\n");
	exit(EXIT_SUCCESS);
}
