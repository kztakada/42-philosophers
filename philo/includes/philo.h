/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:26:56 by katakada          #+#    #+#             */
/*   Updated: 2025/04/08 18:59:02 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define RETRAY_TIME_US 50
# define PRIORITY_WAIT_TIME_US 100

# define NUM_MONITOR_THREAD 1
# define MONITOR_INTERVAL_US 1000

# define MALLOC_ERR_MSG "Error: malloc failed\n"
# define ARGV_ERR_MSG "Error: Invalid arguments\n"

typedef long long		t_lltime;
typedef struct s_phiro	t_philo;

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1,
}						t_bool;

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
	t_bool				is_finished;
}						t_monitor;

typedef struct s_g_shared
{
	int					num_of_philos;
	t_lltime			survival_time_per_meal;
	t_lltime			eating_time;
	t_lltime			sleeping_time;
	int					required_meals;
	pthread_mutex_t		g_mutex;
	t_bool				*fork_in_use;
	t_lltime			meal_interval_time;
	t_lltime			wait_threshold_us;
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
	t_lltime			last_meal_start_time;
	t_lltime			last_sleep_start_time;
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
t_bool					reserve_forks(t_philo *philo);
void					unreseve_forks(t_philo *philo);

// init.c
t_bool					init_memory_space(t_shared *s, pthread_t **philo_thread,
							pthread_mutex_t **forks);
void					init_philos(t_shared *s, pthread_mutex_t *forks);
void					setup_global_params(t_shared *s);

// parse_argv__util.c
int						ft_strncmp(const char *str1_src, const char *str2_src,
							size_t cmp_chars);

t_bool					is_under_int_max_min(const char *str, int sign,
							int digits);
int						ft_isdigit(int c);

// parse_argv.c
t_bool					parse_argv(t_shared *s, int argc, char *argv[]);

// put_error.c
void					put_error(char *err_str);

// thread__monitor.c
void					*monitor_routine(void *arg);

// thread__philo__try_to_eat__has_first_priority.c
t_bool					has_first_priority(t_philo *philo);

// thread__philo__try_to_eat.c
t_bool					try_to_eat(t_philo *philo);

// thread__philo_loop_actions.c
t_bool					done_thinking(t_philo *philo);
void					done_eating(t_philo *philo);
t_bool					is_still_hungry(t_philo *philo);
t_bool					done_sleeping(t_philo *philo);

// thread__philo_util.c
void					sleep_until_next_time(t_lltime next_time);
void					barrier_wait_for_philo(t_philo *philo);
int						right_philo_id(t_philo *philo);
int						left_philo_id(t_philo *philo);
t_bool					print_log_if_alive(t_philo *philo, char *msg,
							t_lltime *store_time);

// thread__philo.c
void					*philosopher_routine(void *arg);

// thread__util.c
void					barrier_wait(t_barrier *barrier);

t_bool					safe_is_finished(t_g_shared *g_s);
int						philo_name(int philo_id);
t_lltime				m_unsafe_get_last_alive_time_us(t_philo *philo);

// tidy_up.c
void					all_mutex_destroy(t_shared *s, pthread_mutex_t *forks);
void					all_free(t_shared *s, pthread_mutex_t *forks,
							pthread_t *philo_thread);

// time__util.c
t_lltime				get_time_in_ms(void);
t_lltime				get_time_in_us(void);

// util.c
int						left_fork(t_philo *philo);
int						right_fork(t_philo *philo);
pthread_mutex_t			*fork_1st(t_philo *philo);
pthread_mutex_t			*fork_2nd(t_philo *philo);

#endif