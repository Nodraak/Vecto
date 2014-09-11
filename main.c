

#include <stdio.h>
#include <stdlib.h>

#include <allegro.h>

#include "constantes.h"
#include "file.h"
#include "draw.h"


BITMAP *page = NULL;

/* croix rouge de la fenetre */
int global_quit = 0;
void close_button_handler(void);
void close_button_handler(void) { global_quit = 1; }


void ft_init_allegro(void);

void ft_init_allegro(void)
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

    page = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (page == NULL)
    {
        fprintf(stderr, "Error create tmp screen.\n");
        exit(EXIT_FAILURE);
    }

}


void ft_event_update(t_event *event);

void ft_event_update(t_event *event)
{
    static t_event old = {{0}};

    event->mousePos.x = mouse_x;
    event->mousePos.y = mouse_y;

    if ((mouse_b & MOUSE_LEFT) && (old.mouseLeft == 0))
    {
        event->mouseLeft = 1;
        old.mouseLeft = 1;
    }
    if ((mouse_b & MOUSE_LEFT) == 0 && (old.mouseLeft == 1))
    {
        event->mouseLeft = 0;
        old.mouseLeft = 0;
    }

    if ((mouse_b & MOUSE_RIGHT) && (old.mouseRight == 0))
    {
        event->mouseRight = 1;
        old.mouseRight = 1;
    }
    if ((mouse_b & MOUSE_RIGHT) == 0 && (old.mouseRight == 1))
    {
        event->mouseRight = 0;
        old.mouseRight = 0;
    }
}


void ft_calc_on_mouseLeft(t_event *event, t_form *form, t_rect *button_load, t_rect *button_save);

void ft_calc_on_mouseLeft(t_event *event, t_form *form, t_rect *button_load, t_rect *button_save)
{
    if (ft_is_mouse_in_rect(button_load, event->mousePos))
    {
        ft_file_load(form, "data.vecto");
        event->mouseLeft = 0;
    }
    else if (ft_is_mouse_in_rect(button_save, event->mousePos))
    {
        ft_file_save(form, "data.vecto");
        event->mouseLeft = 0;
    }
    else if (event->state == E_STATE_WAIT)
    {
        /* draw start */
        event->current_line.x = mouse_x;
        event->current_line.y = mouse_y;

        event->state = E_STATE_DRAW_WAIT;
        event->mouseLeft = 0;
    }
    else if (event->state == E_STATE_DRAW_WAIT)
    {
        /* draw end */
        int i = 0;
        while (form[i].used)
            i++;
        form[i].p1.x = event->current_line.x;
        form[i].p1.y = event->current_line.y;
        form[i].p2.x = mouse_x;
        form[i].p2.y = mouse_y;
        form[i].used = 1;

        event->state = E_STATE_WAIT;
        event->mouseLeft = 0;
    }
}


int main(void)
{
    t_form form[NB_FORM];
    memset(form, 0, sizeof(t_form)*NB_FORM);

    t_event event;
    memset(&event, 0, sizeof(t_event));
    event.state = E_STATE_WAIT;

    t_rect button_load = {0, 0, 100, 50};
    t_rect button_save = {100, 0, 100, 50};


    ft_init_allegro();


    while (!global_quit)
    {
        /* event */
        ft_event_update(&event);

        /* calc */
        if (event.mouseLeft)
            ft_calc_on_mouseLeft(&event, form, &button_load, &button_save);

        if (event.mouseRight)
        {
            event.mouseRight = 0;
            event.state = E_STATE_WAIT;
        }

        /* draw */
        ft_draw_all(&event, form, &button_load, &button_save);
        acquire_screen();
        blit(page, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        release_screen();

        rest(1000/FPS);
    }

    destroy_bitmap(page);
    allegro_exit();

    return 0;
}
END_OF_MAIN()
