/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:37:44 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 08:09:22 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include "util_bonus.h"
#include <semaphore.h>
#include <stdio.h>

int	philo_is_dead(t_philo *philo, t_table *table)
{
	long long	time;
	int			is_dead;

	sem_wait(philo->philo_lock);
	time = get_time_millis();
	//printf("is_dead: id: %d time: %lld, last_meal: %lld, - %lld\n",	\
	//	   philo->id, time, philo->last_meal_time, time - philo->last_meal_time);
	if (philo->times_to_eat != 0
		&& time - philo->last_meal_time > philo->death_time)
	{
		philo->is_dead = 1;
		is_dead = 1;
	}
	else
		is_dead = 0;
	sem_post(philo->philo_lock);
	if (is_dead)
		logger(table, "died", philo);
	return (is_dead);
}
