
#include <allegro.h>

#include "constantes.h"
#include "draw.h"


void ft_draw_button(t_rect *button, int colorDefault, int colorHover, t_vector mousePos)
{
    if (mousePos.x > button->x && mousePos.x < button->x+button->width && mousePos.y > button->y && mousePos.y < button->y+button->height)
        rectfill(page, button->x, button->y, button->x+button->width, button->y+button->height, colorHover);
    else
        rectfill(page, button->x, button->y, button->x+button->width, button->y+button->height, colorDefault);
}


int ft_is_mouse_in_rect(t_rect *rect, t_vector mousePos)
{
    return (mousePos.x > rect->x && mousePos.x < rect->x+rect->width && mousePos.y > rect->y && mousePos.y < rect->y+rect->height);
}


void ft_draw_all(t_event *event, t_form *form, t_rect *button_load, t_rect *button_save)
{
    int i;

    clear_to_color(page, makecol(255, 255, 255));
    for (i = 0; i < NB_FORM; ++i)
    {
        if (form[i].used)
            line(page, form[i].p1.x, form[i].p1.y, form[i].p2.x, form[i].p2.y, makecol(255, 0, 0));
    }

    if (event->state == E_STATE_DRAW_WAIT)
        line(page, event->current_line.x, event->current_line.y, mouse_x, mouse_y, makecol(255, 0, 0));

    ft_draw_button(button_load, makecol(50, 50, 255), makecol(80, 80, 255), event->mousePos);
    ft_draw_button(button_save, makecol(50, 50, 255), makecol(80, 80, 255), event->mousePos);

    textout_centre_ex(page, font, "Load", 50, 20, makecol(0, 0, 0), -1);
    textout_centre_ex(page, font, "Save", 150, 20, makecol(0, 0, 0), -1);
}
