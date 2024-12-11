/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 21:57:48 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/11 14:13:01 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include "table.h"

int			ft_atoi_strict(int *out_ok, const char *str);
long long	get_time_millis(void);
void		millisleep(int millis);
int			logger(t_table *table, char *message, int philo_id);
void		logger_f(t_table *table, char *message, int philo_id, int fork);
int			time_to_die(t_philo *philo, long long last_meal);

#endif
