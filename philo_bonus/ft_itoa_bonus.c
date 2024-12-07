/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 01:30:17 by maurodri          #+#    #+#             */
/*   Updated: 2024/12/07 01:33:57 by maurodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util_bonus.h"
#include <stdlib.h>

static size_t	str_num_len(long long num)
{
	size_t		len;

	len = 0;
	if (num < 0)
	{
		len++;
		num = -num;
	}
	while (num > 9)
	{
		num /= 10;
		len++;
	}
	return (++len);
}

static inline char	ch_digit(char byte_digit)
{
	return (byte_digit + '0');
}

char	*ft_itoa(int n)
{
	size_t		len;
	long long	num;
	char		*str_num;
	char		digit;

	num = (long long)n;
	len = str_num_len(num);
	str_num = malloc((len + 1) * sizeof(char));
	if (!(str_num))
		return (NULL);
	str_num[len--] = '\0';
	if (num < 0)
	{
		str_num[0] = '-';
		num = -num;
	}
	while (num > 9)
	{
		digit = (char)(num % 10);
		str_num[len--] = ch_digit(digit);
		num /= 10;
	}
	digit = (char)(num % 10);
	str_num[len] = ch_digit(digit);
	return (str_num);
}
