/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 23:46:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/26 00:21:17 by maurodri         ###   ########.fr       */
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

void a(sem_t *sem)
{	
	sem_wait(sem);
	usleep(1000000);
	printf("hello bonus child\n");
	sem_post(sem);
}

void b(sem_t *sem)
{
	sem_wait(sem);
	usleep(1000000);
	printf("hello bonus parent\n");
	sem_post(sem);
}

int	main(void)
{
	
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
	return (0);
}
