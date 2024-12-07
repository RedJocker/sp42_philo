/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:19:21 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 08:13:01 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include "table_bonus.h"
#include "util_bonus.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include "util_bonus.h"

static void	philo_think(t_philo *philo, t_table *table)
{
	if (!philo_is_dead(philo, table))
		logger(table, "is thinking", philo);
	millisleep(philo->think_time);
}

static void	philo_eat(t_philo *philo, t_table *table)
{
	if (!philo_is_dead(philo, table))
	{
		logger(table, "is eating", philo);
		sem_wait(philo->philo_lock);
		{
			philo->last_meal_time = get_time_millis();
		}
		sem_post(philo->philo_lock);
		millisleep(philo->eat_time);
	}
}

static void	philo_take_forks(t_philo *philo, t_table *table)
{
	int has_finished_eating;

	sem_wait(philo->philo_lock);
	{
		has_finished_eating = philo->has_finished_eating;
	}
	sem_post(philo->philo_lock);
	if (!philo_is_dead(philo, table) && !has_finished_eating)
	{
		sem_wait(table->cutlery_sem);
		if (!philo_is_dead(philo, table))
			logger(table, "has taken a fork", philo);
		if (!philo_is_dead(philo, table))
		{
			sem_wait(table->cutlery_sem);
			if (!philo_is_dead(philo, table))
				logger(table, "has taken a fork", philo);
			philo_eat(philo, table);
			sem_post(table->cutlery_sem);
		}
		sem_post(table->cutlery_sem);
	}
	sem_wait(philo->philo_lock);
	{
		if (philo->times_to_eat > 0)
			philo->times_to_eat--;
		philo->has_finished_eating = philo->times_to_eat == 0;
	}
	sem_post(philo->philo_lock);
}

static void	philo_sleep(t_philo *philo, t_table *table)
{
	if (!philo_is_dead(philo, table))
	{
		logger(table, "is sleeping", philo);
		millisleep(philo->sleep_time);
	}
}

void	philo_routine(void *args)
{
	t_table	*table;
	t_philo	*philo;

	table = ((t_table **) args)[1];
	philo = ((t_philo **) args)[0];
	printf("philo_routine %d\n", philo->id);
	free(args);
	while (!philo->is_dead && philo->times_to_eat != 0)
	{
		philo_with_seat_do(philo_take_forks, philo, table);
		philo_with_seat_do(philo_sleep, philo, table);
		philo_with_seat_do(philo_think, philo, table);
	}
}
