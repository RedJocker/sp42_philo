/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:37:44 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/09 10:53:10 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include "util_bonus.h"
#include <limits.h>
#include <semaphore.h>
#include <stdio.h>

int	philo_is_dead(t_philo *philo, t_table *table, long long *out_time_to_die)
{
	long long	time;
	int			is_dead;
	long long	time_to_die;

	sem_wait(philo->philo_lock);
	time = get_time_millis();
	time_to_die = philo->death_time - (time - philo->last_meal_time);
	if (!philo->is_dead && philo->times_to_eat != 0 && time_to_die <= 0)
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
	if (out_time_to_die)
		*out_time_to_die = time_to_die;
	return (is_dead);
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

void	philo_monitor(void *args)
{
	t_table		*table;
	t_philo		*philo;
	long long	time_to_die;

	table = ((t_table **) args)[1];
	philo = ((t_philo **) args)[0];
	free(args);
	time_to_die = 0;
	while (1)
	{
		millisleep(time_to_die);
		if (philo_is_dead(philo, table, &time_to_die))
		{
			sem_wait(table->exit_lock);
			table->exit_code = philo->id;
			break ;
		}
		if (philo_has_finished(philo, table))
		{
			sem_wait(table->exit_lock);
			table->exit_code = 0;
			break ;
		}
	}
	return ((void)sem_post(table->exit_lock));
}
