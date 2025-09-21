#include "game_logic.h"

/* GOOD! */
void	clamp_camera(t_game_data *gdata)
{
	int	max_cx;
	int	max_cy;

	if (gdata->cam_x < 0)
		gdata->cam_x = 0;
	if (gdata->cam_y < 0)
		gdata->cam_y = 0;
	max_cx = gdata->map.width * TILE_SIZE - gdata->win_w;
	max_cy = gdata->map.height * TILE_SIZE - gdata->win_h;
	if (gdata->cam_x > max_cx)
		gdata->cam_x = (max_cx > 0 ? max_cx : 0);
	if (gdata->cam_y > max_cy)
		gdata->cam_y = (max_cy > 0 ? max_cy : 0);
}
