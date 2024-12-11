/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cutlery.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:48:55 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/11 05:49:59 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cutlery.h"

void	cutlery_init(t_cutlery *cutlery)
{
	pthread_mutex_init(&cutlery->mutex, 0);
}

void	cutlery_clean(t_cutlery *cutlery)
{
	pthread_mutex_destroy(&cutlery->mutex);
}
