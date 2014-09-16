
#include <stdio.h>

#include "constantes.h"
#include "ft_allegro.h"

void ft_allegro_scale_coord(double zoom, s_vector offset, int *x1, int *y1, int *x2, int *y2)
{
    *x1 = ft_coord_to_pxl(*x1, zoom, offset.x);
    *y1 = ft_coord_to_pxl(*y1, zoom, offset.y);
    *x2 = ft_coord_to_pxl(*x2, zoom, offset.x);
    *y2 = ft_coord_to_pxl(*y2, zoom, offset.y);
}


void close_button_handler(void)
{
    g_quit = 1;
}


void ft_allegro_init(void)
{
    if (allegro_init() != 0)
        exit(EXIT_FAILURE);

    set_color_depth(32);
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) != 0)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Unable to set any graphic mode - %s\n", allegro_error);
        exit(EXIT_FAILURE);
    }

    install_keyboard();
    install_mouse();

    set_close_button_callback(close_button_handler);
    show_mouse(screen);

    g_page = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (g_page == NULL)
    {
        fprintf(stderr, "Error create tmp screen.\n");
        exit(EXIT_FAILURE);
    }
    g_page_tmp = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (g_page_tmp == NULL)
    {
        fprintf(stderr, "Error create tmp screen.\n");
        exit(EXIT_FAILURE);
    }
}


void ft_allegro_line(BITMAP *bmp, s_event *event, int x1, int y1, int x2, int y2, int c, int flags)
{
    if (flags & FLAG_SCALE_COORD)
        ft_allegro_scale_coord(event->zoom, event->offset, &x1, &y1, &x2, &y2);

    line(bmp, x1, y1, x2, y2, c);
    if (flags & FLAG_FAT_LINE)
    {
        line(bmp, x1+1, y1, x2+1, y2, c);
        line(bmp, x1-1, y1, x2-1, y2, c);
        line(bmp, x1, y1+1, x2, y2+1, c);
        line(bmp, x1, y1-1, x2, y2-1, c);
    }
}


void ft_allegro_rect(BITMAP *bmp, s_event *event, int x1, int y1, int x2, int y2, int c)
{
    ft_allegro_scale_coord(event->zoom, event->offset, &x1, &y1, &x2, &y2);
    rect(bmp, x1, y1, x2, y2, c);
}

