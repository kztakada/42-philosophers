/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 13:24:05 by katakada          #+#    #+#             */
/*   Updated: 2025/04/02 19:34:57 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static size_t	ft_strlen(const char *str_src)
{
	size_t	str_len;

	if (!str_src)
		return (0);
	str_len = 0;
	while (str_src[str_len])
		str_len++;
	return (str_len);
}

static int	ft_putstr_fd(char *str_src, int fd)
{
	int	count;

	count = 0;
	if (!str_src)
		str_src = "(null)";
	count = write(fd, str_src, ft_strlen(str_src));
	return (count);
}

void	put_error_exit(char *err_str)
{
	ft_putstr_fd(err_str, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	handle_e(int func_result, char *err_msg)
{
	if (func_result < 0)
		put_error_exit(err_msg);
}
