/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 04:35:00 by dchernik          #+#    #+#             */
/*   Updated: 2025/04/21 13:29:04 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include <unistd.h>

/* Determines if the symbol after the i position is
 * a percent symbol. If so, it prints '%' and skips
 * the i + 1 position. Returns 1 if the next symbol
 * after '%' is percent and 0 if not */
int	next_sym_is_percent(char const *format, int *i)
{
	char	percent;

	percent = '%';
	if (format[*i + 1] == '%')
	{
		if (write(1, &percent, (size_t)1) == -1)
			return (-1);
		*i += 2;
		return (1);
	}
	return (0);
}

char	*int_to_hex(unsigned long long num, int dcase)
{
	char	*lower;
	char	*upper;
	char	*str;
	int		i;

	lower = "0123456789abcdef\0";
	upper = "0123456789ABCDEF\0";
	str = ft_calloc(32, sizeof (char));
	if (str == NULL)
		return (NULL);
	if (num == 0)
		str[0] = '0';
	i = 0;
	while (num != 0)
	{
		if (dcase == 1)
			str[i] = lower[num % 16];
		else if (dcase == 2)
			str[i] = upper[num % 16];
		num /= 16;
		i++;
	}
	return (reverse_str(str));
}

char	*reverse_str(char *str)
{
	size_t	i;
	size_t	slen;
	char	tmp;

	i = 0;
	slen = ft_strlen(str);
	while (i < slen >> 1)
	{
		tmp = str[i];
		str[i] = str[slen - i - 1];
		str[slen - i - 1] = tmp;
		i++;
	}
	return (str);
}

int	count_digits(long long n)
{
	int	num;

	num = 0;
	if (n < 0)
		n = -n;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 10;
		num++;
	}
	return (num);
}

/* Determines if a symbol
 * belongs to a conversion
 * specifier symbols group */
int	is_conv(char ch)
{
	char const	*convs;
	int			i;

	convs = "cspdiuxX\0";
	i = 0;
	while (convs[i] != '\0')
	{
		if (ch == convs[i])
			return (1);
		i++;
	}
	return (0);
}
