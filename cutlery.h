/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cutlery.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:26:06 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/25 00:16:09 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUTLERY_H
# define CUTLERY_H

# include <pthread.h>
# include "phargs.h"

typedef struct s_cutlery
{
	pthread_mutex_t	mutex;
}	t_cutlery;

void	cutlery_init(t_cutlery *cutlery, t_philo_args *args);
void	cutlery_clean(t_cutlery *cutlery);

#endif
