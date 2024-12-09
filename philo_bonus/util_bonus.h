/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_bonus.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:31:06 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/09 03:50:11 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_BONUS_H
# define UTIL_BONUS_H

# include <stdlib.h>
# include "philo_bonus.h"

int			ft_atoi_strict(int *out_ok, const char *str);
long long	get_time_millis(void);
void		millisleep(int millis);
char		*ft_itoa(int n);
size_t		ft_strlen(const char *str);
void		*ft_memcpy(void *dst, const void *src, size_t n);
sem_t		*ft_sem_open(char *semaphore_name, int initial_value);

#endif
