/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 17:37:36 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/16 15:22:27 by maurodri         ###   ########.fr       */
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
	int			num_philos;
	t_philo		*philo_arr;
	t_cutlery	*cutlery_arr;
}	t_table;

typedef struct s_philo_args
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int time_to_sleep;
	int	times_to_eat;
}	t_philo_args;

void	millisleep(int millis)
{
	usleep(1000 * millis);
}

void	logf(char *message, int philo_id)
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	// todo improve time format
	printf("%ld %ld %d %s", tv.tv_sec, tv.tv_usec, philo_id, message);
}

void	philo_think(t_philo *philo)
{
	logf("is thinking\n", philo->id);
}

void	philo_eat(t_philo *philo)
{
	logf("is eating\n", philo->id);
	philo->times_to_eat--;
	millisleep(philo->eat_time);
}

void	philo_take_forks(
	t_philo *philo, t_table *table, void (*eatfun) (t_philo*))
{
	int	forks[2];

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
	pthread_mutex_lock(&table->cutlery_arr[forks[0]].mutex);
	{
		table->cutlery_arr[forks[0]].owned_by = philo->id;
		logf("has taken a fork\n", philo->id);
		pthread_mutex_lock(&table->cutlery_arr[forks[1]].mutex);
		{
			logf("has taken a fork\n", philo->id);
			table->cutlery_arr[forks[1]].owned_by = philo->id;
			eatfun(philo);
			table->cutlery_arr[forks[1]].owned_by = 0;
		}
		pthread_mutex_unlock(&table->cutlery_arr[forks[1]].mutex);
		table->cutlery_arr[forks[0]].owned_by = 0;
	}
	pthread_mutex_unlock(&table->cutlery_arr[forks[0]].mutex);
}

void	philo_sleep(t_philo *philo)
{
	logf("is sleeping\n", philo->id);
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
		philo_sleep(philo);
		philo_think(philo);
	}
}

void	philo_init(t_philo *philo, t_philo_args *args, int i, t_table *table)
{
	void	**arr;

	printf("philo_init %d\n", i + 1);
	philo->id = i + 1;
	philo->times_to_eat = args->times_to_eat;
	philo->death_time = args->time_to_die;
	philo->sleep_time = args->time_to_sleep;
	philo->eat_time = args->time_to_eat;
	philo->last_meal_time = 0;
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

void	table_init(t_table *table, t_philo_args *args)
{
	int	i;

	table->num_philos = args->num_philos;
	table->philo_arr = malloc(args->num_philos * sizeof(t_philo));
	table->cutlery_arr = malloc(args->num_philos * sizeof(t_cutlery));
	i = -1;
	while (++i < args->num_philos)
		cutlery_init(table->cutlery_arr + i, args);
	i = -1;
	while (++i < args->num_philos)
		philo_init(table->philo_arr + i, args, i, table);
}

void	table_clean(t_table *table)
{
	free(table->philo_arr);
	free(table->cutlery_arr);
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
	while (1)
		;
	//destroy mutexes;
	//clean table
	return (0);
}
