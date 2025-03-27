/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread__philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:43:00 by katakada          #+#    #+#             */
/*   Updated: 2025/03/27 19:49:19 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	try_to_eat(int id)
{
	bool		result;
	t_lltime	current_time_us;
	t_lltime	current_time_ms;
	t_lltime	survival_time;
	t_lltime	max_wait;
	int			priority_phil;
	t_lltime	wait_time;

	result = false;
	pthread_mutex_lock(philosophers[id].p_mutex);
	if (philosophers[id].wait_start_us == 0)
	{
		philosophers[id].wait_start_us = get_time_in_us();
	}
	current_time_us = get_time_in_us();
	current_time_ms = current_time_us / 1000;
	survival_time = current_time_ms - philosophers[id].last_meal_satart_time;
	// 死亡確認ブロック
	if (survival_time > SURVIVAL_TIME_PER_MEAL)
	{
		printf("Philosopher %d has starved to death. survival_time: %lld\n", id,
			survival_time);
		pthread_mutex_lock(&m_mutex);
		is_finished = true;
		pthread_mutex_unlock(&m_mutex);
		pthread_mutex_unlock(philosophers[id].p_mutex);
		return (false);
	}
	pthread_mutex_unlock(philosophers[id].p_mutex);
	// 優先度確認ブロック
	max_wait = 0;
	priority_phil = -1;
	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		if (philosophers[i].state == THINKING
			&& philosophers[i].wait_start_us != 0)
		{
			wait_time = current_time_us - philosophers[i].wait_start_us;
			if (wait_time > max_wait)
			{
				max_wait = wait_time;
				priority_phil = i;
			}
		}
	}
	if (max_wait > WAIT_THRESHOLD_MS && priority_phil != -1
		&& priority_phil != id && (get_left_philosopher_id(priority_phil) == id
			|| get_right_philosopher_id(priority_phil) == id))
	{
		printf("Philosopher %d is waiting for too long, \
			giving priority to philosopher %d\n",
				id,
				priority_phil);
		sleep_from_now(PRIORITY_WAIT_TIME_MS);
		return (false);
	}
	// フォーク取得ブロック
	if (reserve_forks(id))
	{
		// 1本目を取得する
		pthread_mutex_lock(philosophers[id].left_fork);
		print_log_if_alive(id, "has taken a fork");
		if (true) // TODO：この間に死亡確認して、死んでいるか？
		{
			// 両方のフォークを取得できた場合
			pthread_mutex_lock(philosophers[id].right_fork);
			print_log_if_alive(id, "has taken a fork");
			pthread_mutex_lock(philosophers[id].p_mutex);
			philosophers[id].state = EATING;
			philosophers[id].wait_start_us = 0;
			philosophers[id].last_meal_satart_time = get_time_in_us() / 1000;
			pthread_mutex_unlock(philosophers[id].p_mutex);
			print_log_if_alive(id, "is eating");
			// printf("Philosopher %d is eating meal %d\n", id,
			// 	philosophers[id].meals_eaten + 1);
			result = true;
		}
		else
		{
			// １本目を取った後、２本目を取る前に死んだ場合
			pthread_mutex_unlock(philosophers[id].left_fork);
			pthread_mutex_lock(&m_mutex);
			is_finished = true;
			pthread_mutex_unlock(&m_mutex);
			result = false;
		}
	}
	return (result);
}

static void	finish_eating(int id)
{
	pthread_mutex_lock(philosophers[id].p_mutex);
	philosophers[id].meals_eaten++;
	philosophers[id].next_meal_time += meal_interval_time;
	pthread_mutex_unlock(philosophers[id].p_mutex);
	unreseve_forks(id);
	pthread_mutex_unlock(philosophers[id].left_fork);
	pthread_mutex_unlock(philosophers[id].right_fork);
	// if (!is_finished)
	// {
	// 	printf("Philosopher %d finished eating (total: %d meals)\n", id,
	// 		philosophers[id].meals_eaten);
	// }
}

static t_lltime	calc_initial_eat_at(int id)
{
	const int	philo_id = id + 1;
	const int	time_to_eat_ms = EATING_TIME_MS;
	const int	n = NUM_PHILOSOPHERS;
	const int	k = NUM_PHILOSOPHERS / 2;
	const int	initial_slot = (k * philo_id) % n;

	if (k == 0)
		return (start_time);
	return (start_time + (time_to_eat_ms * initial_slot / k));
}

void	*philosopher_routine(void *arg)
{
	t_philo	*self;
	int		id;

	self = (t_philo *)arg;
	id = self->id;
	// バリア待機
	barrier_wait(&barrier, id);
	// 次回の食事時間を初期化
	pthread_mutex_lock(philosophers[id].p_mutex);
	philosophers[id].next_meal_time = calc_initial_eat_at(id);
	pthread_mutex_unlock(philosophers[id].p_mutex);
	while (!is_finished)
	{
		if (philosophers[id].state == THINKING)
		{
			print_log_if_alive(id, "is thinking");
			sleep_until_next_mealtime(philosophers[id].next_meal_time);
			while (try_to_eat(id) == false)
			{
				if (is_finished)
					break ;
				usleep(RETRAY_TIME_US);
			}
			philosophers[id].state = EATING;
		}
		else if (philosophers[id].state == EATING)
		{
			sleep_from_now(EATING_TIME_MS);
			finish_eating(id);
			philosophers[id].state = SLEEPING;
			if (philosophers[id].meals_eaten >= REQUIRED_MEALS)
			{
				philosophers[id].wait_start_us = 0;
				// printf("Philosopher %d has finished all %d meals\n", id,
				// 	REQUIRED_MEALS);
				break ;
			}
		}
		else if (philosophers[id].state == SLEEPING)
		{
			print_log_if_alive(id, "is sleeping");
			sleep_from_now(SLEEPING_TIME_MS);
			philosophers[id].state = THINKING;
		}
		if (is_finished)
		{
			break ;
		}
	}
	return (NULL);
}
