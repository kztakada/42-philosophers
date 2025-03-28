/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:26:56 by katakada          #+#    #+#             */
/*   Updated: 2025/03/29 01:01:41 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "./for_test.h" // for test
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

// for test only ////////////////////////////////

// # define NUM_PHILOSOPHERS 199                    // 人数
// # define SURVIVAL_TIME_PER_MEAL (t_lltime)(200) // 生存時間 ms
// # define EATING_TIME (90 * 1000)                 // 食事時間 ms * 1000
// # define THINKING_TIME (90 * 1000)               // 寝る時間 ms * 1000
// # define REQUIRED_MEALS 10                       // 食事回数 ms
// # define NUM_PHILOSOPHERS 5 // 人数
// # define SURVIVAL_TIME_PER_MEAL (t_lltime)510 // 生存時間 ms
// # define EATING_TIME_MS 200 // 食事時間 ms * 1000
// # define SLEEPING_TIME_MS 200 // 寝る時間 ms * 1000
// # define REQUIRED_MEALS 0 // 食事回数 ms

# define NUM_PHILOSOPHERS 4                   // 人数
# define SURVIVAL_TIME_PER_MEAL (t_lltime)300 // 生存時間 ms
# define EATING_TIME_MS 200                   // 食事時間 ms * 1000
# define SLEEPING_TIME_MS 101                 // 寝る時間 ms * 1000
# define REQUIRED_MEALS 10                    // 食事回数 ms

# define RETRAY_TIME_US 50

///////////////////////////////////////////////

# define NUM_MONITOR_THREAD 1
# define MONITOR_INTERVAL_US 1000

typedef long long		t_lltime;
typedef struct s_phiro	t_philo;

enum					e_philo_state
{
	THINKING,
	EATING,
	SLEEPING,
};

typedef struct s_barrier
{
	pthread_mutex_t		b_mutex;
	int					thread_count;
	int					arrived_count;
}						t_barrier;

typedef struct s_monitor
{
	pthread_mutex_t		m_mutex;
	bool				is_finished;
}						t_monitor;

typedef struct s_g_shared
{
	int					num_of_philos;
	t_lltime			survival_time_per_meal;
	t_lltime			eating_time;
	t_lltime			sleeping_time;
	int					required_meals;
	t_lltime			prioryty_wait_time;
	t_lltime			wait_threshold_time;
	pthread_mutex_t		g_mutex;
	bool				*fork_in_use;
	t_lltime			meal_interval_time;
	t_lltime			start_time;
	t_monitor			monitor;
	t_barrier			barrier;
}						t_g_shared;

struct					s_phiro
{
	int					id;
	enum e_philo_state	state;
	int					meals_eaten;
	t_lltime			wait_start_us;
	t_lltime			last_meal_satart_time;
	t_lltime			next_meal_time;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		p_mutex;
	t_g_shared			*g_s;
	t_philo				*other_philos;
};

typedef struct s_shared
{
	t_g_shared			g_s;
	t_philo				*philos;
}						t_shared;

// banker.c
bool					reserve_forks(t_philo *philo);
void					unreseve_forks(t_philo *philo);

// thread__monitor.c
void					*monitor_routine(void *arg);

// thread__philo_util.c
void					sleep_until_next_mealtime(t_lltime next_time);
void					sleep_from_now(t_lltime sleep_time_ms);
void					barrier_wait_for_philo(t_philo *philo);
int						right_philo_id(int philo_id);
int						left_philo_id(int philo_id);
void					print_log_if_alive(t_philo *philo, char *msg);

// thread__philo.c
void					*philosopher_routine(void *arg);

// thread__util.c
void					barrier_wait(t_barrier *barrier);

bool					safe_is_finished(t_g_shared *g_s);
int						philo_name(int philo_id);
t_lltime				get_last_alive_time_us(t_philo *philo);

// time__util.c
t_lltime				get_time_in_ms(void);
t_lltime				get_time_in_us(void);

// util.c
int						left_fork(int philo_id);
int						right_fork(int philo_id);

#endif