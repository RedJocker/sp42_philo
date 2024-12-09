/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 17:37:36 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/09 15:08:46 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "table.h"
#include "phargs.h"
#include <stdio.h>

int	main(int argc, char *argv[])
{
	t_table			table;
	t_phargs		args;

	if (!phargs_init(&args, argc, argv))
	{
		printf("Invalid arguments\n");
		return (42);
	}
	table_init(&table, &args);
	pthread_create(&table.thread, 0, (void *(*)(void *))table_serve, &table);
	pthread_join(table.thread, 0);
	table_clean(&table);
	return (0);
}
