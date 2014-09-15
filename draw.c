
#include <stdio.h>

#include <allegro.h>

#include "constantes.h"
#include "draw.h"
#include "calc.h"


int ft_is_mouse_in_rect(s_rect *rect, s_vector mousePos)
{
    return (mousePos.x > rect->x && mousePos.x < rect->x+rect->width && mousePos.y > rect->y && mousePos.y < rect->y+rect->height);
}


void ft_scale_coord(double zoom, s_vector offset, int *x1, int *y1, int *x2, int *y2)
{
    *x1 = ft_coord_to_pxl(*x1, zoom, offset.x);
    *y1 = ft_coord_to_pxl(*y1, zoom, offset.y);
    *x2 = ft_coord_to_pxl(*x2, zoom, offset.x);
    *y2 = ft_coord_to_pxl(*y2, zoom, offset.y);
}

void my_line(BITMAP *bmp, s_event *event, int x1, int y1, int x2, int y2, int c, int flags)
{
    if (flags & FLAG_SCALE_COORD)
        ft_scale_coord(event->zoom, event->offset, &x1, &y1, &x2, &y2);

    line(bmp, x1, y1, x2, y2, c);
    if (flags & FLAG_FAT_LINE)
    {
        line(bmp, x1+1, y1, x2+1, y2, c);
        line(bmp, x1-1, y1, x2-1, y2, c);
        line(bmp, x1, y1+1, x2, y2+1, c);
        line(bmp, x1, y1-1, x2, y2-1, c);
    }
}


void my_rect(BITMAP *bmp, s_event *event, int x1, int y1, int x2, int y2, int c)
{
    ft_scale_coord(event->zoom, event->offset, &x1, &y1, &x2, &y2);
    rect(bmp, x1, y1, x2, y2, c);
}


void ft_draw_button(s_button *button, s_event *event, int selected)
{
    s_rect *pos = &button->pos;

    /* bg */
    if (ft_is_mouse_in_rect(pos, event->mousePosPxl))
        rectfill(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, button->colorBackgroundHover);
    else
        rectfill(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, button->colorBackgroundDefault);

    rect(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, makecol(128, 128, 128));

    /* text */
    textout_centre_ex(g_page, font, button->text, pos->x+pos->width/2, pos->y+pos->height/2-3, makecol(0, 0, 0), -1);

    /* selected */
    if (selected)
        my_line(g_page, event, pos->x+10, pos->y+35, pos->x+90, pos->y+35, makecol(128, 128, 128), FLAG_FAT_LINE);
}


void ft_draw_line(s_form *form, s_event *event)
{
    int i, colorWanted;
    s_vector *p1 = NULL, *p2 = NULL;

    colorWanted = makecol(form->color.r, form->color.g, form->color.b);

    for (i = 0; i < form->nb_point-1; ++i)
    {
        p1 = &form->point[i];
        p2 = &form->point[i+1];

        my_line(g_page, event, p1->x, p1->y, p2->x, p2->y, colorWanted, FLAG_FAT_LINE | FLAG_SCALE_COORD);
    }
}


void ft_draw_polygon(s_form *form, s_event *event)
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

        my_line(g_page_tmp, event, p1->x, p1->y, p2->x, p2->y, colorAlpha, FLAG_SCALE_COORD);
    }

    /* closing line */
    p1 = &form->point[0];
    p2 = &form->point[form->nb_point-1];

    my_line(g_page_tmp, event, p1->x, p1->y, p2->x, p2->y, colorAlpha, FLAG_SCALE_COORD);

    /* TODO : upgrade this dirty hack */
    floodfill(g_page_tmp, 0, 0, colorAlpha);
    floodfill(g_page_tmp, 0, SCREEN_HEIGHT-1, colorAlpha);
    floodfill(g_page_tmp, SCREEN_WIDTH-1, 0, colorAlpha);
    floodfill(g_page_tmp, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, colorAlpha);

    masked_blit(g_page_tmp, g_page, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}


void ft_draw_all(s_event *event, s_form *forms[NB_FORM], s_button *buttons)
{
    int i;
    char tmp[1024];

    /* clear */
    clear_to_color(g_page, makecol(255, 255, 255));

    /* saved forms */
    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL)
        {
            if (forms[i]->type == FORM_POLYGON)
                ft_draw_polygon(forms[i], event);
            else if (forms[i]->type == FORM_LINE)
                ft_draw_line(forms[i], event);
        }
    }

    /* current form */
    if (event->state == STATE_IN_ACTION && (event->form == FORM_LINE || event->form == FORM_POLYGON))
    {
        s_vector *p1 = NULL, *p2 = NULL;
        int color = makecol(event->color.r, event->color.g, event->color.b);

        for (i = 0; i < event->current.nb_point-1; ++i)
        {
            p1 = &event->current.point[i];
            p2 = &event->current.point[i+1];

            my_line(g_page, event, p1->x, p1->y, p2->x, p2->y, color, FLAG_FAT_LINE | FLAG_SCALE_COORD);
        }

        p1 = &event->current.point[event->current.nb_point-1];

        my_line(g_page, event, p1->x, p1->y, event->mousePosCoord.x, event->mousePosCoord.y, color, FLAG_FAT_LINE | FLAG_SCALE_COORD);
    }

    /* hovered point */
    if (event->form == FORM_EDIT_POINT)
    {
        if (event->editPoint != NULL)
        {
            my_rect(g_page, event,
                 event->editPoint->x-5, event->editPoint->y-5,
                 event->editPoint->x+5, event->editPoint->y+5,
                 makecol(150, 0, 0));
        }
    }
    if (event->form == FORM_EDIT_FORM)
    {
        if (event->formId != -1)
        {
            my_rect(g_page, event,
                 forms[event->formId]->center.x-5, forms[event->formId]->center.y-5,
                 forms[event->formId]->center.x+5, forms[event->formId]->center.y+5,
                 makecol(150, 0, 0));
        }
    }

    /* buttons : menu / forms */
    for (i = 0; i < BUTTON_LAST; ++i)
        ft_draw_button(&buttons[i], event, event->form == buttons[i].form);

    /* current color components */
    sprintf(tmp, "Rouge : %d", event->color.r);
    textout_right_ex(g_page, font, tmp, 730, 65-3, makecol(0, 0, 0), -1);
    sprintf(tmp, "Vert : %d", event->color.g);
    textout_right_ex(g_page, font, tmp, 730, 95-3, makecol(0, 0, 0), -1);
    sprintf(tmp, "Bleu : %d", event->color.b);
    textout_right_ex(g_page, font, tmp, 730, 125-3, makecol(0, 0, 0), -1);
}

