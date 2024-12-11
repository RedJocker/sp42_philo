/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:15:41 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/11 03:49:40 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include "phargs.h"

typedef struct s_table	t_table;

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

void	*philo_routine(void *args);
int		philo_isdead(
			t_philo *philo, t_table *table, long long *out_last_meal_time);
void	philo_choose_forks(t_philo *philo, t_table *table, int *forks);
void	philo_clean(t_philo *philo);
void	philo_init(t_philo *philo, t_phargs *args, int i);
void	philo_spawn(t_philo *philo, t_table *table);

#endif
