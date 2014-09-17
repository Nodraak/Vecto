
#include <stdio.h>

#include "constantes.h"
#include "ft_form.h"
#include "ft_allegro.h"


s_form *ft_form_alloc(void)
{
    s_form *ptr = NULL;

    ptr = malloc(sizeof(s_form));
    if (ptr == NULL)
    {
        fprintf(stderr, "Error malloc new form.\n");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

s_vector ft_form_get_center(s_form *form)
{
    s_vector ret = {0, 0}, min = {BIG_NUMBER_COORD, BIG_NUMBER_COORD}, max = {-BIG_NUMBER_COORD, -BIG_NUMBER_COORD};
    int i;

    for (i = 0; i < form->nb_point; ++i)
    {
        if (form->point[i].x < min.x) min.x = form->point[i].x;
        if (form->point[i].x > max.x) max.x = form->point[i].x;

        if (form->point[i].y < min.y) min.y = form->point[i].y;
        if (form->point[i].y > max.y) max.y = form->point[i].y;
    }

    ret.x = (min.x+max.x) / 2;
    ret.y = (min.y+max.y) / 2;

    return ret;
}


void ft_form_draw_all(s_form *forms[NB_FORM], s_drawing *drawing)
{
    int i;

    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL)
        {
            if (forms[i]->type == ACTION_TYPE_POLYGON)
                ft_form_draw_polygon(forms[i], drawing);
            else if (forms[i]->type == ACTION_TYPE_LINE)
                ft_form_draw_line(forms[i], drawing);
        }
    }
}

void ft_form_draw_hovered(s_drawing *drawing)
{
    if (drawing->actionType == ACTION_TYPE_EDIT_POINT)
    {
        if (drawing->editPoint != NULL)
        {
            ft_allegro_rect(drawing->g_page, drawing,
                 drawing->editPoint->x-5, drawing->editPoint->y-5,
                 drawing->editPoint->x+5, drawing->editPoint->y+5,
                 makecol(150, 0, 0));
        }
    }
    else if (drawing->actionType == ACTION_TYPE_EDIT_FORM)
    {
        if (drawing->editFormId != -1)
        {
            s_vector *center = &drawing->forms[drawing->editFormId]->center;

            ft_allegro_rect(drawing->g_page, drawing,
                 center->x-5, center->y-5,
                 center->x+5, center->y+5,
                 makecol(150, 0, 0));
        }
    }
}

void ft_form_draw_line(s_form *form, s_drawing *drawing)
{
    int i, colorWanted;
    s_vector *p1 = NULL, *p2 = NULL;

    colorWanted = makecol(form->color.r, form->color.g, form->color.b);

    for (i = 0; i < form->nb_point-1; ++i)
    {
        p1 = &form->point[i];
        p2 = &form->point[i+1];

        ft_allegro_line(drawing->g_page, drawing, p1->x, p1->y, p2->x, p2->y, colorWanted, FLAG_FAT_LINE | FLAG_SCALE_COORD);
    }
}


void ft_form_draw_polygon(s_form *form, s_drawing *drawing)
{
    int i;
    s_vector *p1 = NULL, *p2 = NULL;
    int colorWanted = makecol(form->color.r, form->color.g, form->color.b);
    int colorAlpha = makecol(255, 0, 255);

    clear_to_color(drawing->g_page_tmp, colorWanted);

    for (i = 0; i < form->nb_point-1; ++i)
    {
        p1 = &form->point[i];
        p2 = &form->point[i+1];

        ft_allegro_line(drawing->g_page_tmp, drawing, p1->x, p1->y, p2->x, p2->y, colorAlpha, FLAG_SCALE_COORD);
    }

    /* closing line */
    p1 = &form->point[0];
    p2 = &form->point[form->nb_point-1];

    ft_allegro_line(drawing->g_page_tmp, drawing, p1->x, p1->y, p2->x, p2->y, colorAlpha, FLAG_SCALE_COORD);

    /* TODO : upgrade this dirty hack */
    floodfill(drawing->g_page_tmp, 0, 0, colorAlpha);
    floodfill(drawing->g_page_tmp, 0, SCREEN_HEIGHT-1, colorAlpha);
    floodfill(drawing->g_page_tmp, SCREEN_WIDTH-1, 0, colorAlpha);
    floodfill(drawing->g_page_tmp, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, colorAlpha);

    masked_blit(drawing->g_page_tmp, drawing->g_page, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

