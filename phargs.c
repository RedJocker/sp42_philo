/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phargs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:53:26 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/25 00:32:25 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phargs.h"
#include "util.h"

int	philo_args_init(t_philo_args *args, int argc, char *argv[])
{
	int	is_ok;

	if (argc < 5 || argc > 6)
		return (0);
	args->num_philos = ft_atoi_strict(&is_ok, argv[1]);
	if (!is_ok)
		return (0);
	args->time_to_die = ft_atoi_strict(&is_ok, argv[2]);
	if (!is_ok)
		return (0);
	args->time_to_eat = ft_atoi_strict(&is_ok, argv[3]);
	if (!is_ok)
		return (0);
	args->time_to_sleep = ft_atoi_strict(&is_ok, argv[4]);
	if (!is_ok)
		return (0);
	if (argc == 6)
		args->times_to_eat = ft_atoi_strict(&is_ok, argv[5]);
	else
		args->times_to_eat = -1;
	if (!is_ok)
		return (0);
	return (1);
}
