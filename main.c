/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 17:37:36 by maurodri          #+#    #+#             */
/*   Updated: 2024/07/25 00:16:36 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "table.h"

int	main(int argc, char *argv[])
{
	t_table			table;
	t_philo_args	args;

	if (!philo_args_init(&args, argc, argv))
		return (1);
	table_init(&table, &args);
	pthread_create(&table.thread, 0, (void *(*)(void *))table_serve, &table);
	pthread_join(table.thread, 0);
	table_clean(&table);
	return (0);
}
