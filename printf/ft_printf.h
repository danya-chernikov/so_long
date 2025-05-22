/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 04:34:51 by dchernik          #+#    #+#             */
/*   Updated: 2025/04/21 13:47:24 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stddef.h>

/* ft_printf.c */
int		ft_printf(char const *format, ...);
int		parse_format_str(char const *format, va_list *vl, int *pbytes);
int		process_percent(char const *format, va_list *vl, int *pbytes, int *i);
int		process_not_percent(char const *f, va_list *vl, void **pack);
int		process_conv(char const *format, va_list *vl, int cpos);

/* non_numeric.c */
int		char_conv(va_list *vl);
int		string_conv(va_list *vl);
int		ptr_conv(va_list *vl);

/* numeric.c */
int		nbr_conv(va_list *vl);
int		u_nbr_conv(va_list *vl);
int		hex_conv(va_list *vl, int dcase);
int		hex_not_zero(unsigned int hex_arg, int dcase, int *len);

/* utils.c */
int		next_sym_is_percent(char const *format, int *i);
char	*int_to_hex(unsigned long long num, int dcase);
char	*reverse_str(char *str);
int		is_conv(char ch);
int		count_digits(long long n);

/* packer.c */
void	**pack_args(int nargs, ...);

/* ft_utils.c */
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_putstr_fd(char *s, int fd);
size_t	ft_strlen(const char *str);
char	*ft_itoa_pos(unsigned int n);

/* ft_itoa.c */
char	*ft_itoa(int n);

#endif
