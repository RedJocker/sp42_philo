/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:17:41 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 08:23:16 by maurodri         ###   ########.fr       */
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
	sem_unlink("/cutlery_sem");
	sem_unlink("/log_lock");
	sem_unlink("/seat_lock");
	table->cutlery_sem = sem_open( \
		"/cutlery_sem", O_CREAT | O_EXCL, 0777, args->num_philos);
	printf("num_philos: %d\n", args->num_philos);
	table->seat_lock = sem_open(								\
		"/seat_lock", O_CREAT | O_EXCL, 0777, args->num_philos);
	table->log_lock = sem_open("/log_lock", O_CREAT | O_EXCL, 0777, 1);
	table->init_time = get_time_millis();
}

void	table_clean(t_table *table)
{
	sem_close(table->cutlery_sem);
	sem_close(table->log_lock);
	sem_close(table->seat_lock);
	sem_unlink("/cutlery_sem");
	sem_unlink("/log_lock");
	sem_unlink("/seat_lock");
	printf("table_clean\n");
}

static int	table_receive_philos(t_table *table, int number_of_philos)
{
	int		i;
	pid_t	pid;
	i = -1;
	while (++i < number_of_philos)
	{
		pid = fork();
		if (pid == 0)
			return (i);
	}
	return (-1);
}

int	wait_meal_is_over(t_table *table, int number_of_philos)
{
	int		i;
	int		status;
	int		j;
	int		is_meal_over;

	status = -1;
	is_meal_over = 0;
	i = -1;
	while (++i < number_of_philos)
	{
		waitpid(-1, &status, 0);
		printf("after waitpid %d %d\n", i, WEXITSTATUS(status));
		if (!is_meal_over && WEXITSTATUS(status) != EXIT_SUCCESS)
		{
			sem_getvalue(table->log_lock, &i);
			is_meal_over = 1;
			j = -1;
			printf("meal_over: log_lock %d\n", i);
			sem_wait(table->log_lock);
			sem_getvalue(table->log_lock, &i);
			printf("meal_over: log_locked %d\n", i);
			while (++j < number_of_philos)
				sem_wait(table->seat_lock);
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
		return (wait_meal_is_over(table, args->num_philos));
}
