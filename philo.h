/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:15:41 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/24 22:20:40 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include "table.h"

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

int		philo_isdead(t_philo *philo, t_table *table);
void	philo_think(t_philo *philo, t_table *table);
void	philo_eat(t_philo *philo, t_table *table);
void	philo_choose_forks(t_philo *philo, t_table *table, int *forks);
void	philo_take_forks(t_philo *philo, t_table *table);
void    philo_sleep(t_philo *philo, t_table *table);
void	philo_routine(void *args);
void    philo_clean(t_philo *philo);
void	philo_init(t_philo *philo, t_philo_args *args, int i, t_table *table);
void	philo_spawn(t_philo *philo, t_table *table);
int		philo_args_init(t_philo_args *args, int argc, char *argv[]);

#endif
