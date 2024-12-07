/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:31:05 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 11:32:22 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util_bonus.h"
#include <limits.h>
#include <semaphore.h>
#include <sys/time.h>
#include <pthread.h>
#include "philo_bonus.h"
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

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

void	logger(t_table *table, char *message, t_philo *philo)
{
	long long	uptime;
	long long	time;

	sem_wait(philo->philo_lock);
	{
		time = get_time_millis();
		philo->lock_time = time;
		sem_wait(table->log_lock);
		{
			philo->lock_time = LLONG_MAX;
			uptime = time - table->init_time;
			printf("%lld %d %s\n", uptime, philo->id, message);
		}
		sem_post(table->log_lock);
	}
	sem_post(philo->philo_lock);
}

void	log_death(t_table *table, int philo_id)
{
	long long	uptime;
	long long	time;

	time = get_time_millis();
	sem_wait(table->log_lock);
	{
		uptime = time - table->init_time;
		printf("%lld %d died\n", uptime, philo_id);
	}
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (dst == NULL && src == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((char *) dst)[i] = ((char *) src)[i];
		i++;
	}
	return (dst);
}
