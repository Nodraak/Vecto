

#include <stdio.h>
#include <stdlib.h>

#include <allegro.h>

#include "constantes.h"
#include "file.h"


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
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0) != 0)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Unable to set any graphic mode - %s\n", allegro_error);
        exit(EXIT_FAILURE);
    }

    install_keyboard();
    install_mouse();

    set_close_button_callback(close_button_handler);
    show_mouse(screen);
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


void ft_draw_button(t_rect button, int colorDefault, int colorHover, t_vector mousePos);


void ft_draw_button(t_rect button, int colorDefault, int colorHover, t_vector mousePos)
{
    if (mousePos.x > button.x && mousePos.x < button.x+button.width && mousePos.y > button.y && mousePos.y < button.y+button.height)
        rectfill(screen, button.x, button.y, button.x+button.width, button.y+button.height, colorHover);
    else
        rectfill(screen, button.x, button.y, button.x+button.width, button.y+button.height, colorDefault);
}


int ft_is_mouse_in_rect(t_rect rect, t_vector mousePos);


int ft_is_mouse_in_rect(t_rect rect, t_vector mousePos)
{
    return (mousePos.x > rect.x && mousePos.x < rect.x+rect.width && mousePos.y > rect.y && mousePos.y < rect.y+rect.height);
}



int main(void)
{
    t_vector tmp;

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
        {
            if (ft_is_mouse_in_rect(button_load, event.mousePos))
            {
                ft_file_load(form, "data.vecto");
                event.mouseLeft = 0;
            }
            else if (ft_is_mouse_in_rect(button_save, event.mousePos))
            {
                ft_file_save(form, "data.vecto");
                event.mouseLeft = 0;
            }
            else if (event.state == E_STATE_WAIT)
            {
                /* draw start */
                tmp.x = mouse_x;
                tmp.y = mouse_y;

                event.state = E_STATE_DRAW_WAIT;
                event.mouseLeft = 0;
            }
            else if (event.state == E_STATE_DRAW_WAIT)
            {
                /* draw end */
                int i = 0;
                while (form[i].used)
                    i++;
                form[i].p1.x = tmp.x;
                form[i].p1.y = tmp.y;
                form[i].p2.x = mouse_x;
                form[i].p2.y = mouse_y;
                form[i].used = 1;

                event.state = E_STATE_WAIT;
                event.mouseLeft = 0;
            }
        }

        if (event.mouseRight)
        {
            event.mouseRight = 0;
            event.state = E_STATE_WAIT;
        }


        /* draw */
        int i;

        clear_to_color(screen, makecol(255, 255, 255));
        for (i = 0; i < NB_FORM; ++i)
        {
            if (form[i].used)
                line(screen, form[i].p1.x, form[i].p1.y, form[i].p2.x, form[i].p2.y, makecol(255, 0, 0));
        }

        if (event.state == E_STATE_DRAW_WAIT)
            line(screen, tmp.x, tmp.y, mouse_x, mouse_y, makecol(255, 0, 0));

        ft_draw_button(button_load, makecol(50, 50, 255), makecol(80, 80, 255), event.mousePos);
        ft_draw_button(button_save, makecol(50, 50, 255), makecol(80, 80, 255), event.mousePos);

        textout_centre_ex(screen, font, "Load", 50, 20, makecol(0, 0, 0), -1);
        textout_centre_ex(screen, font, "Save", 150, 20, makecol(0, 0, 0), -1);


        rest(1000/50);
    }

    allegro_exit();

    return 0;
}
END_OF_MAIN()
