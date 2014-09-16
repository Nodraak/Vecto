

#include <stdio.h>
#include <stdlib.h>


#include "constantes.h"
#include "ft_drawing.h"
#include "ft_form.h"
#include "ft_allegro.h"
#include "ft_button.h"


void ft_drawing_reset_forms(s_form **forms)
{
    int i;

    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL)
        {
            free(forms[i]);
            forms[i] = NULL;
        }
    }
}


int ft_file_goto_next_nb(char *str, int index)
{
    /* goto space */
    while (str[index] != '\0' && str[index] != ' ')
        index ++;

    if (str[index] == '\0')
        return index;

    /* skip space */
    index ++;

    return index;
}


void ft_drawing_load(s_form *forms[NB_FORM])
{
    FILE *f = NULL;
    int ret = 0, i_form = 0, i_tmp, i_point;
    char tmp[1024], path[1024] = "data.vecto";

    /* open file */
    f = fopen(path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", path);
        exit(EXIT_FAILURE);
    }

    /* clear old forms */
    ft_drawing_reset_forms(forms);

    /* for each line = for each form */
    i_form = 0;
    while (fgets(tmp, 1024-1, f) != NULL)
    {
        if (i_form >= NB_FORM)
        {
            printf("Error no space for another form %d %s\n", __LINE__, __FILE__);
            exit(EXIT_FAILURE);
        }

        i_tmp = 0;
        forms[i_form] = ft_form_alloc();

        /* get type of form */
        ret = sscanf(&tmp[i_tmp], "%d", (int*)&forms[i_form]->type);
        if (ret != 1)
        {
            printf("Error form.type %d %s\n", __LINE__, __FILE__);
            break;
        }

        i_tmp = ft_file_goto_next_nb(tmp, i_tmp);

        /* color */
        ret = sscanf(&tmp[i_tmp], "%d %d %d", &forms[i_form]->color.r, &forms[i_form]->color.g, &forms[i_form]->color.b);
        if (ret != 3)
        {
            printf("Error form.color %d %s\n", __LINE__, __FILE__);
            break;
        }

        i_tmp = ft_file_goto_next_nb(tmp, i_tmp);
        i_tmp = ft_file_goto_next_nb(tmp, i_tmp);
        i_tmp = ft_file_goto_next_nb(tmp, i_tmp);

        /* for each point of form */
        i_point = 0;
        for (;;)
        {
            ret = sscanf(&tmp[i_tmp], "%lf %lf", &forms[i_form]->point[i_point].x, &forms[i_form]->point[i_point].y);
            if (ret != 2) /* end of line (or unexpeted error, but osef) */
                break;

            i_point ++;

            i_tmp = ft_file_goto_next_nb(tmp, i_tmp);
            i_tmp = ft_file_goto_next_nb(tmp, i_tmp);

            if (tmp[i_tmp] == '\0')
                break;
        }

        /* save nb_point */
        if (i_point < 2)
            printf("Error form.point %d %s - %d\n", __LINE__, __FILE__, i_point);

        forms[i_form]->nb_point = i_point;
        forms[i_form]->center = ft_form_get_center(forms[i_form]);

        i_form ++;
    }

    fclose(f);
}


void ft_drawing_save(s_form *forms[NB_FORM])
{
    FILE *f = NULL;
    int i = 0, j = 0;
    char path[1024] = "data.vecto";

    f = fopen(path, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", path);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL)
        {
            fprintf(f, "%d", forms[i]->type);
            fprintf(f, " %d %d %d", forms[i]->color.r, forms[i]->color.g, forms[i]->color.b);

            for (j = 0; j < forms[i]->nb_point; ++j)
                fprintf(f, " %f %f", forms[i]->point[j].x, forms[i]->point[j].y);

            fprintf(f, "\n");
        }
    }

    fclose(f);
}


void ft_drawing_add_form(s_form *forms[NB_FORM], s_form *new)
{
    int i = 0;

    while (i < NB_FORM && forms[i] != NULL)
        ++i;

    if (i >= NB_FORM)
    {
        printf("No more space for another form %d %s.\n", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    forms[i] = new;
}


int ft_drawing_get_closer_center(s_form *forms[NB_FORM], s_vector *mousePosCoord, double zoom)
{
    int i;
    int dist = (SCREEN_WIDTH * SCREEN_WIDTH) + (SCREEN_HEIGHT * SCREEN_HEIGHT); /* big number */
    int ret = -1;

    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL)
        {
            int diff_x = mousePosCoord->x - forms[i]->center.x;
            int diff_y = mousePosCoord->y - forms[i]->center.y;
            int cur = diff_x*diff_x + diff_y*diff_y;

            if (cur < dist)
            {
                dist = cur;
                ret = i;
            }
        }
    }

    if (dist > DIST_FOR_HOVER / (zoom*zoom))
        ret = -1;

    return ret;
}


s_vector *ft_drawing_get_closer_point(s_form *forms[NB_FORM], s_vector *mousePosCoord, double zoom)
{
    int i, j;
    int dist = (SCREEN_WIDTH * SCREEN_WIDTH) + (SCREEN_HEIGHT * SCREEN_HEIGHT); /* big number */
    s_vector *ret = NULL;

    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL)
        {
            for (j = 0; j < forms[i]->nb_point; ++j)
            {
                int diff_x = mousePosCoord->x - forms[i]->point[j].x;
                int diff_y = mousePosCoord->y - forms[i]->point[j].y;
                int cur = diff_x*diff_x + diff_y*diff_y;

                if (cur < dist)
                {
                    dist = cur;
                    ret = &forms[i]->point[j];
                }
            }
        }
    }

    if (dist > DIST_FOR_HOVER / (zoom*zoom))
        ret = NULL;

    return ret;
}


void ft_drawing_update_coord(s_event *event)
{
    /* zoom minus */
    if (event->keyDown[KEY_SEMICOLON])
        event->zoom /= ZOOM_FACTOR;
    /* zoom plus */
    if (event->keyDown[KEY_P])
        event->zoom *= ZOOM_FACTOR;
    /* move drawing */
    if (event->keyDown[KEY_UP])
        event->offset.y += MOVE_SPEED / event->zoom;
    if (event->keyDown[KEY_DOWN])
        event->offset.y -= MOVE_SPEED / event->zoom;
    if (event->keyDown[KEY_RIGHT])
        event->offset.x -= MOVE_SPEED / event->zoom;
    if (event->keyDown[KEY_LEFT])
        event->offset.x += MOVE_SPEED / event->zoom;

}

void ft_drawing_draw_tmp_form(s_event *event)
{
    int i;

    if (event->state == STATE_IN_ACTION && (event->form == FORM_LINE || event->form == FORM_POLYGON))
    {
        s_vector *p1 = NULL, *p2 = NULL;
        int color = makecol(event->color.r, event->color.g, event->color.b);

        for (i = 0; i < event->current.nb_point-1; ++i)
        {
            p1 = &event->current.point[i];
            p2 = &event->current.point[i+1];

            ft_allegro_line(g_page, event, p1->x, p1->y, p2->x, p2->y, color, FLAG_FAT_LINE | FLAG_SCALE_COORD);
        }

        p1 = &event->current.point[event->current.nb_point-1];

        ft_allegro_line(g_page, event, p1->x, p1->y, event->mousePosCoord.x, event->mousePosCoord.y, color, FLAG_FAT_LINE | FLAG_SCALE_COORD);
    }
}


