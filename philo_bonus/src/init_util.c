/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:04:19 by katakada          #+#    #+#             */
/*   Updated: 2025/04/06 17:04:58 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#define OWNER_ONLY_WRITE 0644

sem_t	*get_sem(const char *sem_name, int sem_count)
{
	sem_t	*sem;

	sem_unlink(sem_name);
	sem = sem_open(sem_name, O_CREAT | O_EXCL, OWNER_ONLY_WRITE, sem_count);
	if (sem == SEM_FAILED)
		put_error_exit("Error: sem_open\n");
	handle_e(sem_unlink(sem_name), E_SEM_U);
	return (sem);
}
