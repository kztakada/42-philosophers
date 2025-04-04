/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:32:19 by katakada          #+#    #+#             */
/*   Updated: 2025/04/04 14:09:03 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// philosopherの名前を取得する
int	philo_name(int philo_id)
{
	return (philo_id + 1);
}

void	sleep_until_next_mealtime(t_lltime next_time_ms)
{
	t_lltime	now;
	t_lltime	next_time_us;
	t_lltime	sleep_time;

	now = get_time_in_us();
	next_time_us = next_time_ms * 1000;
	sleep_time = next_time_us - now;
	while (sleep_time > 0)
	{
		usleep((useconds_t)(sleep_time / 2));
		now = get_time_in_us();
		sleep_time = next_time_us - now;
	}
}

void	sleep_from_now(t_lltime sleep_time_ms)
{
	t_lltime	now;
	t_lltime	next_time_us;

	now = get_time_in_us();
	next_time_us = now + (sleep_time_ms * 1000);
	sleep_until_next_mealtime(next_time_us / 1000);
}

// 死亡したログを１度だけ表示させる
static void	unsafe_print_dead_log_only_once(t_lltime death_time_ms,
		t_philo *philo)
{
	const t_lltime	print_time = death_time_ms - philo->g_dup->start_time;

	handle_e(sem_wait(philo->g_dup->can_log_dead), E_SEM_W);
	printf("%lld %d died\n", print_time, philo_name(philo->id));
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
