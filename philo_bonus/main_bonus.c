/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 23:46:59 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 11:17:14 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include "phargs_bonus.h"
#include "table_bonus.h"

int	main(int argc, char *argv[])
{
	t_phargs		args;
	t_table			table;
	int				exit_status;

	if (!phargs_init(&args, argc, argv))
		return (42);
	table_init(&table, &args);
	exit_status = table_serve(&table, &args);
	table_clean(&table);
	return (exit_status);
}
