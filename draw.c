
#include <allegro.h>

#include "constantes.h"
#include "draw.h"


void ft_draw_button(t_label *button, t_vector mousePos)
{
    if (ft_is_mouse_in_rect(&button->pos, mousePos))
        rectfill(page, button->pos.x, button->pos.y, button->pos.x+button->pos.width, button->pos.y+button->pos.height, button->colorBackgroundHover);
    else
        rectfill(page, button->pos.x, button->pos.y, button->pos.x+button->pos.width, button->pos.y+button->pos.height, button->colorBackgroundDefault);

    textout_centre_ex(page, font, button->text, button->pos.x + button->pos.width/2, button->pos.y + button->pos.height/2-5, makecol(0, 0, 0), -1);
}


int ft_is_mouse_in_rect(t_rect *rect, t_vector mousePos)
{
    return (mousePos.x > rect->x && mousePos.x < rect->x+rect->width && mousePos.y > rect->y && mousePos.y < rect->y+rect->height);
}


void ft_draw_all(t_event *event, t_form *form, t_label *label)
{
    int i, j;

    /* clear */
    clear_to_color(page, makecol(255, 255, 255));

    /* saved forms */
    for (i = 0; i < NB_FORM; ++i)
    {
        if (form[i].type != E_FORM_NONE)
        {
            for (j = 0; j < form[i].nb_point-1; ++j)
                line(page, form[i].point[j].x, form[i].point[j].y, form[i].point[j+1].x, form[i].point[j+1].y, makecol(255, 0, 0));
        }
    }

    /* current form */
    if (event->state == E_STATE_DRAWING)
    {
        for (j = 0; j < event->current.nb_point-1; ++j)
            line(page, event->current.point[j].x, event->current.point[j].y, event->current.point[j+1].x, event->current.point[j+1].y, makecol(255, 0, 0));
        line(page, event->current.point[j].x, event->current.point[j].y, mouse_x, mouse_y, makecol(255, 0, 0));
    }

    /* buttons : menu / forms */
    for (i = 0; i < E_LABEL_LAST; ++i)
        ft_draw_button(&label[i], event->mousePos);

    /* selected buttons */
    if (event->form == E_FORM_LINE)
        rect(page, 600, 0, 700, 50, makecol(255, 100, 100));
    if (event->form == E_FORM_POLYGON)
        rect(page, 700, 0, 800, 50, makecol(255, 100, 100));
}

