
#include <stdio.h>

#include "constantes.h"
#include "calc.h"
#include "file.h"
#include "draw.h"


void ft_calc_on_mouseLeft(s_event *event, s_form *forms, s_button *buttons)
{
    /* menu */
    if (ft_is_mouse_in_rect(&buttons[BUTTON_LOAD].pos, event->mousePos))
        ft_file_load(forms, "data.vecto");
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_SAVE].pos, event->mousePos))
        ft_file_save(forms, "data.vecto");
    /* form */
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_LINE].pos, event->mousePos))
        event->form = FORM_LINE;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_POLYGON].pos, event->mousePos))
        event->form = FORM_POLYGON;
    /* draw start / continue / end */
    else if (event->state == STATE_IDLE)
    {
        /* draw start */
        event->current.nb_point = 1;
        event->current.point[0].x = mouse_x;
        event->current.point[0].y = mouse_y;

        event->state = STATE_DRAWING;
    }
    else if (event->state == STATE_DRAWING)
    {
        /* draw continue / end */
        if (event->form == FORM_LINE)
        {
            int i = 0;
            while (forms[i].type != FORM_NONE)
                i++;

            forms[i].point[0].x = event->current.point[0].x;
            forms[i].point[0].y = event->current.point[0].y;
            forms[i].point[1].x = mouse_x;
            forms[i].point[1].y = mouse_y;
            forms[i].nb_point = 2;
            forms[i].type = FORM_LINE;

            event->state = STATE_IDLE;
        }
        else if (event->form == FORM_POLYGON)
        {
            event->current.point[event->current.nb_point].x = mouse_x;
            event->current.point[event->current.nb_point].y = mouse_y;
            event->current.nb_point ++;
        }
        else
            printf("Error %d %s\n", __LINE__, __FILE__);
    }
}


void ft_calc_on_mouseRight(s_event *event, s_form *forms)
{
    if (event->form == FORM_LINE)
    {

    }
    else if (event->form == FORM_POLYGON)
    {
        int i = 0, j;
        while (forms[i].type != FORM_NONE)
            i++;

        for (j = 0; j < event->current.nb_point; ++j)
        {
            forms[i].point[j].x = event->current.point[j].x;
            forms[i].point[j].y = event->current.point[j].y;
        }

        forms[i].point[event->current.nb_point].x = mouse_x;
        forms[i].point[event->current.nb_point].y = mouse_y;
        forms[i].nb_point = event->current.nb_point+1;

        forms[i].type = FORM_POLYGON;

        event->state = STATE_IDLE;
    }
}
