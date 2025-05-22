CC=cc
NAME=so_long
LIBFT=Libft/libft.a
PRINTF=printf/libftprintf.a
LIBX=libx/libmlx.a
LIBX_FLAGS=-lXext -lX11 -lm -lz
CFLAGS=-Wall -Werror -Wextra -O0 -g3

# I use -no-pie flag to avoid these errors:
# /usr/bin/ld: printf/libftprintf.a(non_numeric.o): warning: relocation in read-only section `.text'
# /usr/bin/ld: warning: creating DT_TEXTREL in a PIE

OBJS=main.o map_checker.o


$(NAME) : $(OBJS)
	cd Libft/ && $(MAKE) all && $(MAKE) bonus # First let's build the Libft
	cd printf/ && $(MAKE) all # Next let's build printf
	cd libx/ && $(MAKE) all # Finally let's build minilibx for Linux
	$(CC) $(CFLAGS) -no-pie $(OBJS) $(LIBFT) $(PRINTF) $(LIBX) $(LIBX_FLAGS) -o $(NAME) # Building so_long itself

all : $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

main.o : main.c map.h
	$(CC) $(CFLAGS) -I/usr/include -c main.c

map_checker.o : map_checker.c map.h
	$(CC) $(CFLAGS) -I/usr/include -c map_checker.c

.PHONY: all clean fclean re
