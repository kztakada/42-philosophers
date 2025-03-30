/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 00:49:52 by katakada          #+#    #+#             */
/*   Updated: 2025/03/30 15:00:15 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_under_int_digits(const char *str)
{
	int	i;
	int	sign;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	if (is_under_int_max_min(str, sign, i) == false)
		return (false);
	return (true);
}

static long	ascii_to_int(const char *str_pos, int sign)
{
	unsigned long	int_value;
	int				digit_value;
	unsigned long	max_limit;

	int_value = 0;
	max_limit = (unsigned long)LONG_MAX;
	while (ft_isdigit(*str_pos))
	{
		digit_value = *str_pos - '0';
		if (sign == 1 && int_value > ((max_limit - digit_value) / 10))
			return (LONG_MAX);
		if (sign == -1 && int_value > ((max_limit + 1 - digit_value) / 10))
			return (LONG_MIN);
		int_value = int_value * 10 + digit_value;
		str_pos++;
	}
	return (int_value);
}

static int	ft_atoi(const char *str_src)
{
	int			sign;
	long		abs_value;
	const char	*str_pos;

	sign = 1;
	abs_value = 0;
	str_pos = str_src;
	while (*str_pos == ' ' || (*str_pos >= '\t' && *str_pos <= '\r'))
		str_pos++;
	if (*str_pos == '-' || *str_pos == '+')
	{
		if (*str_pos == '-')
			sign = -1;
		str_pos++;
	}
	abs_value = (int)ascii_to_int(str_pos, sign);
	return ((int)(abs_value * sign));
}

static bool	convert_str_to_num(const char *str, int *num)
{
	if (!is_under_int_digits(str))
		return (false);
	*num = ft_atoi(str);
	if (*num < 0)
		return (false);
	return (true);
}

bool	parse_argv(t_shared *s, int argc, char *argv[])
{
	int	num;

	if (argc < 5 || argc > 6)
		return (put_error(ARGV_ERR_MSG), false);
	if (convert_str_to_num(argv[1], &num) == false || num <= 0)
		return (put_error(ARGV_ERR_MSG), false);
	s->g_s.num_of_philos = num;
	if (convert_str_to_num(argv[2], &num) == false || num <= 0)
		return (put_error(ARGV_ERR_MSG), false);
	s->g_s.survival_time_per_meal = (t_lltime)num;
	if (convert_str_to_num(argv[3], &num) == false || num <= 0)
		return (put_error(ARGV_ERR_MSG), false);
	s->g_s.eating_time = (t_lltime)num;
	if (convert_str_to_num(argv[4], &num) == false || num <= 0)
		return (put_error(ARGV_ERR_MSG), false);
	s->g_s.sleeping_time = (t_lltime)num;
	if (argc == 6)
	{
		if (convert_str_to_num(argv[5], &num) == false || num <= 0)
			return (put_error(ARGV_ERR_MSG), false);
		s->g_s.required_meals = num;
	}
	else
		s->g_s.required_meals = -1;
	return (true);
}
