/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 21:57:48 by maurodri          #+#    #+#             */
/*   Updated: 2024/11/24 18:38:25 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

//# include "table.h" 

int			ft_atoi_strict(int *out_ok, const char *str);
long long	get_time_millis(void);
void		millisleep(int millis);
//void		logger(t_table *table, char *message, int philo_id);
//void		logger_f(t_table *table, char *message, int philo_id, int fork);

#endif
