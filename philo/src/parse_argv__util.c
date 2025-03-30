/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv__util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:44:05 by katakada          #+#    #+#             */
/*   Updated: 2025/03/31 01:05:59 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strncmp(const char *str1_src, const char *str2_src, size_t cmp_chars)
{
	unsigned char	*s1_char;
	unsigned char	*s2_char;
	size_t			compare_count;

	s1_char = (unsigned char *)str1_src;
	s2_char = (unsigned char *)str2_src;
	compare_count = 0;
	while (cmp_chars > compare_count++)
	{
		if (*s1_char != *s2_char || !*s1_char || !*s2_char)
			return (*s1_char - *s2_char);
		s1_char++;
		s2_char++;
	}
	return (0);
}

t_bool	is_under_int_max_min(const char *str, int sign, int digits)
{
	if (digits > 10)
		return (FALSE);
	if (digits == 10 && sign == 1 && ft_strncmp(str, "2147483647", 10) > 0)
		return (FALSE);
	if (digits == 10 && sign == -1 && ft_strncmp(str, "2147483648", 10) > 0)
		return (FALSE);
	return (TRUE);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
