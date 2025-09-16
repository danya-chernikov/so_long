/* move_circle_continuous.c
   MiniLibX: continuous movement for a circle using WASD (hold keys).
*/

#include "mlx.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#define WIN_W 800
#define WIN_H 600
#define RADIUS 40
#define SPEED_PPS 300.0   /* pixels per second */

typedef struct s_img {
    void    *img;
    char    *data;
    int     bpp;
    int     size_line;
    int     endian;
}   t_img;

typedef struct s_keys {
    int up;
    int down;
    int left;
    int right;
}   t_keys;

typedef struct s_vars {
    void    *mlx;
    void    *win;
    t_img   img;
    int     cx;
    int     cy;
    int     radius;
    t_keys  keys;
    double  last_time; /* seconds since epoch (double) */
}   t_vars;

/* utility: current time in seconds as double */
static double now_seconds(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

/* set pixel in image buffer (x,y within bounds) */
static void put_pixel(t_img *img, int x, int y, int color)
{
    if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
        return;
    unsigned int pixel = (unsigned int)color;
    char *dst = img->data + (y * img->size_line + x * (img->bpp / 8));
    *(unsigned int *)dst = pixel;
}

/* clear image (fill with background color) */
static void clear_image(t_img *img, int color)
{
    int x, y;
    for (y = 0; y < WIN_H; ++y)
        for (x = 0; x < WIN_W; ++x)
            put_pixel(img, x, y, color);
}

/* draw filled circle using vertical scanlines */
static void draw_filled_circle(t_img *img, int cx, int cy, int r, int color)
{
    int dx;
    for (dx = -r; dx <= r; ++dx)
    {
        int h = (int)floor(sqrt((double)(r * r - dx * dx)));
        int x = cx + dx;
        int y0 = cy - h;
        int y1 = cy + h;
        for (int y = y0; y <= y1; ++y)
            put_pixel(img, x, y, color);
    }
}

/* redraw scene */
static void redraw(t_vars *vars)
{
    int bg = 0x00102030;
    int circle_color = 0x00A0FFFF;

    clear_image(&vars->img, bg);
    draw_filled_circle(&vars->img, vars->cx, vars->cy, vars->radius, circle_color);
    mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
}

/* cleanup and exit */
static int cleanup_and_exit(t_vars *vars)
{
    if (!vars)
        exit(0);
    if (vars->img.img)
        mlx_destroy_image(vars->mlx, vars->img.img);
    if (vars->win)
        mlx_destroy_window(vars->mlx, vars->win);
    exit(0);
    return (0);
}

/* Move circle according to keys and elapsed time */
static void physics_update(t_vars *vars, double dt)
{
    double dx = 0.0;
    double dy = 0.0;
    double speed = SPEED_PPS;

    if (vars->keys.up)    dy -= 1.0;
    if (vars->keys.down)  dy += 1.0;
    if (vars->keys.left)  dx -= 1.0;
    if (vars->keys.right) dx += 1.0;

    if (dx == 0.0 && dy == 0.0)
        return;

    /* normalize so diagonal isn't faster */
    double len = sqrt(dx * dx + dy * dy);
    if (len != 0.0) {
        dx = dx / len;
        dy = dy / len;
    }

    /* apply movement */
    vars->cx += (int)round(dx * speed * dt);
    vars->cy += (int)round(dy * speed * dt);

    /* clamp */
    if (vars->cx < vars->radius) vars->cx = vars->radius;
    if (vars->cx > WIN_W - vars->radius) vars->cx = WIN_W - vars->radius;
    if (vars->cy < vars->radius) vars->cy = vars->radius;
    if (vars->cy > WIN_H - vars->radius) vars->cy = WIN_H - vars->radius;
}

/* loop hook: called each iteration of mlx_loop */
static int loop_hook(void *param)
{
    t_vars *vars = (t_vars *)param;
    double t = now_seconds();
    double dt = t - vars->last_time;
    if (dt <= 0.0)
        dt = 0.0;
    /* cap dt to avoid huge jumps after debugger pause */
    if (dt > 0.1)
        dt = 0.1;

    physics_update(vars, dt);
    redraw(vars);

    vars->last_time = t;
    /* small sleep to avoid busy-looping too hard (optional) */
    usleep(1000); /* 1ms */
    return (0);
}

/* key press handler (set flags). Checks common X11 and mac keycodes */
static int key_press(int keycode, void *param)
{
    t_vars *vars = (t_vars *)param;
    if (keycode == 65307 || keycode == 53) /* ESC */
        cleanup_and_exit(vars);

    /* W */
    if (keycode == 119 || keycode == 13)
        vars->keys.up = 1;
    /* S */
    if (keycode == 115 || keycode == 1)
        vars->keys.down = 1;
    /* A */
    if (keycode == 97 || keycode == 0)
        vars->keys.left = 1;
    /* D */
    if (keycode == 100 || keycode == 2)
        vars->keys.right = 1;

    return (0);
}

/* key release handler (clear flags) */
static int key_release(int keycode, void *param)
{
    t_vars *vars = (t_vars *)param;

    /* W */
    if (keycode == 119 || keycode == 13)
        vars->keys.up = 0;
    /* S */
    if (keycode == 115 || keycode == 1)
        vars->keys.down = 0;
    /* A */
    if (keycode == 97 || keycode == 0)
        vars->keys.left = 0;
    /* D */
    if (keycode == 100 || keycode == 2)
        vars->keys.right = 0;

    return (0);
}

/* handle window close (red cross) */
static int close_hook(void *param)
{
    t_vars *vars = (t_vars *)param;
    (void)vars;
    cleanup_and_exit(vars);
    return (0);
}

int main(void)
{
    t_vars vars;

    memset(&vars, 0, sizeof(vars));
    vars.radius = RADIUS;
    vars.cx = WIN_W / 2;
    vars.cy = WIN_H / 2;
    vars.last_time = now_seconds();

    vars.mlx = mlx_init();
    if (!vars.mlx)
        return (1);

    vars.win = mlx_new_window(vars.mlx, WIN_W, WIN_H, "Move the Circle (hold WASD)");
    vars.img.img = mlx_new_image(vars.mlx, WIN_W, WIN_H);
    vars.img.data = mlx_get_data_addr(vars.img.img, &vars.img.bpp, &vars.img.size_line, &vars.img.endian);

    redraw(&vars);

    /* key press / key release hooks (event codes 2 and 3 on X11-based mlx) */
    mlx_hook(vars.win, 2, (1L<<0), key_press, &vars);    /* KeyPress */
    mlx_hook(vars.win, 3, (1L<<1), key_release, &vars);  /* KeyRelease */

    /* handle window close (red cross) */
    mlx_hook(vars.win, 17, 0L, close_hook, &vars);

    /* loop hook called as often as possible; we use time delta for smooth movement */
    mlx_loop_hook(vars.mlx, loop_hook, &vars);

    mlx_loop(vars.mlx);
    return (0);
}

