/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:09:47 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/02 17:09:27 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TABLE_H
# define TABLE_H

# include <pthread.h>
# include "philo.h"
# include "cutlery.h"

typedef struct s_table
{
	int				num_philos;
	t_philo			*philo_arr;
	sem_t			*cutlery_sem;
	long long		init_time;
	pthread_mutex_t	log_lock;
	pthread_mutex_t	table_lock;
	int				any_death;
	int				hungry_philos;
}	t_table;

int		table_is_serving(t_table *table);
void	table_init(t_table *table, t_phargs *args);
int		table_is_serving(t_table *table);
void	table_serve(t_table *table);
void	table_clean(t_table *table);

#endif
