/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybeaucou <ybeaucou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:36:27 by npigeon           #+#    #+#             */
/*   Updated: 2024/11/14 10:50:29 by ybeaucou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include <stdio.h>

static int	gc_itoa_info(long int n)
{
	int	size;

	size = 0;
	if (n < 0)
	{
		n *= -1;
		size++;
	}
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		size++;
	}
	return (size);
}

char	*gc_itoa(t_memory_table *mem, long int n)
{
	char		*str;
	int			size;
	long int	num;

	num = n;
	size = gc_itoa_info(n);
	str = (char *)gc_malloc(mem, sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	if (num < 0)
	{
		str[0] = '-';
		num *= -1;
	}
	if (num == 0)
		str[0] = '0';
	while (num != 0 && size > 0)
	{
		str[size - 1] = num % 10 + '0';
		num /= 10;
		size--;
	}
	str[gc_itoa_info(n)] = '\0';
	return (str);
}
