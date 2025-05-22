CC=cc
NAME=libftprintf.a
CFLAGS=-Wall -Werror -Wextra

SRCS=ft_printf.c numeric.c non_numeric.c utils.c packer.c ft_utils.c ft_itoa.c

OBJS=ft_printf.o numeric.o non_numeric.o utils.o packer.o ft_utils.o ft_itoa.o

$(NAME) : $(OBJS)
	ar rcs $(NAME) $(OBJS)

all : $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

%.o : %.c ft_printf.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re
