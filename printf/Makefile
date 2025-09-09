CC=cc
NAME=libftprintf.a
CFLAGS=-Wall -Werror -Wextra

OBJS=ft_printf.o numeric.o non_numeric.o utils.o ft_utils.o

$(NAME) : $(OBJS)
	cd ../libft/ && $(MAKE) all && $(MAKE) bonus
	ar rcs $(NAME) $(OBJS)

all : $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

ft_printf.o : ft_printf.c ft_printf.h ../libft/libft.h
	$(CC) $(CFLAGS) -c ft_printf.c

numeric.o : numeric.c ft_printf.h
	$(CC) $(CFLAGS) -c numeric.c

non_numeric.o : non_numeric.c ft_printf.h
	$(CC) $(CFLAGS) -c non_numeric.c

utils.o : utils.c ft_printf.h
	$(CC) $(CFLAGS) -c utils.c

ft_utils.o : ft_utils.c ft_printf.h
	$(CC) $(CFLAGS) -c ft_utils.c

.PHONY: all clean fclean re
