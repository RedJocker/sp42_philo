/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:09:47 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/11 06:40:18 by maurodri         ###   ########.fr       */
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
	t_cutlery		*cutlery_arr;
	long long		init_time;
	int				should_log;
	pthread_mutex_t	log_lock;
	pthread_mutex_t	table_lock;
	int				any_death;
	int				hungry_philos;
	pthread_t		thread;
}	t_table;

int		table_is_serving(t_table *table);
void	table_init(t_table *table, t_phargs *args);
void	*table_serve(t_table *table);
void	table_clean(t_table *table);

#endif
