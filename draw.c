
#include <allegro.h>

#include "constantes.h"
#include "draw.h"


void ft_draw_button(s_button *button, s_vector mousePos, int selected)
{
    s_rect *pos = &button->pos;

    if (ft_is_mouse_in_rect(pos, mousePos))
        rectfill(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, button->colorBackgroundHover);
    else
        rectfill(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, button->colorBackgroundDefault);

    textout_centre_ex(g_page, font, button->text, pos->x + pos->width/2, pos->y + pos->height/2-5, makecol(0, 0, 0), -1);

    if (selected)
        rect(g_page, pos->x+1, pos->y+1, pos->x+pos->width-1, pos->y+pos->height-1, makecol(255, 100, 100));
}


int ft_is_mouse_in_rect(s_rect *rect, s_vector mousePos)
{
    return (mousePos.x > rect->x && mousePos.x < rect->x+rect->width && mousePos.y > rect->y && mousePos.y < rect->y+rect->height);
}


void ft_draw_all(s_event *event, s_form *form, s_button *buttons)
{
    int i, j;

    /* clear */
    clear_to_color(g_page, makecol(255, 255, 255));

    /* saved forms */
    for (i = 0; i < NB_FORM; ++i)
    {
        if (form[i].type != FORM_NONE)
        {
            s_vector *p1 = NULL, *p2 = NULL;

            for (j = 0; j < form[i].nb_point-1; ++j)
            {
                p1 = &form[i].point[j];
                p2 = &form[i].point[j+1];

                line(g_page, p1->x, p1->y, p2->x, p2->y, makecol(255, 0, 0));
            }

            /* closing line */
            p1 = &form[i].point[0];
            p2 = &form[i].point[form[i].nb_point-1];

            line(g_page, p1->x, p1->y, p2->x, p2->y, makecol(255, 0, 0));
        }
    }

    /* current form */
    if (event->state == STATE_DRAWING)
    {
        s_vector *p1 = NULL, *p2 = NULL;

        for (j = 0; j < event->current.nb_point-1; ++j)
        {
            p1 = &event->current.point[j];
            p2 = &event->current.point[j+1];

            line(g_page, p1->x, p1->y, p2->x, p2->y, makecol(255, 0, 0));
        }

        p1 = &event->current.point[event->current.nb_point-1];

        line(g_page, p1->x, p1->y, mouse_x, mouse_y, makecol(255, 0, 0));
    }

    /* buttons : menu / forms */
    for (i = 0; i < BUTTON_LAST; ++i)
        ft_draw_button(&buttons[i], event->mousePos, event->form == buttons[i].form);
}

