/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 00:52:11 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/12 12:01:25 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>
#include "util.h"

static int	philo_think(t_philo *philo, t_table *table)
{
	long long	last_meal;
	int			time_to_death;

	if (table_is_serving(table) && !philo_isdead(philo, table, &last_meal))
	{
		time_to_death = time_to_die(philo, last_meal);
		logger(table, "is thinking", philo->id);
		if (time_to_death < philo->think_time)
			millisleep(time_to_death - 10);
		if (philo->think_time > 0)
			millisleep(philo->think_time);
		else
			usleep(50);
		return (1);
	}
	return (0);
}

static int	philo_eat(t_philo *philo, t_table *table)
{
	if (table_is_serving(table) && !philo_isdead(philo, table, 0))
	{
		logger(table, "is eating", philo->id);
		pthread_mutex_lock(&philo->lock);
		if (philo->times_to_eat > 0)
			philo->times_to_eat--;
		philo->last_meal_time = get_time_millis();
		pthread_mutex_unlock(&philo->lock);
		if (philo->death_time < philo->eat_time)
			millisleep(philo->death_time + 2);
		else
			millisleep(philo->eat_time);
		return (philo->times_to_eat != 0);
	}
	return (0);
}

static int	philo_take_forks(t_philo *philo, t_table *table)
{
	int			forks[2];
	int			ret;

	ret = 0;
	philo_choose_forks(philo, table, forks);
	if (table_is_serving(table) && !philo_isdead(philo, table, 0))
	{
		pthread_mutex_lock(&table->cutlery_arr[forks[0]].mutex);
		if (!philo_isdead(philo, table, 0))
			logger_f(table, "has taken a fork", philo->id, forks[0]);
		while (forks[0] == forks[1] && !philo_isdead(philo, table, 0))
			;
		if (!philo_isdead(philo, table, 0))
		{
			pthread_mutex_lock(&table->cutlery_arr[forks[1]].mutex);
			if (!philo_isdead(philo, table, 0))
				logger_f(table, "has taken a fork", philo->id, forks[1]);
			ret = philo_eat(philo, table);
			pthread_mutex_unlock(&table->cutlery_arr[forks[1]].mutex);
		}
		pthread_mutex_unlock(&table->cutlery_arr[forks[0]].mutex);
	}
	return (ret);
}

static int	philo_sleep(t_philo *philo, t_table *table)
{
	long long	last_meal;
	int			time_to_death;

	if (table_is_serving(table) && !philo_isdead(philo, table, &last_meal))
	{
		time_to_death = time_to_die(philo, last_meal);
		logger(table, "is sleeping", philo->id);
		if (time_to_death < philo->sleep_time)
			millisleep(time_to_death + 2);
		else
			millisleep(philo->sleep_time);
		return (1);
	}
	return (0);
}

void	*philo_routine(void *args)
{
	t_table	*table;
	t_philo	*philo;

	table = ((t_table **) args)[1];
	philo = ((t_philo **) args)[0];
	free(args);
	millisleep((philo->id % 2 == 0) * (philo->death_time / 5));
	while (philo_take_forks(philo, table) \
		&& philo_sleep(philo, table) \
		&& philo_think(philo, table))
		;
	pthread_mutex_lock(&philo->lock);
	{
		if (philo->times_to_eat == 0)
		{
			pthread_mutex_lock(&table->table_lock);
			table->hungry_philos--;
			pthread_mutex_unlock(&table->table_lock);
		}
	}
	pthread_mutex_unlock(&philo->lock);
	return (0);
}
