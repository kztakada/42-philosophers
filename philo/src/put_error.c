/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 13:24:05 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 13:33:29 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	put_error(char *err_str)
{
	ft_putstr_fd(err_str, STDERR_FILENO);
}
