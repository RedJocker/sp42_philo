/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phargs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:50:36 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/24 22:52:36 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHARGS_H
# define PHARGS_H

typedef struct s_philo_args
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	times_to_eat;
}	t_philo_args;

int	philo_args_init(t_philo_args *args, int argc, char *argv[]);

#endif
