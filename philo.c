/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:22:26 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/24 22:54:54 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

int	philo_isdead(t_philo *philo, t_table *table)
{
	long long	time;
	int			should_log;
	int			is_dead;

	pthread_mutex_lock(&philo->lock);
	time = get_time_millis() - table->init_time;
	if (philo->times_to_eat > 0
		&& time - philo->last_meal_time > philo->death_time)
	{
		pthread_mutex_lock(&table->table_lock);
		should_log = !(table->any_death || philo->is_dead);
		table->any_death = 1;
		pthread_mutex_unlock(&table->table_lock);
		philo->is_dead = 1;
		if (should_log)
			logger(table, "died", philo->id);
		is_dead = 1;
	}
	else
		is_dead = 0;
	pthread_mutex_unlock(&philo->lock);
	return (is_dead);
}

void	philo_think(t_philo *philo, t_table *table)
{
	if (!philo_isdead(philo, table))
		logger(table, "is thinking", philo->id);
	usleep(1000);
}

void	philo_eat(t_philo *philo, t_table *table)
{
	if (!philo_isdead(philo, table))
	{
		logger(table, "is eating", philo->id);
		pthread_mutex_lock(&philo->lock);
		if (philo->times_to_eat > 0)
			philo->times_to_eat--;
		philo->last_meal_time = -table->init_time + get_time_millis();
		pthread_mutex_unlock(&philo->lock);
		millisleep(philo->eat_time);
	}
}

void	philo_choose_forks(t_philo *philo, t_table *table, int *forks)
{
	if (philo->id % 2 == 1)
	{
		forks[0] = (philo->id - 1 + table->num_philos) % table->num_philos;
		forks[1] = (philo->id + table->num_philos) % table->num_philos;
	}
	else
	{
		forks[0] = (philo->id + table->num_philos) % table->num_philos;
		forks[1] = (philo->id - 1 + table->num_philos) % table->num_philos;
	}
}

void	philo_take_forks(t_philo *philo, t_table *table)
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

void	philo_sleep(t_philo *philo, t_table *table)
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
		logger(table, "is not hungry", philo->id);
		table->hungry_philos--;
		pthread_mutex_unlock(&table->table_lock);
	}
}

void	philo_clean(t_philo *philo)
{
	pthread_mutex_destroy(&philo->lock);
}

void	philo_init(t_philo *philo, t_philo_args *args, int i, t_table *table)
{
	philo->id = i + 1;
	philo->times_to_eat = args->times_to_eat;
	philo->death_time = args->time_to_die;
	philo->sleep_time = args->time_to_sleep;
	philo->eat_time = args->time_to_eat;
	philo->last_meal_time = 0;
	philo->is_dead = 0;
	pthread_mutex_init(&philo->lock, 0);
}

void	philo_spawn(t_philo *philo, t_table *table)
{
	void	**arr;

	arr = malloc(2 * sizeof(void *));
	arr[0] = philo;
	arr[1] = table;
	pthread_create(&philo->thread, 0, (void *(*)(void *)) philo_routine, arr);
}
