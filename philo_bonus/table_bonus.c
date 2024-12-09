/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:17:41 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/09 03:53:48 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "table_bonus.h"
#include "philo_bonus.h"
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "util_bonus.h"

void	table_init(t_table *table, t_phargs *args)
{
	table->cutlery_sem = ft_sem_open("/cutlery_sem", args->num_philos);
	table->log_lock = ft_sem_open("/log_lock", 1);
	table->death_lock = ft_sem_open("/death_lock", 1);
	table->philo_pids = malloc(args->num_philos * sizeof(pid_t));
	table->philo_pids_len = args->num_philos;
	table->time_of_death = 0;
	table->init_time = get_time_millis();
}

void	table_clean(t_table *table)
{
	sem_close(table->cutlery_sem);
	sem_close(table->log_lock);
	sem_close(table->death_lock);
	sem_unlink("/cutlery_sem");
	sem_unlink("/log_lock");
	sem_unlink("/death_lock");
	free(table->philo_pids);
}

static int	table_receive_philos(t_table *table, int number_of_philos)
{
	int		i;
	pid_t	pid;

	i = -1;
	while (++i < number_of_philos)
	{
		pid = fork();
		table->philo_pids[i] = pid;
		if (pid == 0)
			return (i + 1);
	}
	return (-1);
}

int	wait_meal_is_over(t_table *table)
{
	int		i;
	int		status;
	int		j;
	int		is_meal_over;

	status = -1;
	is_meal_over = 0;
	i = -1;
	while (++i < table->philo_pids_len)
	{
		waitpid(-1, &status, 0);
		if (!is_meal_over && WEXITSTATUS(status) != EXIT_SUCCESS)
		{
			is_meal_over = 1;
			j = -1;
			while (++j < table->philo_pids_len)
			{
				kill(table->philo_pids[j], 9);
			}
		}
	}
	return (0);
}

// table is main process and has philo_id = -1; philos are child process
int	table_serve(t_table *table, t_phargs *args)
{
	int	philo_id;

	philo_id = table_receive_philos(table, args->num_philos);
	if (philo_id >= 0)
		return (philo_sit_table(table, args, philo_id));
	else
		return (wait_meal_is_over(table));
}
