/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 23:46:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/24 18:33:48 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include "phargs.h"

void a(sem_t *sem)
{
	sem_wait(sem);
	usleep(1000000);
	printf("hello bonus child\n");
	sem_post(sem);
	sem_close(sem);
}

void b(sem_t *sem)
{
	sem_wait(sem);
	usleep(1000000);
	printf("hello bonus parent\n");
	sem_post(sem);
	sem_close(sem);
}

int	main(int argc, char *argv[])
{
	t_phargs		args;

	if (!phargs_init(&args, argc, argv))
		return (1);
	printf("phargs_init: %d %d %d %d %d\n", args.num_philos, args.time_to_die, args.time_to_eat, args.time_to_sleep, args.times_to_eat);
	sem_unlink("sem");
	sem_t *sem = sem_open("sem", O_CREAT, 0777, 1);
	pid_t i = fork();
	if (i == 0)
	{
		a(sem);
    }
	else if (i > 0)
	{
		usleep(111);
		b(sem);
		waitpid(i, 0, 0);
	}
	sem_unlink("sem");
	return (0);
}
