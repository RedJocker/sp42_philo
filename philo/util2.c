/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:25:48 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/11 14:12:48 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "util.h"

int	time_to_die(t_philo *philo, long long last_meal)
{
	return (philo->death_time - ((int)(get_time_millis() - last_meal)));
}
