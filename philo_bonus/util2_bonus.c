/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:33:03 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/11 14:33:37 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include "util_bonus.h"

int	time_to_die(t_philo *philo, long long last_meal)
{
	return (philo->death_time - ((int)(get_time_millis() - last_meal)));
}
