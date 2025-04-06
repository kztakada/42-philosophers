/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo_loop_actions.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:27:14 by katakada          #+#    #+#             */
/*   Updated: 2025/04/06 20:58:10 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	safe_change_state(t_philo *philo, enum e_philo_state state)
{
	handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	philo->state = state;
	if (state == SLEEPING)
		philo->meals_eaten++;
	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
}

static void	p_unsafe_print_log_if_alive(t_philo *philo, char *msg,
		t_lltime *store_time)
{
	t_lltime	now;
	t_lltime	print_time;

	handle_e(sem_wait(philo->g_dup->can_log), E_SEM_W);
	now = unsafe_get_last_alive_time(philo);
	if (now == -1)
		return ;
	if (store_time != NULL)
		*store_time = now;
	print_time = now - philo->g_dup->start_time;
	printf("%lld %d %s\n", print_time, philo_name(philo->id), msg);
	handle_e(sem_post(philo->g_dup->can_log), E_SEM_P);
}

void	done_thinking(t_philo *philo)
{
	t_lltime	next_meal_time;

	// 考え始める
	p_unsafe_print_log_if_alive(philo, "is thinking", NULL);
	next_meal_time = philo->next_meal_time;
	sleep_until_next_mealtime(next_meal_time);
	// １本目フォーク取得開始
	handle_e(sem_wait(philo->g_dup->waiters), E_SEM_W);
	handle_e(sem_wait(philo->g_dup->forks), E_SEM_W);
	p_unsafe_print_log_if_alive(philo, "has taken a fork", NULL);
	// 2本目フォーク取得開始
	handle_e(sem_wait(philo->g_dup->forks), E_SEM_W);
	p_unsafe_print_log_if_alive(philo, "has taken a fork", NULL);
	handle_e(sem_post(philo->g_dup->waiters), E_SEM_P);
	// フォーク取得完了
	safe_change_state(philo, EATING);
}

void	done_eating(t_philo *philo)
{
	int	eating_time;

	// 食事開始の処理
	handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	printf("");
	p_unsafe_print_log_if_alive(philo, "is eating",
		&philo->last_meal_start_time);
	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	philo->next_meal_time += philo->g_dup->meal_interval_time;
	eating_time = philo->g_dup->eating_time;
	sleep_from_now(eating_time);
	// 食後の処理
	handle_e(sem_post(philo->g_dup->forks), E_SEM_P);
	handle_e(sem_post(philo->g_dup->forks), E_SEM_P);
	safe_change_state(philo, SLEEPING);
}

void	done_sleeping(t_philo *philo)
{
	int	sleeping_time;

	// 睡眠開始
	p_unsafe_print_log_if_alive(philo, "is sleeping", NULL);
	sleeping_time = philo->g_dup->sleeping_time;
	sleep_from_now(sleeping_time);
	// 睡眠終了
	safe_change_state(philo, THINKING);
}
