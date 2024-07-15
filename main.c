/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 17:37:36 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/15 17:45:51 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_philo
{
	int		id;
	int		sleep_time;
	int		eat_time;
	int		death_time;
	long	last_meal_time;
	int		times_eaten;
}	t_philo;

typedef struct s_cutlery
{
}	t_cutlery;

typedef struct s_table
{
	int			num_philos;
	t_philo		*philo_arr;
	t_cutlery	*cutlery_arr;
}	t_table;

void	millisleep(int millis)
{
	usleep(1000 * millis);
}

void	callback(void *args)
{
	int				*shared_counter;
	pthread_mutex_t	*m;

	shared_counter = ((int **)args)[0];
	m = ((pthread_mutex_t **) args)[1];
	printf("callback before lock %d \n", *shared_counter);
	pthread_mutex_lock(m);
	printf("callback after lock\n");
	*shared_counter = *shared_counter + 1;
	usleep(10);
	printf("callback %d\n", *shared_counter);
	pthread_mutex_unlock(m);
	millisleep(4000);
}

int	main(void)
{
	struct timeval	tv;
	pthread_t		t;
	pthread_mutex_t	m;
	int				i;
	int				shared_arg;
	void				*arr[2];

	shared_arg = 0;
	printf("hi\n");
	arr[0] = &shared_arg;
	arr[1] = &m;
	pthread_mutex_init(&m, 0);
	pthread_create(&t, 0, (void *(*)(void *))callback, arr);
	pthread_create(&t, 0, (void *(*)(void *))callback, arr);
	pthread_create(&t, 0, (void *(*)(void *))callback, arr);
	pthread_create(&t, 0, (void *(*)(void *))callback, arr);
	pthread_create(&t, 0, (void *(*)(void *))callback, arr);
	i = -1;
	while (++i < 20)
	{
		gettimeofday(&tv, 0);
		printf("%ld %ld\n", tv.tv_sec, tv.tv_usec);
		millisleep(100);
	}
	pthread_join(t, 0);
	millisleep(2000);
	printf("there\n");
	pthread_mutex_destroy(&m);
	//pthread_detach(t);
	return (0);
}
