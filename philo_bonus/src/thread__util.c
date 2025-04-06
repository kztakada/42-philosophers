/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:53:45 by katakada          #+#    #+#             */
/*   Updated: 2025/04/07 00:51:53 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#define SYNC_WAIT_TIME_US 10000

// philosopherの名前を取得する
int	philo_name(int philo_id)
{
	return (philo_id + 1);
}

void	all_sem_close_at_thread(t_philo *philo)
{
	handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	printf("");
	philo->g_dup->is_finished = TRUE;
	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	printf("");
	usleep(SYNC_WAIT_TIME_US);
	handle_e(sem_close(philo->g_dup->forks), E_SEM_C);
	handle_e(sem_close(philo->g_dup->waiters), E_SEM_C);
	handle_e(sem_close(philo->g_dup->can_log), E_SEM_C);
	handle_e(sem_close(philo->g_dup->can_log_dead), E_SEM_C);
	handle_e(sem_close(philo->can_touch_me), E_SEM_C);
}

// 死亡したログを１度だけ表示させる
static void	unsafe_print_dead_log_only_once(t_lltime death_time_ms,
		t_philo *philo)
{
	const t_lltime	print_time = death_time_ms - philo->g_dup->start_time;

	handle_e(sem_wait(philo->g_dup->can_log_dead), E_SEM_W);
	printf("%lld %d died\n", print_time, philo_name(philo->id));
	// all_sem_close_at_thread(philo);
	exit(EXIT_FAILURE);
}

static t_bool	unsafe_is_hungry(t_philo *philo)
{
	if (philo->g_dup->required_meals < 0)
		return (TRUE);
	else
		return (philo->meals_eaten < philo->g_dup->required_meals);
}

t_lltime	unsafe_get_last_alive_time(t_philo *philo)
{
	t_lltime	now;
	t_lltime	survival_time;

	now = get_time_in_ms();
	survival_time = now - philo->last_meal_start_time;
	if (survival_time > philo->g_dup->survival_time_per_meal)
	{
		if (unsafe_is_hungry(philo) == TRUE)
		{
			// 死亡ログを出力していない場合、死亡ログを出力する
			unsafe_print_dead_log_only_once(now, philo);
		}
		return (-1);
	}
	return (now);
}
