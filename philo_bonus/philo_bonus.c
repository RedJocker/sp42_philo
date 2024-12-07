/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 01:10:40 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 07:42:35 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include "util_bonus.h"
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void	philo_init(t_philo *philo, t_phargs *args, int id, long long time_init)
{
	char	*id_str;
	int		id_str_len;;

	philo->id = id;
	philo->sleep_time = args->time_to_sleep;
	philo->eat_time = args->time_to_eat;
	philo->death_time = args->time_to_die;
	id_str = ft_itoa(id);
	id_str_len = ft_strlen(id_str);
	philo->philo_lock_name = malloc(6 + id_str_len + 1);
	ft_memcpy(philo->philo_lock_name, "/philo", 6);
	ft_memcpy(philo->philo_lock_name + 6, id_str, id_str_len);
	philo->philo_lock_name[6 + id_str_len] = 0;
	free(id_str);
	sem_unlink(philo->philo_lock_name);
	philo->philo_lock = sem_open(\
		philo->philo_lock_name, O_CREAT | O_EXCL, 0777, 1);
	philo->is_dead = 0;
	philo->last_meal_time = time_init;
	philo->has_finished_eating = 0;
	philo->lock_time = LLONG_MAX;
	philo->times_to_eat = args->times_to_eat;
}

void	philo_clean(t_philo *philo)
{
	sem_close(philo->philo_lock);
	sem_unlink(philo->philo_lock_name);
	free(philo->philo_lock_name);
}

void	philo_start_dinning(t_philo *philo, t_table *table)
{
	void	**arr;

	arr = malloc(2 * sizeof(void *));
	arr[0] = philo;
	arr[1] = table;
	pthread_create(&philo->routine, 0, (void *(*)(void *)) philo_routine, arr);
	pthread_detach(philo->routine);
}

void	philo_with_seat_do(
	void (*action) (t_philo*, t_table*), t_philo *philo, t_table *table)
{
	int	should_leave;
	int num_locks;

	should_leave = 0;
	sem_wait(philo->philo_lock);
	{
		should_leave = philo->is_dead || philo->times_to_eat == 0;
		if (!should_leave)
			philo->lock_time = get_time_millis();
	}
	sem_post(philo->philo_lock);
	if (should_leave)
		return;
	sem_getvalue(table->seat_lock, &num_locks);
	printf("seat_lock id:%d num_locks %d\n", philo->id, num_locks);
	sem_wait(table->seat_lock);
	{
		sem_wait(philo->philo_lock);
		{
			philo->lock_time = LLONG_MAX;
		}
		sem_post(philo->philo_lock);
		sem_getvalue(table->seat_lock, &num_locks);
		printf("seat_locked id:%d num_locks %d\n", philo->id, num_locks);
		action(philo, table);
	}
	sem_post(table->seat_lock);
	printf("seat_unlock %d\n", philo->id);
}

int philo_has_to_leave(t_philo *philo, t_table *table)
{
	long long	time;
	long long	time_locked;
	int			has_to_leave;

	sem_wait(philo->philo_lock);
	{
		time = get_time_millis();
		time_locked = time - philo->lock_time;
		has_to_leave = time_locked > 50;
		//printf("philo_has_to_leave %d time_locked %lld\n", philo->id, time_locked);
	}
	sem_post(philo->philo_lock);
	if (has_to_leave)
		printf("has_to_leave %d\n", philo->id);
	return (has_to_leave);
}

int	philo_has_finished(t_philo *philo, t_table *table)
{
	int	has_finished;

	sem_wait(philo->philo_lock);
	{
		has_finished = philo->times_to_eat == 0;
	}
	sem_post(philo->philo_lock);
	return (has_finished);
}

int		philo_sit_table(t_table *table, t_phargs *args, int id)
{
	t_philo	philo;
	int		exit_code;

	philo_init(&philo, args, id, table->init_time);
	philo_start_dinning(&philo, table);
	while (1)
	{
		millisleep(10);
		if (philo_isdead(&philo,  table) || philo_has_to_leave(&philo, table))
		{
			printf("philo_sit_table exit 1: %d\n", id);
			exit_code = 1;
			break ;
		}
		if (philo_has_finished(&philo, table))
		{
			printf("philo_sit_table exit 0: %d\n", id);
			exit_code = 0;
			break ;
		}
	}
	philo_clean(&philo);
	return (exit_code);
}
