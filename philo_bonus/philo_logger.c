/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_logger.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 03:09:04 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/08 03:32:08 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "table_bonus.h"
#include "philo_bonus.h"
#include "util_bonus.h"
#include <limits.h>
#include <stdio.h>

void	log_death(t_table *table, int philo_id)
{
	sem_wait(table->death_lock);
	printf("%lld %d died\n", table->time_of_death, philo_id);
}

void	philo_logger(t_table *table, char *message, t_philo *philo)
{
	long long	uptime;
	long long	time;

	sem_wait(philo->philo_lock);
	{
		if (!philo->should_log)
			return ((void)(sem_post(philo->philo_lock)));
		time = get_time_millis();
		philo->lock_time = time;
		sem_wait(table->log_lock);
		{
			philo->lock_time = LLONG_MAX;
			uptime = time - table->init_time;
			printf("%lld %d %s\n", uptime, philo->id, message);
		}
		if (!philo->is_dead)
			sem_post(table->log_lock);
	}
	sem_post(philo->philo_lock);
}
