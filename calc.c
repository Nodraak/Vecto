
#include <stdio.h>

#include "constantes.h"
#include "calc.h"
#include "file.h"
#include "draw.h"
#include "init.h"


void ft_swap(s_form **ptr1, s_form **ptr2)
{
    s_form *tmp = NULL;

    tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}


void ft_calc_all(s_event *event, s_form *forms[NB_FORM], s_button *buttons)
{
    if (event->mouseDownLeft)
    {
        event->mouseDownLeft = 0;
        ft_calc_on_mouseDownLeft(event, forms, buttons);
    }

    if (event->mouseDownRight)
    {
        event->mouseDownRight = 0;
        ft_calc_on_mouseDownRight(event, forms);
    }

    if (event->mouseUpLeft)
    {
        event->mouseUpLeft = 0;

        if (event->form == FORM_EDIT_POINT || event->form == FORM_EDIT_FORM)
            event->state = STATE_IDLE;
    }

    if (event->mouseUpRight)
    {
        event->mouseUpRight = 0;
    }

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


void ft_calc_on_mouseDownLeft(s_event *event, s_form *forms[NB_FORM], s_button *buttons)
{
    /* menu */
    if (ft_is_mouse_in_rect(&buttons[BUTTON_NEW].pos, event->mousePosPxl))
        ft_init_form_reset(forms);
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_LOAD].pos, event->mousePosPxl))
    {
        ft_file_load(forms, "data.vecto");
        ft_calc_update_all_barycenter(forms);
    }
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_SAVE].pos, event->mousePosPxl))
        ft_file_save(forms, "data.vecto");
    /* form */
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_EDIT_POINT].pos, event->mousePosPxl))
    {
        event->form = FORM_EDIT_POINT;
        event->state = STATE_IDLE;
    }
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_EDIT_FORM].pos, event->mousePosPxl))
    {
        ft_calc_update_all_barycenter(forms);

        event->form = FORM_EDIT_FORM;
        event->state = STATE_IDLE;
    }
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_LINE].pos, event->mousePosPxl))
    {
        event->form = FORM_LINE;
        event->state = STATE_IDLE;
    }
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_POLYGON].pos, event->mousePosPxl))
    {
        event->form = FORM_POLYGON;
        event->state = STATE_IDLE;
    }
    /* colors */
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_R_MINUS].pos, event->mousePosPxl))
        event->color.r -= COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_R_PLUS].pos, event->mousePosPxl))
        event->color.r += COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_G_MINUS].pos, event->mousePosPxl))
        event->color.g -= COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_G_PLUS].pos, event->mousePosPxl))
        event->color.g += COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_B_MINUS].pos, event->mousePosPxl))
        event->color.b -= COLOR_STEP;
    else if (ft_is_mouse_in_rect(&buttons[BUTTON_B_PLUS].pos, event->mousePosPxl))
        event->color.b += COLOR_STEP;
    /* start action or drawing */
    else if (event->state == STATE_IDLE)
    {
        /* draw start */
        if (event->form == FORM_LINE || event->form == FORM_POLYGON)
        {
            event->current.nb_point = 1;
            event->current.point[0].x = event->mousePosCoord.x;
            event->current.point[0].y = event->mousePosCoord.y;

            event->state = STATE_IN_ACTION;
        }
        /* edit point start */
        else if (event->form == FORM_EDIT_POINT)
        {
            ft_calc_get_closer_point(event, forms);

            event->state = STATE_IN_ACTION;
        }
        /* forms edit start */
        else if (event->form == FORM_EDIT_FORM)
        {
            ft_calc_get_closer_barycenter(event, forms);

            event->state = STATE_IN_ACTION;
        }
        else
            printf("Error event.form %d %s\n", __LINE__, __FILE__);
    }
    /* continue / end action */
    else if (event->state == STATE_IN_ACTION)
    {
        /* line */
        if (event->form == FORM_LINE)
        {
            s_form *ptr = ft_init_form_alloc();

            ptr->point[0].x = event->current.point[0].x;
            ptr->point[0].y = event->current.point[0].y;
            ptr->point[1].x = event->mousePosCoord.x;
            ptr->point[1].y = event->mousePosCoord.y;
            ptr->nb_point = 2;
            ptr->type = FORM_LINE;
            memcpy(&ptr->color, &event->color, sizeof(s_color));
            ptr->barycenter = ft_calc_barycenter(ptr->point, ptr->nb_point);

            ft_calc_add_form(forms, ptr);

            event->state = STATE_IDLE;
        }
        /* polygon */
        else if (event->form == FORM_POLYGON)
        {
            event->current.point[event->current.nb_point].x = event->mousePosCoord.x;
            event->current.point[event->current.nb_point].y = event->mousePosCoord.y;
            event->current.nb_point ++;
        }
        else
            printf("Error event.form %d %s\n", __LINE__, __FILE__);
    }
    else
        printf("Error event.state %d %s\n", __LINE__, __FILE__);
}


void ft_calc_on_mouseDownRight(s_event *event, s_form *forms[NB_FORM])
{
    if (event->form == FORM_POLYGON)
    {
        int j;
        s_form *ptr = ft_init_form_alloc();

        for (j = 0; j < event->current.nb_point; ++j)
        {
            ptr->point[j].x = event->current.point[j].x;
            ptr->point[j].y = event->current.point[j].y;
        }

        ptr->point[event->current.nb_point].x = event->mousePosCoord.x;
        ptr->point[event->current.nb_point].y = event->mousePosCoord.y;
        ptr->nb_point = event->current.nb_point+1;
        ptr->type = FORM_POLYGON;
        memcpy(&ptr->color, &event->color, sizeof(s_color));
        ptr->barycenter = ft_calc_barycenter(ptr->point, ptr->nb_point);

        ft_calc_add_form(forms, ptr);

        event->state = STATE_IDLE;
    }
}


void ft_calc_update_button_color(s_button *buttons, s_color *color)
{
    int i;

    if (color->r < 0) color->r = 0;
    if (color->r > 255) color->r = 255;
    if (color->g < 0) color->g = 0;
    if (color->g > 255) color->g = 255;
    if (color->b < 0) color->b = 0;
    if (color->b > 255) color->b = 255;

    for (i = 0; i < BUTTON_LAST; ++i)
    {
        if ((i == BUTTON_R_MINUS) || (i == BUTTON_R_PLUS) || (i == BUTTON_G_MINUS) || (i == BUTTON_G_PLUS) || (i == BUTTON_B_MINUS) || (i == BUTTON_B_PLUS))
            buttons[i].colorBackgroundDefault = makecol(color->r, color->g, color->b);
    }
}


void ft_calc_update_closer_point_or_barycenter(s_event *event, s_form *forms[NB_FORM])
{
    if (event->state == STATE_IN_ACTION)
    {
        if (event->form == FORM_EDIT_POINT)
        {
            if (event->editPoint != NULL)
            {
                event->editPoint->x += event->mouseRel.x;
                event->editPoint->y += event->mouseRel.y;
            }
        }
        else if (event->form == FORM_EDIT_FORM)
        {
            if (event->formId != -1)
            {
                int i, id = event->formId;

                for (i = 0; i < forms[id]->nb_point; ++i)
                {
                    forms[id]->point[i].x += event->mouseRel.x;
                    forms[id]->point[i].y += event->mouseRel.y;
                }

                forms[id]->barycenter.x += event->mouseRel.x;
                forms[id]->barycenter.y += event->mouseRel.y;
            }
        }
    }
    else if (event->state == STATE_IDLE)
    {
        if (event->form == FORM_EDIT_POINT)
        {
            ft_calc_get_closer_point(event, forms);
        }
        else if (event->form == FORM_EDIT_FORM)
        {
            ft_calc_get_closer_barycenter(event, forms);

            /* delete */
            if (event->keyDown[KEY_S] && event->formId != -1)
            {
                /* in forms tab */
                free(forms[event->formId]);
                forms[event->formId] = NULL;
                /* working form */
                event->formId = -1;

                event->keyDown[KEY_S] = 0;
            }
            /* move to background */
            else if (event->keyDown[KEY_R] && event->formId != -1 && event->formId > 0)
            {
                ft_swap(&forms[event->formId], &forms[event->formId-1]);
                event->formId --;
                event->keyDown[KEY_R] = 0;
            }
            /* move to foreground */
            else if (event->keyDown[KEY_Q] && event->formId != -1 && event->formId < NB_FORM-1)
            {
                ft_swap(&forms[event->formId], &forms[event->formId+1]);
                event->formId ++;
                event->keyDown[KEY_Q] = 0;
            }
            /* copy */
            else if (event->keyDown[KEY_D] && event->formId != -1)
            {
                int i = 0;
                s_form *ptr = NULL;

                ptr = ft_init_form_alloc();
                memcpy(ptr, forms[event->formId], sizeof(s_form));

                for (i = 0; i < ptr->nb_point; ++i)
                {
                    ptr->point[i].x += 50;
                    ptr->point[i].y += 50;
                }
                ptr->barycenter = ft_calc_barycenter(ptr->point, ptr->nb_point);

                ft_calc_add_form(forms, ptr);

                event->keyDown[KEY_D] = 0;
            }
        }
    }
}


void ft_calc_update_all_barycenter(s_form *forms[NB_FORM])
{
    int i;

    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL && forms[i]->type != FORM_NONE)
        {
            forms[i]->barycenter = ft_calc_barycenter(forms[i]->point, forms[i]->nb_point);
        }
    }
}


void ft_calc_get_closer_point(s_event *event, s_form *forms[NB_FORM])
{
    int i, j;
    int dist = (SCREEN_WIDTH * SCREEN_WIDTH) + (SCREEN_HEIGHT * SCREEN_HEIGHT); /* big number */

    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL)
        {
            for (j = 0; j < forms[i]->nb_point; ++j)
            {
                int diff_x = event->mousePosCoord.x - forms[i]->point[j].x;
                int diff_y = event->mousePosCoord.y - forms[i]->point[j].y;
                int cur = diff_x*diff_x + diff_y*diff_y;

                if (cur < dist)
                {
                    dist = cur;
                    event->editPoint = &forms[i]->point[j];
                }
            }
        }
    }

    if (dist > DIST_FOR_HOVER / (event->zoom*event->zoom))
        event->editPoint = NULL;
}


void ft_calc_get_closer_barycenter(s_event *event, s_form *forms[NB_FORM])
{
    int i;
    int dist = (SCREEN_WIDTH * SCREEN_WIDTH) + (SCREEN_HEIGHT * SCREEN_HEIGHT); /* big number */

    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL)
        {
            int diff_x = event->mousePosCoord.x - forms[i]->barycenter.x;
            int diff_y = event->mousePosCoord.y - forms[i]->barycenter.y;
            int cur = diff_x*diff_x + diff_y*diff_y;

            if (cur < dist)
            {
                dist = cur;
                event->formId = i;
            }
        }
    }

    if (dist > DIST_FOR_HOVER / (event->zoom*event->zoom))
        event->formId = -1;
}



s_vector ft_calc_barycenter(s_vector *points, int nb_point)
{
    s_vector ret = {0, 0};
    int i;

    for (i = 0; i < nb_point; ++i)
    {
        ret.x += points[i].x;
        ret.y += points[i].y;
    }

    ret.x /= nb_point;
    ret.y /= nb_point;

    return ret;
}


void ft_calc_add_form(s_form *forms[NB_FORM], s_form *new)
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


int ft_pxl_to_coord(double pxl, double zoom, double offset)
{
    return pxl/zoom - offset;
}


int ft_coord_to_pxl(double coord, double zoom, double offset)
{
    return (coord+offset) * zoom;
}

