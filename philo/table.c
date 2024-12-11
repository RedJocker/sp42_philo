/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:24:54 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/11 04:35:39 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "table.h"
#include "util.h"

void	table_init(t_table *table, t_phargs *args)
{
	int	i;

	table->num_philos = args->num_philos;
	table->philo_arr = malloc(args->num_philos * sizeof(t_philo));
	table->cutlery_arr = malloc(args->num_philos * sizeof(t_cutlery));
	table->any_death = 0;
	table->hungry_philos = args->num_philos;
	table->init_time = -1;
	pthread_mutex_init(&table->log_lock, 0);
	pthread_mutex_init(&table->table_lock, 0);
	i = -1;
	while (++i < args->num_philos)
		cutlery_init(table->cutlery_arr + i, args);
	i = -1;
	while (++i < args->num_philos)
		philo_init(table->philo_arr + i, args, i);
}

int	table_is_serving(t_table *table)
{
	int	is_serving;

	pthread_mutex_lock(&table->table_lock);
	{
		is_serving = (!table->any_death && table->hungry_philos > 0);
	}
	pthread_mutex_unlock(&table->table_lock);
	return (is_serving);
}

static int	time_to_sleep(long long *last_meal_time, t_table *table)
{
	long long	min_time;
	int			time_to_sleep;
	int			i;

	min_time = last_meal_time[0];
	i = 0;
	while (++i < table->num_philos)
	{
		if (last_meal_time[i] < min_time)
			min_time = last_meal_time[i];
	}
	time_to_sleep = table->philo_arr[0].death_time \
		- ((int)(get_time_millis() - min_time - table->init_time));
	if (time_to_sleep <= 0)
		return (2);
	else
		return (time_to_sleep);
}

void	*table_serve(t_table *table)
{
	int			i;
	long long	*last_meal;

	last_meal = malloc(table->num_philos * sizeof(long long));
	i = table->num_philos;
	table->init_time = get_time_millis();
	while (--i >= 0)
		philo_spawn(table->philo_arr + i, table);
	while (table_is_serving(table))
	{
		i = 0;
		while (++i < table->num_philos \
				&& !philo_isdead(table->philo_arr + i, table, last_meal + i))
			;
		millisleep(time_to_sleep(last_meal, table));
	}
	free(last_meal);
	return (0);
}

void	table_clean(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->num_philos)
		pthread_join(table->philo_arr[i].thread, 0);
	i = -1;
	while (++i < table->num_philos)
	{
		cutlery_clean(table->cutlery_arr + i);
		philo_clean(table->philo_arr + i);
	}
	free(table->philo_arr);
	free(table->cutlery_arr);
	pthread_mutex_destroy(&table->log_lock);
	pthread_mutex_destroy(&table->table_lock);
}
