/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:37:44 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 12:00:57 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include "util_bonus.h"
#include <limits.h>
#include <semaphore.h>
#include <stdio.h>

int	philo_is_dead(t_philo *philo, t_table *table)
{
	long long	time;
	int			is_dead;

	sem_wait(philo->philo_lock);
	time = get_time_millis();
	if (philo->times_to_eat != 0
		&& time - philo->last_meal_time > philo->death_time)
	{
		philo->is_dead = 1;
		is_dead = 1;
	}
	else
		is_dead = 0;
	sem_post(philo->philo_lock);
	return (is_dead);
}

int	philo_has_to_leave(t_philo *philo, t_table *table)
{
	long long	time;
	long long	time_locked;
	int			has_to_leave;

	sem_wait(philo->philo_lock);
	{
		time = get_time_millis();
		time_locked = time - philo->lock_time;
		has_to_leave = time_locked > 50 || philo->is_dead;
	}
	sem_post(philo->philo_lock);
	return (has_to_leave);
}

int	philo_has_finished(t_philo *philo, t_table *table)
{
	int	has_finished;

	sem_wait(philo->philo_lock);
	{
		has_finished = philo->times_to_eat == 0;
	}
	sem_post(philo->philo_lock);
	return (has_finished);
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
