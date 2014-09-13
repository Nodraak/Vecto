
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
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_EDIT].pos, event->mousePos))
    {
        event->form = FORM_EDIT;
        event->state = STATE_IDLE;
    }
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_LINE].pos, event->mousePos))
    {
        event->form = FORM_LINE;
        event->state = STATE_IDLE;
    }
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_POLYGON].pos, event->mousePos))
    {
        event->form = FORM_POLYGON;
        event->state = STATE_IDLE;
    }
    /* colors */
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_R_MINUS].pos, event->mousePos))
        event->color.r -= COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_R_PLUS].pos, event->mousePos))
        event->color.r += COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_G_MINUS].pos, event->mousePos))
        event->color.g -= COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_G_PLUS].pos, event->mousePos))
        event->color.g += COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_B_MINUS].pos, event->mousePos))
        event->color.b -= COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_B_PLUS].pos, event->mousePos))
        event->color.b += COLOR_STEP;
    /* draw start / continue / end */
    else if (event->state == STATE_IDLE)
    {
        if (event->form == FORM_LINE || event->form == FORM_POLYGON)
        {
            /* draw start */
            event->current.nb_point = 1;
            event->current.point[0].x = mouse_x;
            event->current.point[0].y = mouse_y;

            event->state = STATE_DRAWING;
        }
        else if (event->form == FORM_EDIT)
        {

        }
        else
            printf("Error %d %s\n", __LINE__, __FILE__);
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
            forms[i].color.r = event->color.r;
            forms[i].color.g = event->color.g;
            forms[i].color.b = event->color.b;

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
        forms[i].color.r = event->color.r;
        forms[i].color.g = event->color.g;
        forms[i].color.b = event->color.b;

        event->state = STATE_IDLE;
    }
}


int ft_is_button_for_color(e_button button)
{
    return ((button == BUTTON_R_MINUS) || (button == BUTTON_R_PLUS)
        || (button == BUTTON_G_MINUS) || (button == BUTTON_G_PLUS)
        || (button == BUTTON_B_MINUS) || (button == BUTTON_B_PLUS)
    );
}


void ft_calc_update_button_color(s_button *buttons, s_color *color)
{
    int i;
    int newColor;

    if (color->r < 0) color->r = 0;
    if (color->r > 255) color->r = 255;
    if (color->g < 0) color->g = 0;
    if (color->g > 255) color->g = 255;
    if (color->b < 0) color->b = 0;
    if (color->b > 255) color->b = 255;

    newColor = makecol(color->r, color->g, color->b);

    for (i = 0; i < BUTTON_LAST; ++i)
    {
        if (ft_is_button_for_color(i))
            buttons[i].colorBackgroundDefault = newColor;
    }
}


void ft_calc_update_closer_point(s_event *event, s_form *forms)
{
    if (event->form == FORM_EDIT)
    {
        int i, j;
        int dist = (SCREEN_WIDTH * SCREEN_WIDTH) + (SCREEN_HEIGHT * SCREEN_HEIGHT); /* big number */

        for (i = 0; i < NB_FORM; ++i)
        {
            if (forms[i].type != FORM_NONE)
            {
                for (j = 0; j < forms[i].nb_point; ++j)
                {
                    int diff_x = mouse_x - forms[i].point[j].x;
                    int diff_y = mouse_y - forms[i].point[j].y;
                    int cur = diff_x*diff_x + diff_y*diff_y;

                    if (cur < dist)
                    {
                        dist = cur;
                        event->pointHovered = &forms[i].point[j];
                    }
                }
            }
        }
    }
}

