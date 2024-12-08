/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:37:44 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/08 03:15:38 by maurodri         ###   ########.fr       */
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
		philo->should_log = 0;
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
