/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 17:37:36 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/24 22:52:54 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include "util.h"
#include "table.h"
#include "philo.h"
#include "cutlery.h"
#include "phargs.h"


long long	get_time_millis(void)
{
	struct timeval	tv;
	long long		time_millis;

	gettimeofday(&tv, 0);
	time_millis = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_millis);
}

void	millisleep(int millis)
{
	usleep(1000 * millis);
}

void	logger(t_table *table, char *message, int philo_id)
{
	long long	uptime;

	pthread_mutex_lock(&table->log_lock);
	{
		uptime = get_time_millis() - table->init_time;
		printf("%lld %d %s\n", uptime, philo_id, message);
	}
	pthread_mutex_unlock(&table->log_lock);
}

void	logger_f(t_table *table, char *message, int philo_id, int fork)
{
	long long	uptime;

	pthread_mutex_lock(&table->log_lock);
	{
		uptime = get_time_millis() - table->init_time;
		printf("%lld %d %s %d\n", uptime, philo_id, message, fork);
	}
	pthread_mutex_unlock(&table->log_lock);
}


int	philo_args_init(t_philo_args *args, int argc, char *argv[])
{
	int	is_ok;

	(void) argc;
	(void) argv;
	if (argc < 5 || argc > 6)
		return (0);
	args->num_philos = ft_atoi_strict(&is_ok, argv[1]);
	if (!is_ok)
		return (0);
	args->time_to_die = ft_atoi_strict(&is_ok, argv[2]);
	if (!is_ok)
		return (0);
	args->time_to_eat = ft_atoi_strict(&is_ok, argv[3]);
	if (!is_ok)
		return (0);
	args->time_to_sleep = 200;
	args->times_to_eat = 100;
	return (1);
}


int	main(int argc, char *argv[])
{
	t_table			table;
	t_philo_args	args;

	if (!philo_args_init(&args, argc, argv))
		return (1);
	table_init(&table, &args);
	pthread_create(&table.thread, 0, (void *(*)(void *))table_serve, &table);
	pthread_join(table.thread, 0);
	table_clean(&table);
	return (0);
}
