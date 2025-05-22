/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 04:35:20 by dchernik          #+#    #+#             */
/*   Updated: 2025/03/28 17:44:45 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include <unistd.h>
#include <stdlib.h>

/* In a loop, we process all symbols in
 * the format string. If we find two
 * consecutive '%' symbols, we print
 * one of them. If the symbol following
 * '%' is not another '%', we continue
 * until we find a conversion symbol or
 * reach the end of the string. When we
 * find a conversion symbol corresponding
 * to that '%', we call process_conv(),
 * which will call the appropriate function
 * to handle the conversion. And finally
 * when we find a regular symbol we just
 * replicate it.
 *
 *     pbytes - number of bytes that were
 *              output by printf;
 *     cpos   - position of a found
 *			    conversion field;
 *     i      - index variable. */
int	ft_printf(char const *format, ...)
{
	int			pbytes;
	int			res;
	va_list		vl;

	pbytes = 0;
	va_start(vl, format);
	res = parse_format_str(format, &vl, &pbytes);
	if (res == -1)
		return (-1);
	va_end(vl);
	return (pbytes);
}

/* Returns 0 on success */
int	parse_format_str(char const *format, va_list *vl, int *pbytes)
{
	int	res;
	int	i;

	i = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			res = process_percent(format, vl, pbytes, &i);
			if (res == 0)
				break ;
			else if (res == 1)
				continue ;
			else if (res == -1)
				return (-1);
		}
		else
		{
			if (write(1, &format[i], (size_t)1) == -1)
				return (-1);
			(*pbytes)++;
		}
		i++;
	}
	return (0);
}

int	process_percent(char const *format, va_list *vl, int *pbytes, int *i)
{
	void	**pack;
	int		cpos;

	cpos = *i + 1;
	if (format[*i + 1] == '\0')
	{
		*pbytes = -1;
		return (0);
	}
	if (next_sym_is_percent(format, i))
	{
		*pbytes = *pbytes + 1;
		return (1);
	}
	else
	{
		pack = pack_args(3, (void *)pbytes, (void *)i, (void *)&cpos);
		if (pack == NULL)
			return (-1);
		if (process_not_percent(format, vl, pack) == -1)
			return (-1);
	}
	return (2);
}

/* Returns 0 on success */
int	process_not_percent(char const *f, va_list *vl, void **pack)
{
	int	*pbytes;
	int	*cpos;
	int	*i;
	int	res;

	res = 0;
	pbytes = (int *)pack[0];
	i = (int *)pack[1];
	cpos = (int *)pack[2];
	free(pack);
	while (!is_conv(f[*i]) && f[*i] != '\0')
		*i = *i + 1;
	if (is_conv(f[*i]))
	{
		res = process_conv(f, vl, *cpos);
		if (res == -1)
			return (-1);
		*pbytes = *pbytes + res;
	}
	return (0);
}

/* For each conversion calls the appropriate function
 * to handle it */
int	process_conv(char const *format, va_list *vl, int cpos)
{
	int	pbytes;

	if (format[cpos] == 'c')
		pbytes = char_conv(vl);
	if (format[cpos] == 's')
		pbytes = string_conv(vl);
	if (format[cpos] == 'p')
		pbytes = ptr_conv(vl);
	if (format[cpos] == 'd' || format[cpos] == 'i')
		pbytes = nbr_conv(vl);
	if (format[cpos] == 'u')
		pbytes = u_nbr_conv(vl);
	if (format[cpos] == 'x')
		pbytes = hex_conv(vl, 1);
	if (format[cpos] == 'X')
		pbytes = hex_conv(vl, 2);
	return (pbytes);
}
