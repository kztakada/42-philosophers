/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:26:56 by katakada          #+#    #+#             */
/*   Updated: 2025/03/27 19:46:47 by katakada         ###   ########.fr       */
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

typedef long long		t_lltime;
// for test only ////////////////////////////////

// # define NUM_PHILOSOPHERS 199                    // 人数
// # define SURVIVAL_TIME_PER_MEAL (t_lltime)(200) // 生存時間 ms
// # define EATING_TIME (90 * 1000)                 // 食事時間 ms * 1000
// # define THINKING_TIME (90 * 1000)               // 寝る時間 ms * 1000
// # define REQUIRED_MEALS 10                       // 食事回数 ms
# define NUM_PHILOSOPHERS 5 // 人数
# define SURVIVAL_TIME_PER_MEAL (t_lltime)510 // 生存時間 ms
# define EATING_TIME_MS 200 // 食事時間 ms * 1000
# define SLEEPING_TIME_MS 200 // 寝る時間 ms * 1000
# define REQUIRED_MEALS 10 // 食事回数 ms

# define RETRAY_TIME_US (50)
# define PRIORITY_WAIT_TIME_MS (EATING_TIME_MS / 4)
# define WAIT_THRESHOLD_MS (SURVIVAL_TIME_PER_MEAL / 3)

///////////////////////////////////////////////

enum					e_philo_state
{
	THINKING,
	EATING,
	SLEEPING,
};

typedef struct s_phiro
{
	int					id;
	enum e_philo_state	state;
	int					meals_eaten;
	t_lltime			wait_start_us;
	t_lltime			last_meal_satart_time;
	t_lltime			next_meal_time;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*p_mutex;
	pthread_t			thread;
}						t_philo;

typedef struct barrier_s
{
	pthread_mutex_t		b_mutex;
	int					thread_count;
	int					arrived_count;
}						t_barrier;

// global variables
// for fork
pthread_mutex_t			forks[NUM_PHILOSOPHERS];

extern pthread_mutex_t	g_mutex;
extern bool				fork_in_use[NUM_PHILOSOPHERS];
extern t_lltime			start_time;
t_lltime				meal_interval_time;

// for philo
pthread_mutex_t			p_mutex[NUM_PHILOSOPHERS];
t_philo					philosophers[NUM_PHILOSOPHERS];

// barrier
t_barrier				barrier;

// for monitor
extern bool				is_finished;
pthread_mutex_t			m_mutex;
////////////////////////

// banker.c
bool					reserve_forks(int philo_id);
void					unreseve_forks(int philo_id);

// thread__philo_util.c
void					sleep_until_next_mealtime(t_lltime next_time);
void					sleep_from_now(t_lltime sleep_time_ms);
void					barrier_wait(t_barrier *barrier, int id);
int						get_right_philosopher_id(int philo_id);
int						get_left_philosopher_id(int philo_id);
void					print_log_if_alive(int philo_id, char *msg);

// thread__philo.c
void					*philosopher_routine(void *arg);

// time__util.c
t_lltime				get_time_in_ms(void);
t_lltime				get_time_in_us(void);

// util.c
int						left_fork(int philo_id);
int						right_fork(int philo_id);

#endif