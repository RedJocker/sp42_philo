/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phargs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:50:36 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/25 00:44:35 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHARGS_H
# define PHARGS_H

typedef struct s_phargs
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	times_to_eat;
}	t_phargs;

int	phargs_init(t_phargs *args, int argc, char *argv[]);

#endif
