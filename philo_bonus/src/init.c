/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:28:21 by katakada          #+#    #+#             */
/*   Updated: 2025/04/06 17:05:13 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_bool	init_memory_space(t_shared_dup *s)
{
	s->philos = malloc(sizeof(t_philo) * s->g_dup.num_of_philos);
	if (s->philos == NULL)
		return (put_error_exit("Error: malloc error\n"), FALSE);
	return (TRUE);
}

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

static void	init_philo(t_shared_dup *s)
{
	int	i;

	i = 0;
	while (i < s->g_dup.num_of_philos)
	{
		s->philos[i].id = i;
		s->philos[i].state = THINKING;
		s->philos[i].meals_eaten = 0;
		s->philos[i].can_touch_me = get_sem(P_SEM_CAN_ACTION, 1);
		s->philos[i].g_dup = &s->g_dup;
		s->philos[i].next_meal_time = calc_initial_eat_at(&s->philos[i]);
		s->philos[i].last_meal_start_time = s->g_dup.start_time;
		i++;
	}
}

static t_lltime	calc_optimal_interval_ms(t_shared_dup *s)
{
	const int	n = s->g_dup.num_of_philos;
	const int	same_time_max_eat = s->g_dup.num_of_philos / 2;
	const int	offset_unit_time = s->g_dup.eating_time / same_time_max_eat;
	const int	required_offset_time = offset_unit_time * n;

	// 人数が0か1の場合＝待機時間0
	if (same_time_max_eat == 0)
		return ((s->g_dup.eating_time + s->g_dup.sleeping_time));
	// 人数が2以上の場合＝待機時間必要
	if (required_offset_time < (s->g_dup.eating_time + s->g_dup.sleeping_time))
		return (s->g_dup.eating_time + s->g_dup.sleeping_time);
	else
		return (required_offset_time);
}

void	init_shared_dup(t_shared_dup *s)
{
	s->g_dup.meal_interval_time = calc_optimal_interval_ms(s);
	s->g_dup.forks = get_sem(G_SEM_FORKS, s->g_dup.num_of_philos);
	s->g_dup.waiters = get_sem(G_SEM_WAITERS, s->g_dup.num_of_philos / 2);
	s->g_dup.can_log = get_sem(G_SEM_LOG, 1);
	s->g_dup.can_log_dead = get_sem(G_SEM_LOG_DEAD, 1);
	s->g_dup.start_time = get_time_in_ms() + SYNC_DILAY_TIME_MS;
	init_philo(s);
}
