/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time__util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakada <katakada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:53:48 by katakada          #+#    #+#             */
/*   Updated: 2025/03/27 17:53:49 by katakada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// 現在時刻をミリ秒で取得する関数
t_lltime	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL));
}

// 現在時刻をマイクロ秒で取得する関数
t_lltime	get_time_in_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000LL) + tv.tv_usec);
}
