CC=cc
NAME=so_long

LIBFT_PATH=libft
LIBFT=$(LIBFT_PATH)/libft.a
LIBFT_HEADER=$(LIBFT_PATH)/libft.h

PRINTF_PATH=printf
PRINTF=$(PRINTF_PATH)/libftprintf.a
PRINTF_HEADER=$(PRINTF_PATH)/ft_printf.h

LIBX=libx/libmlx.a
LIBX_FLAGS=-lXext -lX11 -lm -lz

CFLAGS=-Wall -Werror -Wextra -O0 -g3

# I use -no-pie flag to avoid these errors:
# /usr/bin/ld: printf/libftprintf.a(non_numeric.o): warning: relocation in read-only section `.text'
# /usr/bin/ld: warning: creating DT_TEXTREL in a PIE

OBJS=main.o map_checker.o map_checker2.o map_checker3.o map_checker4.o map_checker5.o game_logic.o game_logic2.o input.o render.o


$(NAME) : $(OBJS)
	cd libft/ && $(MAKE) all && $(MAKE) bonus # First let's build the Libft
	cd printf/ && $(MAKE) all # Next let's build printf
	cd libx/ && $(MAKE) all # Finally let's build minilibx for Linux
	$(CC) $(CFLAGS) -no-pie $(OBJS) $(PRINTF) $(LIBFT) $(LIBX) $(LIBX_FLAGS) -o $(NAME) # Building so_long itself

all : $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

main.o : main.c game_logic.h
	$(CC) $(CFLAGS) -I/usr/include -c main.c

map_checker.o : map_checker.c game_logic.h $(LIBFT_HEADER)
	$(CC) $(CFLAGS) -c map_checker.c

map_checker2.o : map_checker2.c game_logic.h $(LIBFT_HEADER)
	$(CC) $(CFLAGS) -c map_checker2.c

map_checker3.o : map_checker3.c game_logic.h $(LIBFT_HEADER)
	$(CC) $(CFLAGS) -c map_checker3.c

map_checker4.o : map_checker4.c game_logic.h $(LIBFT_HEADER)
	$(CC) $(CFLAGS) -c map_checker4.c

map_checker5.o : map_checker5.c game_logic.h $(LIBFT_HEADER)
	$(CC) $(CFLAGS) -c map_checker5.c

game_logic.o : game_logic.c libx/mlx.h map.h key_codes.h
	$(CC) $(CFLAGS) -c game_logic.c

game_logic2.o : game_logic2.c libx/mlx.h map.h key_codes.h
	$(CC) $(CFLAGS) -c game_logic2.c

input.o : input.c game_logic.h
	$(CC) $(CFLAGS) -c input.c

render.o : render.c game_logic.h
	$(CC) $(CFLAGS) -c render.c

.PHONY: all clean fclean re
