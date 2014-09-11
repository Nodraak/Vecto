
#include <allegro.h>

#include "constantes.h"
#include "draw.h"


void ft_draw_button(t_rect *button, int colorDefault, int colorHover, t_vector mousePos)
{
    if (ft_is_mouse_in_rect(button, mousePos))
        rectfill(page, button->x, button->y, button->x+button->width, button->y+button->height, colorHover);
    else
        rectfill(page, button->x, button->y, button->x+button->width, button->y+button->height, colorDefault);
}


int ft_is_mouse_in_rect(t_rect *rect, t_vector mousePos)
{
    return (mousePos.x > rect->x && mousePos.x < rect->x+rect->width && mousePos.y > rect->y && mousePos.y < rect->y+rect->height);
}


void ft_draw_all(t_event *event, t_form *form, t_rect *label)
{
    int i;

    clear_to_color(page, makecol(255, 255, 255));
    for (i = 0; i < NB_FORM; ++i)
    {
        if (form[i].used && form[i].type == E_FORM_LINE)
            line(page, form[i].point[0].x, form[i].point[0].y, form[i].point[1].x, form[i].point[1].y, makecol(255, 0, 0));
    }

    if (event->state == E_STATE_DRAW_WAIT)
        line(page, event->current.point[0].x, event->current.point[0].y, mouse_x, mouse_y, makecol(255, 0, 0));

    for (i = 0; i < E_LABEL_LAST; ++i)
        ft_draw_button(&label[i], makecol(50, 50, 255), makecol(80, 80, 255), event->mousePos);

    textout_centre_ex(page, font, "Charger", 50, 20, makecol(0, 0, 0), -1);
    textout_centre_ex(page, font, "Sauver", 150, 20, makecol(0, 0, 0), -1);

    textout_centre_ex(page, font, "Ligne", 650, 20, makecol(0, 0, 0), -1);
    textout_centre_ex(page, font, "Polygone", 750, 20, makecol(0, 0, 0), -1);

    if (event->selected == E_FORM_LINE)
        rect(page, 600, 0, 700, 50, makecol(255, 100, 100));
    if (event->selected == E_FORM_POLYGON)
        rect(page, 700, 0, 800, 50, makecol(255, 100, 100));
}

