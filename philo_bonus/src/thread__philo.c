/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 00:27:29 by katakada          #+#    #+#             */
/*   Updated: 2025/04/06 16:16:17 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static t_bool	safe_is_hungry(t_philo *philo)
{
	t_bool	result;

	handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	if (philo->g_dup->required_meals < 0)
		result = TRUE;
	else
		result = (philo->meals_eaten < philo->g_dup->required_meals);
	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	return (result);
}

static t_bool	safe_is_in_state(t_philo *philo, enum e_philo_state state)
{
	t_bool	result;

	handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	result = (philo->state == state);
	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	return (result);
}

static void	safe_change_state(t_philo *philo, enum e_philo_state state)
{
	handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	philo->state = state;
	if (state == SLEEPING)
		philo->meals_eaten++;
	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
}

void	p_unsafe_print_log_if_alive(t_philo *philo, char *msg,
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
	// return (TRUE);
}

t_bool	done_thinking(t_philo *philo)
{
	t_lltime	next_meal_time;

	// 考え始める
	// handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	p_unsafe_print_log_if_alive(philo, "is thinking", NULL);
	// if (p_unsafe_print_log_if_alive(philo, "is thinking", NULL) == FALSE)
	// 	return (handle_e(sem_post(philo->can_touch_me), E_SEM_P), FALSE);
	next_meal_time = philo->next_meal_time;
	// handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	sleep_until_next_mealtime(next_meal_time);
	// １本目フォーク取得開始
	handle_e(sem_wait(philo->g_dup->waiters), E_SEM_W);
	// printf("get waiters id:%d\n", philo_name(philo->id));
	handle_e(sem_wait(philo->g_dup->forks), E_SEM_W);
	// handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	p_unsafe_print_log_if_alive(philo, "has taken a fork", NULL);
	// if (p_unsafe_print_log_if_alive(philo, "has taken a fork",
	// NULL) == FALSE)
	// 	return (handle_e(sem_post(philo->can_touch_me), E_SEM_P),
	// 		handle_e(sem_post(philo->g_dup->waiters), E_SEM_P), FALSE);
	// handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	// 2本目フォーク取得開始
	handle_e(sem_wait(philo->g_dup->forks), E_SEM_W);
	// handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	p_unsafe_print_log_if_alive(philo, "has taken a fork", NULL);
	// if (p_unsafe_print_log_if_alive(philo, "has taken a fork",
	// NULL) == FALSE)
	// 	return (handle_e(sem_post(philo->can_touch_me), E_SEM_P),
	// 		handle_e(sem_post(philo->g_dup->waiters), E_SEM_P), FALSE);
	// handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	handle_e(sem_post(philo->g_dup->waiters), E_SEM_P);
	// フォーク取得完了
	safe_change_state(philo, EATING);
	return (TRUE);
}

void	putdown_forks(t_philo *philo)
{
	handle_e(sem_post(philo->g_dup->forks), E_SEM_P);
	handle_e(sem_post(philo->g_dup->forks), E_SEM_P);
}

t_bool	done_eating(t_philo *philo)
{
	int	eating_time;

	// 食事開始の処理
	handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	printf("");
	p_unsafe_print_log_if_alive(philo, "is eating",
		&philo->last_meal_start_time);
	// printf("");
	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	philo->next_meal_time += philo->g_dup->meal_interval_time;
	eating_time = philo->g_dup->eating_time;
	sleep_from_now(eating_time);
	// 食後の処理
	putdown_forks(philo);
	safe_change_state(philo, SLEEPING);
	return (TRUE);
}

t_bool	done_sleeping(t_philo *philo)
{
	int	sleeping_time;

	// 睡眠開始
	// handle_e(sem_wait(philo->can_touch_me), E_SEM_W);
	p_unsafe_print_log_if_alive(philo, "is sleeping", NULL);
	// if (p_unsafe_print_log_if_alive(philo, "is sleeping", NULL) == FALSE)
	// 	return (handle_e(sem_post(philo->can_touch_me), E_SEM_P), FALSE);
	sleeping_time = philo->g_dup->sleeping_time;
	// handle_e(sem_post(philo->can_touch_me), E_SEM_P);
	sleep_from_now(sleeping_time);
	// 睡眠終了
	safe_change_state(philo, THINKING);
	return (TRUE);
}

// void	barrier_wait_for_philo(t_philo *philo)
// {
// 	printf("%lld wait child philo_id: %d\n", get_time_in_us(),
// 		philo_name(philo->id));
// 	// 子プロセス
// 	// 準備完了を通知
// 	handle_e(sem_post(philo->g_dup->barrier.ready_sem), E_SEM_P);
// 	// 開始信号を待つ
// 	handle_e(sem_wait(philo->g_dup->barrier.start_sem), E_SEM_W);
// 	philo->g_dup->start_time = get_time_in_ms();
// 	philo->next_meal_time = calc_initial_eat_at(philo);
// 	philo->last_meal_start_time = philo->g_dup->start_time;
// 	// セマフォをクローズ
// 	handle_e(sem_close(philo->g_dup->barrier.ready_sem), E_SEM_C);
// 	handle_e(sem_close(philo->g_dup->barrier.start_sem), E_SEM_C);
// 	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
// 	handle_e(sem_post(philo->can_touch_me), E_SEM_P);
// 	printf("%lld start philo_id: %d\n", get_time_in_us(),
// 		philo_name(philo->id));
// 	printf("philo_id: %d start_time: %lld  next_meal_time: %lld calc_e:%lld\n",
// 		philo_name(philo->id), philo->g_dup->start_time, philo->next_meal_time,
// 		philo->next_meal_time - philo->g_dup->start_time);
// }

void	*philo_rutine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// barrier_wait_for_philo(philo);
	sleep_until_next_mealtime(philo->g_dup->start_time);
	while (safe_is_hungry(philo) == TRUE)
	{
		if (safe_is_in_state(philo, THINKING))
		{
			if (done_thinking(philo) == FALSE)
				break ;
		}
		else if (safe_is_in_state(philo, EATING))
		{
			if (done_eating(philo) == FALSE)
				break ;
		}
		else if (safe_is_in_state(philo, SLEEPING))
		{
			if (done_sleeping(philo) == FALSE)
				break ;
		}
		else
			put_error_exit("Error: Invalid philo state\n");
	}
	exit(EXIT_SUCCESS);
}
