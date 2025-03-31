/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:27:51 by katakada          #+#    #+#             */
/*   Updated: 2025/03/31 19:32:19 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	app_main(int argc, char **argv)
{
	t_shared	s;

	if (parse_argv(&s, argc, argv) == FALSE)
		return (1);
	printf("num_of_philos: %d\n", s.g_s.num_of_philos);
	printf("survival_time_per_meal: %lld\n", s.g_s.survival_time_per_meal);
	printf("eating_time: %lld\n", s.g_s.eating_time);
	printf("sleeping_time: %lld\n", s.g_s.sleeping_time);
	printf("required_meals: %d\n", s.g_s.required_meals);
	return (0);
}

#ifndef TEST

int	main(int argc, char *argv[])
{
	return (app_main(argc, argv));
}
#endif // TEST
