/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_bonus.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:31:06 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 03:22:20 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stdlib.h>
# include "philo_bonus.h"

int			ft_atoi_strict(int *out_ok, const char *str);
long long	get_time_millis(void);
void		millisleep(int millis);
char		*ft_itoa(int n);
size_t		ft_strlen(const char *str);
void		logger(t_table *table, char *message, t_philo *philo);
void		*ft_memcpy(void *dst, const void *src, size_t n);

#endif
