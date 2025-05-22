/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_numeric.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 04:35:03 by dchernik          #+#    #+#             */
/*   Updated: 2025/03/28 17:25:14 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include <unistd.h>
#include <stdlib.h>

/* Processes %c conversion
   args_cnt here is static variable declared in ft_printf.c ! */
int	char_conv(va_list *vl)
{
	int		char_arg;
	char	null;
	int		len;

	len = 1;
	null = '\0';
	char_arg = va_arg(*vl, int);
	if (char_arg == 0)
	{
		if (write(1, &null, len) == -1)
			return (-1);
	}
	else
	{
		if (write(1, &char_arg, len) == -1)
			return (-1);
	}
	return (len);
}

/* Processes %s conversion */
int	string_conv(va_list *vl)
{
	char	*str_arg;
	int		len;

	str_arg = va_arg(*vl, char *);
	if (str_arg == NULL)
	{
		len = 6;
		if (ft_putstr_fd("(null)", 1) == -1)
			return (-1);
	}
	else
	{
		len = ft_strlen(str_arg);
		if (ft_putstr_fd(str_arg, 1) == -1)
			return (-1);
	}
	return (len);
}

/* Processes %p conversion */
int	ptr_conv(va_list *vl)
{
	char				*hexnum;
	unsigned long long	uint_arg;
	int					len;

	uint_arg = va_arg(*vl, unsigned long long);
	if (uint_arg == 0)
	{
		len = 5;
		if (ft_putstr_fd("(nil)", 1) == -1)
			return (-1);
	}
	else
	{
		hexnum = int_to_hex(uint_arg, 1);
		if (hexnum == NULL)
			return (-1);
		len = ft_strlen(hexnum) + 2;
		if (write(1, "0x", (size_t)2) == -1)
			return (-1);
		if (ft_putstr_fd(hexnum, 1) == -1)
			return (-1);
		free(hexnum);
	}
	return (len);
}
