/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:27:51 by katakada          #+#    #+#             */
/*   Updated: 2025/03/18 18:45:12 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	app_main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	printf("Hello, world!\n");
	return (0);
}

#ifndef TEST

int	main(int argc, char *argv[])
{
	return (app_main(argc, argv));
}
#endif // TEST
