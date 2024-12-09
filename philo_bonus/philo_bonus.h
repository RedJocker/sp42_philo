/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:30:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/09 10:53:32 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

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
	int				think_time;
	char			*philo_lock_name;
	sem_t			*philo_lock;
	int				is_dead;
	int				should_log;
	long long		last_meal_time;
	int				times_to_eat;
	pthread_t		routine;
	pthread_t		monitoring;
}	t_philo;

void		philo_routine(void *args);
int			philo_sit_table(t_table *table, t_phargs *args, int id);
int			philo_with_seat_do(
				void (*action) (t_philo*, t_table*),
				t_philo *philo,
				t_table *table);
int			philo_is_dead(
				t_philo *philo, t_table *table, long long *out_time_to_die);
long long	philo_sit_down(t_philo *philo);
int			philo_has_finished(t_philo *philo, t_table *table);
void		philo_monitor(void *args);
void		philo_logger(t_table *table, char *message, t_philo *philo);
void		log_death(t_table *table, int philo_id);

#endif
