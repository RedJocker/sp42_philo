/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 17:37:36 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/18 22:28:54 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include "util.h"

typedef struct s_philo
{
	int				id;
	int				sleep_time;
	int				eat_time;
	int				death_time;
	int				is_dead;
	long long		last_meal_time;
	int				times_to_eat;
	pthread_t		thread;
	pthread_mutex_t	lock;
}	t_philo;

typedef struct s_cutlery
{
	pthread_mutex_t	mutex;
}	t_cutlery;

typedef struct s_table
{
	int				num_philos;
	t_philo			*philo_arr;
	t_cutlery		*cutlery_arr;
	long long		init_time;
	pthread_mutex_t	log_lock;
	pthread_mutex_t	table_lock;
	int				any_death;
	int				hungry_philos;
	pthread_t		thread;
}	t_table;

typedef struct s_philo_args
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	times_to_eat;
}	t_philo_args;

int	table_is_serving(t_table *table);

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

void	cutlery_init(t_cutlery *cutlery, t_philo_args *args)
{
	pthread_mutex_init(&cutlery->mutex, 0);
}

void	cutlery_clean(t_cutlery *cutlery)
{
	pthread_mutex_destroy(&cutlery->mutex);
}

void	table_init(t_table *table, t_philo_args *args)
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
		philo_init(table->philo_arr + i, args, i, table);
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

void	table_serve(t_table *table)
{
	int	i;

	i = table->num_philos;
	table->init_time = get_time_millis();
	while (--i >= 0)
		philo_spawn(table->philo_arr + i, table);
	while (table_is_serving(table))
	{
		i = 0;
		while (++i < table->num_philos)
			philo_isdead(&table->philo_arr[i], table);
		millisleep(5);
	}
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
