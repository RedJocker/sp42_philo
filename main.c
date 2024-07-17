/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 17:37:36 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/17 19:03:42 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_philo
{
	int			id;
	int			sleep_time;
	int			eat_time;
	int			death_time;
	long		last_meal_time;
	int			times_to_eat;
	pthread_t	thread;
}	t_philo;

typedef struct s_cutlery
{
	pthread_mutex_t	mutex;
	int				owned_by;
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
	int time_to_sleep;
	int	times_to_eat;
}	t_philo_args;

long long get_time_millis()
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

void	philo_think(t_philo *philo, t_table *table)
{
	logger(table, "is thinking", philo->id);
}

void	philo_eat(t_philo *philo, t_table *table)
{
	logger(table, "is eating", philo->id);
	philo->times_to_eat--;
	millisleep(philo->eat_time);
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

void	philo_take_forks(
	t_philo *philo, t_table *table, void (*eatfun) (t_philo*, t_table*))
{
	int			forks[2];

	philo_choose_forks(philo, table, forks);
	pthread_mutex_lock(&table->cutlery_arr[forks[0]].mutex);
	{
		table->cutlery_arr[forks[0]].owned_by = philo->id;
		logger(table, "has taken a fork", philo->id);
		pthread_mutex_lock(&table->cutlery_arr[forks[1]].mutex);
		{
			logger(table, "has taken a fork", philo->id);
			table->cutlery_arr[forks[1]].owned_by = philo->id;
			eatfun(philo, table);
			table->cutlery_arr[forks[1]].owned_by = 0;
		}
		pthread_mutex_unlock(&table->cutlery_arr[forks[1]].mutex);
		table->cutlery_arr[forks[0]].owned_by = 0;
	}
	pthread_mutex_unlock(&table->cutlery_arr[forks[0]].mutex);
}

void	philo_sleep(t_philo *philo, t_table *table)
{
	logger(table, "is sleeping", philo->id);
	millisleep(philo->sleep_time);
}

void	philo_routine(void *args)
{
	t_table	*table;
	t_philo	*philo;

	table = ((t_table **) args)[1];
	philo = ((t_philo **) args)[0];
	free(args);
	while (philo->times_to_eat != 0)
	{
		philo_take_forks(philo, table, philo_eat);
		philo_sleep(philo, table);
		philo_think(philo, table);
	}
	pthread_mutex_lock(&table->table_lock);
	{
		logger(table, "is not hungry", philo->id);
		table->hungry_philos--;
	}
	pthread_mutex_unlock(&table->table_lock);
}

void	philo_init(t_philo *philo, t_philo_args *args, int i, t_table *table)
{
	printf("philo_init %d\n", i + 1);
	philo->id = i + 1;
	philo->times_to_eat = args->times_to_eat;
	philo->death_time = args->time_to_die;
	philo->sleep_time = args->time_to_sleep;
	philo->eat_time = args->time_to_eat;
	philo->last_meal_time = 0;
}

void	philo_spawn(t_philo *philo, t_table *table)
{
	void	**arr;

	arr = malloc(2 * sizeof(void *));
	arr[0] = philo;
	arr[1] = table;
	pthread_create(&philo->thread, 0, (void *(*)(void *)) philo_routine, arr);
	pthread_detach(philo->thread);
}


void	cutlery_init(t_cutlery *cutlery, t_philo_args *args)
{
	cutlery->owned_by = 0;
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

void	table_serve(t_table *table)
{
	int i;

	i = -1;
	table->init_time = get_time_millis();
	while (++i < table->num_philos)
		philo_spawn(table->philo_arr + i, table);
	while (1)
	{
		pthread_mutex_lock(&table->table_lock);
		if (table->any_death || table->hungry_philos <= 0)
			break ;
		pthread_mutex_unlock(&table->table_lock);
		usleep(1);
	}
	pthread_mutex_unlock(&table->table_lock);
}

void	table_clean(t_table *table)
{
	int	i;

	pthread_mutex_lock(&table->table_lock);
	{
		i = -1;
		while (++i < table->num_philos)
			cutlery_clean(table->cutlery_arr + i);
		free(table->philo_arr);
		free(table->cutlery_arr);
		pthread_mutex_destroy(&table->log_lock);
	}
	pthread_mutex_unlock(&table->table_lock);
	pthread_mutex_destroy(&table->table_lock);
}

void	philo_args_init(t_philo_args *args, int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	args->num_philos = 5;
	args->time_to_die = 1000;
	args->time_to_eat = 100;
	args->times_to_eat = 10;
	args->time_to_sleep = 100;
}

int	main(int argc, char *argv[])
{
	t_table			table;
	t_philo_args	args;

	philo_args_init(&args, argc, argv);
	table_init(&table, &args);
	pthread_create(&table.thread, 0, (void *(*)(void *))table_serve, &table);
	pthread_join(table.thread, 0);
	table_clean(&table);
	return (0);
}
