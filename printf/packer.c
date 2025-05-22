/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:34:03 by dchernik          #+#    #+#             */
/*   Updated: 2025/03/28 16:34:07 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/* It copies the pointers to the passed
 * variables into an array of (void *),
 * called 'packet', so we can pass this
 * array into another function, bypassing
 * the constraint that no more than four
 * arguments can be passed to a function.
 * pack - the packet of arguments */
void	**pack_args(int nargs, ...)
{
	void	**pack;
	va_list	vl;
	int		i;

	pack = (void **)ft_calloc(nargs, sizeof (void *));
	if (pack == NULL)
		return (NULL);
	va_start(vl, nargs);
	i = 0;
	while (i < nargs)
	{
		pack[i] = va_arg(vl, void *);
		i++;
	}
	va_end(vl);
	return (pack);
}
