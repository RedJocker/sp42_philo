/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:31:05 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 12:00:37 by maurodri         ###   ########.fr       */
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
