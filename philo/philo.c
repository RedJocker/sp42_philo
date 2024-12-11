/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:22:26 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/11 04:35:00 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include "util.h"

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

int	philo_isdead(t_philo *philo, t_table *table, long long *out_last_meal_time)
{
	long long	time;
	int			should_log;
	int			is_dead;

	pthread_mutex_lock(&philo->lock);
	if (out_last_meal_time)
		*out_last_meal_time = philo->last_meal_time;
	time = get_time_millis();
	if (philo->times_to_eat != 0
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

void	philo_clean(t_philo *philo)
{
	pthread_mutex_destroy(&philo->lock);
}

void	philo_init(t_philo *philo, t_phargs *args, int i)
{
	philo->id = i + 1;
	philo->times_to_eat = args->times_to_eat;
	philo->death_time = args->time_to_die;
	philo->sleep_time = args->time_to_sleep;
	philo->eat_time = args->time_to_eat;
	philo->last_meal_time = 0;
	philo->is_dead = 0;
	pthread_mutex_init(&philo->lock, 0);
	philo->think_time = args->time_to_die - args->time_to_eat \
		- args->time_to_sleep - 10;
	philo->think_time = (philo->think_time <= 0) * 0 \
		+ (philo->think_time >= 31) * 31 \
		+ (philo->think_time > 0 && philo->think_time < 31) * philo->think_time;
}

void	philo_spawn(t_philo *philo, t_table *table)
{
	void	**arr;

	arr = malloc(2 * sizeof(void *));
	arr[0] = philo;
	arr[1] = table;
	philo->last_meal_time = table->init_time;
	pthread_create(&philo->thread, 0, (void *(*)(void *)) philo_routine, arr);
}
