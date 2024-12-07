/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:37:44 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 16:21:39 by maurodri         ###   ########.fr       */
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
	if (!philo->is_dead && philo->times_to_eat != 0
		&& time - philo->last_meal_time > philo->death_time)
	{
		philo->is_dead = 1;
		is_dead = 1;
		table->time_of_death = time - table->init_time;
		table->should_log = 0;
		sem_wait(table->log_lock);
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
	sem_wait(table->death_lock);
	printf("%lld %d died\n", table->time_of_death, philo_id);
}

void	logger(t_table *table, char *message, t_philo *philo)
{
	long long	uptime;
	long long	time;

	sem_wait(philo->philo_lock);
	{
		if (!table->should_log)
			return (void) (sem_post(philo->philo_lock));
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

void	*philo_monitor(void *args)
{
	t_table	*table;
	t_philo	*philo;
	int		exit_code;

	table = ((t_table **) args)[1];
	philo = ((t_philo **) args)[0];
	free(args);
	while (1)
	{
		millisleep(5);
		if (philo_has_to_leave(philo, table) || philo_is_dead(philo, table))
		{
			table->exit_code = philo->id;
			break ;
		}
		if (philo_has_finished(philo, table))
		{
			table->exit_code = 0;
			break ;
		}
	}
	return (&table->exit_code);
}
