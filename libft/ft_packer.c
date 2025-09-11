/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_packer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:37:17 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/10 16:14:13 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>

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
	if (!pack)
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
