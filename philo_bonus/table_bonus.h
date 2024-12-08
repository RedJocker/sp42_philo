/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:31:04 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/08 03:42:01 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TABLE_BONUS_H
# define TABLE_BONUS_H

# include <pthread.h>
# include <unistd.h>
# include <semaphore.h>
# include "phargs_bonus.h"

typedef struct s_table
{
	sem_t			*cutlery_sem;
	long long		init_time;
	sem_t			*log_lock;
	sem_t			*death_lock;
	pid_t			*philo_pids;
	int				philo_pids_len;
	int				exit_code;
	long long		time_of_death;
}	t_table;

void	table_init(t_table *table, t_phargs *args);
int		table_serve(t_table *table, t_phargs *args);
void	table_clean(t_table *table);

#endif
