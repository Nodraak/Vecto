
#include <stdio.h>

#include <allegro.h>

#include "constantes.h"
#include "draw.h"


int ft_is_mouse_in_rect(s_rect *rect, s_vector mousePos)
{
    return (mousePos.x > rect->x && mousePos.x < rect->x+rect->width && mousePos.y > rect->y && mousePos.y < rect->y+rect->height);
}


void ft_draw_button(s_button *button, s_vector mousePos, int selected)
{
    s_rect *pos = &button->pos;

    /* bg */
    if (ft_is_mouse_in_rect(pos, mousePos))
        rectfill(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, button->colorBackgroundHover);
    else
        rectfill(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, button->colorBackgroundDefault);

    /* text */
    textout_centre_ex(g_page, font, button->text, pos->x+pos->width/2, pos->y+pos->height/2-3, makecol(0, 0, 0), -1);

    /* selected */
    if (selected)
        rect(g_page, pos->x+1, pos->y+1, pos->x+pos->width-1, pos->y+pos->height-1, makecol(150, 0, 0));
}

void ft_draw_line(s_form *form)
{
    int i, colorWanted;
    s_vector *p1 = NULL, *p2 = NULL;

    colorWanted = makecol(form->color.r, form->color.g, form->color.b);

    for (i = 0; i < form->nb_point-1; ++i)
    {
        p1 = &form->point[i];
        p2 = &form->point[i+1];

        line(g_page, p1->x, p1->y, p2->x, p2->y, colorWanted);
    }
}


void ft_draw_polygon(s_form *form)
{
    int i;
    s_vector *p1 = NULL, *p2 = NULL;
    int colorWanted = makecol(form->color.r, form->color.g, form->color.b);
    int colorAlpha = makecol(255, 0, 255);

    clear_to_color(g_page_tmp, colorWanted);

    for (i = 0; i < form->nb_point-1; ++i)
    {
        p1 = &form->point[i];
        p2 = &form->point[i+1];

        line(g_page_tmp, p1->x, p1->y, p2->x, p2->y, colorAlpha);
    }

    /* closing line */
    p1 = &form->point[0];
    p2 = &form->point[form->nb_point-1];

    line(g_page_tmp, p1->x, p1->y, p2->x, p2->y, colorAlpha);

    floodfill(g_page_tmp, 0, 0, colorAlpha);
    masked_blit(g_page_tmp, g_page, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}


void ft_draw_all(s_event *event, s_form *form, s_button *buttons)
{
    int i;
    char tmp[1024];

    /* clear */
    clear_to_color(g_page, makecol(255, 255, 255));

    /* saved forms */
    for (i = 0; i < NB_FORM; ++i)
    {
        if (form[i].type == FORM_POLYGON)
            ft_draw_polygon(&form[i]);
        else if (form[i].type == FORM_LINE)
            ft_draw_line(&form[i]);
    }

    /* current form */
    if (event->state == STATE_DRAWING)
    {
        s_vector *p1 = NULL, *p2 = NULL;
        int color = makecol(event->color.r, event->color.g, event->color.b);

        for (i = 0; i < event->current.nb_point-1; ++i)
        {
            p1 = &event->current.point[i];
            p2 = &event->current.point[i+1];

            line(g_page, p1->x, p1->y, p2->x, p2->y, color);
        }

        p1 = &event->current.point[event->current.nb_point-1];

        line(g_page, p1->x, p1->y, mouse_x, mouse_y, color);
    }

    /* hovered point */
    if (event->form == FORM_EDIT)
    {
        if (event->pointHovered != NULL)
        {
            rect(g_page,
                 event->pointHovered->x-5, event->pointHovered->y-5,
                 event->pointHovered->x+5, event->pointHovered->y+5,
                 makecol(150, 0, 0));
        }
    }

    /* buttons : menu / forms */
    for (i = 0; i < BUTTON_LAST; ++i)
        ft_draw_button(&buttons[i], event->mousePos, event->form == buttons[i].form);

    /* current color components */
    sprintf(tmp, "Rouge : %d", event->color.r);
    textout_right_ex(g_page, font, tmp, 730, 115-3, makecol(0, 0, 0), -1);
    sprintf(tmp, "Vert : %d", event->color.g);
    textout_right_ex(g_page, font, tmp, 730, 145-3, makecol(0, 0, 0), -1);
    sprintf(tmp, "Bleu : %d", event->color.b);
    textout_right_ex(g_page, font, tmp, 730, 175-3, makecol(0, 0, 0), -1);
}

