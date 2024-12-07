/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:30:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 06:30:00 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <semaphore.h>
# include "phargs_bonus.h"
# include "table_bonus.h"

typedef struct s_philo
{
	int				id;
	int				sleep_time;
	int				eat_time;
	int				death_time;
	char			*philo_lock_name;
	sem_t			*philo_lock;
	int				is_dead;
	long long		last_meal_time;
	long long		lock_time;
	int				times_to_eat;
	int				has_finished_eating;
	pthread_t		routine;
}	t_philo;

void		philo_routine(void *args);
int			philo_sit_table(t_table *table, t_phargs *args, int id);
void		philo_with_seat_do(
				void (*action) (t_philo*, t_table*),
				t_philo *philo,
				t_table *table);
int			philo_isdead(t_philo *philo, t_table *table);
long long	philo_sit_down(t_philo *philo);
int			philo_has_to_leave(t_philo *philo, t_table *table);

#endif
