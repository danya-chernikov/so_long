#include "map.h"

#include <stdlib.h>

int	main(int argc, char *argv[])
{
	if (argc != 2)
	{
		ft_printf("%s\n", "You need to specify the path to the game's map");
		exit(1);
	}

	ft_printf("%s\n", argv[1]);
	check_map();

	return (0);
}
