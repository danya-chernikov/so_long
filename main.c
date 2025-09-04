#include "map.h"

#include "libx/mlx.h"

#include <stdlib.h>

int	main(int argc, char *argv[])
{
	void	*mlx;
	void	*mlx_win;
    t_map   map;

	if (argc != 2)
	{
		ft_printf("%s", "You need to specify the path to the game's map\n");
		exit(1);
	}

	ft_printf("%s\n", argv[1]);
    map_init(&map, argv[1]);
	map_check(&map);

	mlx = mlx_init();
	if (mlx == NULL)
	{
		ft_printf("%s", "mlx_init(): Failed to initialize mlx\n");
		exit(2);
	}

	mlx_win = mlx_new_window(mlx, 1280, 720, "Just a window");
	if (mlx_win == NULL)
	{
		ft_printf("%s", "mlx_new_window(): Cannot create a window\n");
		exit(2);
	}
	mlx_loop(mlx);

	return (0);
}
