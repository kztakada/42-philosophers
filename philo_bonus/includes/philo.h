/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:26:56 by katakada          #+#    #+#             */
/*   Updated: 2025/03/31 19:34:07 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "./for_test.h" // for test
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define MALLOC_ERR_MSG "Error: malloc failed\n"
# define ARGV_ERR_MSG "Error: Invalid arguments\n"

typedef long long	t_lltime;

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1,
}					t_bool;

enum				e_philo_state
{
	THINKING,
	EATING,
	SLEEPING,
};

typedef struct s_g_shared
{
	int				num_of_philos;
	t_lltime		survival_time_per_meal;
	t_lltime		eating_time;
	t_lltime		sleeping_time;
	int				required_meals;
	// pthread_mutex_t		g_mutex;
	// t_bool				*fork_in_use;
	t_lltime		meal_interval_time;
	// t_lltime			wait_threshold_us;
	t_lltime		start_time;
	// t_monitor			monitor;
	// t_barrier			barrier;
}					t_g_shared;

typedef struct s_shared
{
	t_g_shared		g_s;
	// t_philo				*philos;
}					t_shared;

// parse_argv__util.c
int					ft_strncmp(const char *str1_src, const char *str2_src,
						size_t cmp_chars);

t_bool				is_under_int_max_min(const char *str, int sign, int digits);
int					ft_isdigit(int c);

// parse_argv.c
t_bool				parse_argv(t_shared *s, int argc, char *argv[]);

// put_error.c
void				put_error(char *err_str);

#endif