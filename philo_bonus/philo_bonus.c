/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 01:10:40 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 16:22:05 by maurodri         ###   ########.fr       */
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
#include <unistd.h>

void	philo_init(t_philo *p, t_phargs *args, int id, long long time_init)
{
	char	*id_str;
	int		id_str_len;

	p->id = id;
	p->sleep_time = args->time_to_sleep;
	p->eat_time = args->time_to_eat;
	p->death_time = args->time_to_die;
	p->think_time = args->time_to_die - args->time_to_eat \
		- args->time_to_sleep - 9;
	p->think_time = (p->think_time <= 0) * 0 + (p->think_time >= 100) * 100 \
		+ (p->think_time > 0 && p->think_time < 100) * p->think_time;
	id_str = ft_itoa(id);
	id_str_len = ft_strlen(id_str);
	p->philo_lock_name = malloc(6 + id_str_len + 1);
	ft_memcpy(p->philo_lock_name, "/philo", 6);
	ft_memcpy(p->philo_lock_name + 6, id_str, id_str_len);
	p->philo_lock_name[6 + id_str_len] = 0;
	free(id_str);
	sem_unlink(p->philo_lock_name);
	p->philo_lock = sem_open(\
		p->philo_lock_name, O_CREAT | O_EXCL, 0777, 1);
	p->is_dead = 0;
	p->last_meal_time = time_init;
	p->lock_time = LLONG_MAX;
	p->times_to_eat = args->times_to_eat;
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
	void	**arr2;

	arr = malloc(2 * sizeof(void *));
	arr[0] = philo;
	arr[1] = table;
	arr2 = malloc(2 * sizeof(void *));
	arr2[0] = philo;
	arr2[1] = table;
	pthread_create(&philo->routine, 0, (void *(*)(void *)) philo_routine, arr);
	pthread_create(&philo->monitoring, 0,\
		(void *(*)(void *)) philo_monitor, arr2);
}

void	philo_with_seat_do(
	void (*action) (t_philo*, t_table*), t_philo *philo, t_table *table)
{
	int	should_leave;
	int	num_locks;

	should_leave = 0;
	sem_wait(philo->philo_lock);
	{
		should_leave = philo->is_dead || philo->times_to_eat == 0;
		if (!should_leave)
			philo->lock_time = get_time_millis();
	}
	sem_post(philo->philo_lock);
	if (should_leave)
		return ;
	sem_wait(table->seat_lock);
	{
		sem_wait(philo->philo_lock);
		{
			philo->lock_time = LLONG_MAX;
			should_leave = philo->is_dead || philo->times_to_eat == 0;
		}
		sem_post(philo->philo_lock);
	}
	sem_post(table->seat_lock);
	if (should_leave)
		return ;
	action(philo, table);
}

int	philo_sit_table(t_table *table, t_phargs *args, int id)
{
	t_philo philo;

	philo_init(&philo, args, id, table->init_time);
	philo_start_dinning(&philo, table);
	pthread_join(philo.monitoring, 0);
	if (philo.is_dead)
		log_death(table, id);
	pthread_join(philo.routine, 0);
	philo_clean(&philo);
	return (table->exit_code);
}
