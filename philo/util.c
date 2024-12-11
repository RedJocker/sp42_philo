/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 00:06:34 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/11 06:47:59 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include <sys/time.h>
#include <pthread.h>
#include "table.h"
#include <stdio.h>
#include <unistd.h>

long long	get_time_millis(void)
{
	struct timeval	tv;
	long long		time_millis;

	gettimeofday(&tv, 0);
	time_millis = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_millis);
}

void	millisleep(int millis)
{
	usleep(1000 * millis);
}

int	logger(t_table *table, char *message, int philo_id)
{
	long long	uptime;

	pthread_mutex_lock(&table->log_lock);
	{
		uptime = get_time_millis() - table->init_time;
		if (table->should_log)
			printf("%lld %d %s\n", uptime, philo_id, message);
	}
	pthread_mutex_unlock(&table->log_lock);
	return (1);
}

void	logger_f(t_table *table, char *message, int philo_id, int fork)
{
	long long	uptime;

	(void) fork;
	pthread_mutex_lock(&table->log_lock);
	{
		uptime = get_time_millis() - table->init_time;
		if (table->should_log)
			printf("%lld %d %s\n", uptime, philo_id, message);
	}
	pthread_mutex_unlock(&table->log_lock);
}
