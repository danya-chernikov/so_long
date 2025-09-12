#include "../libx/mlx.h"

int	main(void)
{
	void	*mlx;
	void	*mlx_win;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 600, 600, "Test");
	mlx_loop(mlx);
	return (0);
}
