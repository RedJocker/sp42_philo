/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 00:52:11 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/09 14:30:37 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>
#include "util.h"

static void	philo_think(t_philo *philo, t_table *table)
{
	if (!philo_isdead(philo, table))
		logger(table, "is thinking", philo->id);
	usleep(1000);
}

static void	philo_eat(t_philo *philo, t_table *table)
{
	if (!philo_isdead(philo, table))
	{
		logger(table, "is eating", philo->id);
		pthread_mutex_lock(&philo->lock);
		if (philo->times_to_eat > 0)
			philo->times_to_eat--;
		philo->last_meal_time = get_time_millis();
		pthread_mutex_unlock(&philo->lock);
		millisleep(philo->eat_time);
	}
}

static void	philo_take_forks(t_philo *philo, t_table *table)
{
	int			forks[2];

	philo_choose_forks(philo, table, forks);
	if (!philo_isdead(philo, table))
	{
		pthread_mutex_lock(&table->cutlery_arr[forks[0]].mutex);
		if (!philo_isdead(philo, table))
			logger_f(table, "has taken a fork", philo->id, forks[0]);
		while (forks[0] == forks[1] && !philo_isdead(philo, table))
			;
		if (!philo_isdead(philo, table))
		{
			pthread_mutex_lock(&table->cutlery_arr[forks[1]].mutex);
			if (!philo_isdead(philo, table))
				logger_f(table, "has taken a fork", philo->id, forks[1]);
			philo_eat(philo, table);
			pthread_mutex_unlock(&table->cutlery_arr[forks[1]].mutex);
		}
		pthread_mutex_unlock(&table->cutlery_arr[forks[0]].mutex);
	}
}

static void	philo_sleep(t_philo *philo, t_table *table)
{
	if (!philo_isdead(philo, table))
	{
		logger(table, "is sleeping", philo->id);
		millisleep(philo->sleep_time);
	}
}

void	philo_routine(void *args)
{
	t_table	*table;
	t_philo	*philo;

	table = ((t_table **) args)[1];
	philo = ((t_philo **) args)[0];
	free(args);
	//TODO: improve loop condition, pobably there is race condition
	while (!philo->is_dead && philo->times_to_eat != 0)
	{
		if (table_is_serving(table))
			philo_take_forks(philo, table);
		if (table_is_serving(table))
			philo_sleep(philo, table);
		if (table_is_serving(table))
			philo_think(philo, table);
		else
			break ;
	}
	if (philo->times_to_eat == 0)
	{
		pthread_mutex_lock(&table->table_lock);
		table->hungry_philos--;
		pthread_mutex_unlock(&table->table_lock);
	}
}
