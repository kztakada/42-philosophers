/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:26:56 by katakada          #+#    #+#             */
/*   Updated: 2025/04/06 19:00:50 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include "./for_test.h" // for test
# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

# define MALLOC_ERR_MSG "Error: malloc failed\n"
# define ARGV_ERR_MSG "Error: Invalid arguments\n"

// error messages
# define E_SEM_P "Error: sem_post"
# define E_SEM_W "Error: sem_wait"
# define E_SEM_C "Error: sem_close"
# define E_SEM_O "Error: sem_open"
# define E_SEM_U "Error: sem_unlink"

// semaphore names
# define B_SEM_READY "/ready_sem"
# define B_SEM_START "/start_sem"
# define G_SEM_FORKS "/forks"
# define G_SEM_WAITERS "/waiters"
# define G_SEM_LOG "/can_log"
# define G_SEM_LOG_DEAD "/can_log_dead"
# define P_SEM_CAN_ACTION "/can_touch_me"

# define SYNC_DILAY_TIME_MS 2000
# define MONITOR_INTERVAL_US 1000

typedef long long		t_lltime;
typedef struct s_philo	t_philo;

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
	sem_t				*ready_sem;
	sem_t				*start_sem;
}						t_barrier;

typedef struct s_g_shared_duplication
{
	int					num_of_philos;
	t_lltime			survival_time_per_meal;
	t_lltime			eating_time;
	t_lltime			sleeping_time;
	int					required_meals;
	t_lltime			meal_interval_time;
	t_lltime			start_time;
	t_barrier			barrier;

	sem_t				*forks;
	sem_t				*waiters;
	sem_t				*can_log;
	sem_t				*can_log_dead;
}						t_g_dup;

struct					s_philo
{
	int					id;
	enum e_philo_state	state;
	int					meals_eaten;
	t_lltime			last_meal_start_time;
	t_lltime			next_meal_time;

	pthread_t			main;
	pthread_t			monitor;

	pid_t				pid;
	sem_t				*can_touch_me;
	pthread_mutex_t		m_mutex;
	t_g_dup				*g_dup;
};

typedef struct s_shared_duplication
{
	t_g_dup				g_dup;
	t_philo				*philos;
}						t_shared_dup;

// error.c
void					put_error_exit(char *err_str);
void					handle_e(int func_result, char *err_msg);

// handle_prosess.c
void					handle_start_philo_prosess(t_philo *philo);
void					handle_terminate_all_philo_prosess(t_shared_dup *s);

// init__util.c
sem_t					*get_sem(const char *sem_name, int sem_count);

// init.c
t_bool					init_memory_space(t_shared_dup *s);
void					init_shared_dup(t_shared_dup *s);

// parse_argv__util.c
int						ft_strncmp(const char *str1_src, const char *str2_src,
							size_t cmp_chars);

t_bool					is_under_int_max_min(const char *str, int sign,
							int digits);
int						ft_isdigit(int c);

// parse_argv.c
t_bool					parse_argv(t_shared_dup *s, int argc, char *argv[]);

// prosess_philo.c
void					exec_philo_prosess(t_philo *philo);

// thread__monitor.c
void					*monitor_rutine(void *arg);

// thread__philo__util.c
int						philo_name(int philo_id);

// thread__philo_loop_actions.c
t_bool					done_thinking(t_philo *philo);
t_bool					done_eating(t_philo *philo);
t_bool					done_sleeping(t_philo *philo);

// thread__philo.c
void					*philo_rutine(void *arg);

// thread__sleep.c
void					sleep_until_next_mealtime(t_lltime next_time_ms);
void					sleep_from_now(t_lltime sleep_time_ms);

// thread__util.c
void					all_sem_close_at_thread(t_philo *philo);
t_lltime				unsafe_get_last_alive_time(t_philo *philo);

// time__util.c
t_lltime				get_time_in_ms(void);
t_lltime				get_time_in_us(void);

#endif