
#include <math.h>

#include "constantes.h"
#include "ft_event.h"
#include "ft_form.h"
#include "ft_drawing.h"


void ft_event_init(s_event *event)
{
    memset(event, 0, sizeof(s_event));

    event->state = STATE_IDLE;
    event->form = FORM_LINE;
    event->color.r = 120;
    event->color.g = 120;
    event->color.b = 120;
    event->offset.x = 0;
    event->offset.y = 0;
    event->zoom = 1;

    event->formId = -1;
}


void ft_event_update(s_event *event)
{
    static s_event old;
    static int mouse_prev;
    int i, mouse_now, mouse_click, mouse_unclick;

    event->mousePosPxl.x = mouse_x;
    event->mousePosPxl.y = mouse_y;

    event->mousePosCoord.x = ft_pxl_to_coord(mouse_x, event->zoom, event->offset.x);
    event->mousePosCoord.y = ft_pxl_to_coord(mouse_y, event->zoom, event->offset.y);

    event->mouseRel.x = event->mousePosCoord.x - old.mousePosCoord.x;
    event->mouseRel.y = event->mousePosCoord.y - old.mousePosCoord.y;

    mouse_now = mouse_b;
    mouse_click = mouse_now & ~mouse_prev;
    mouse_unclick = ~mouse_now & mouse_prev;

    if (mouse_click & 1) event->mouseDownLeft = 1;
    if (mouse_click & 2) event->mouseDownRight = 1;

    if (mouse_unclick & 1) event->mouseUpLeft = 1;
    if (mouse_unclick & 2) event->mouseUpRight = 1;

    for (i = 0; i < KEY_MAX; ++i)
    {
        if (key[i] != old.keyDown[i])
        {
            event->keyDown[i] = key[i];
            old.keyDown[i] = key[i];
        }
    }

    if (event->keyDown[KEY_ESC])
        event->state = STATE_IDLE;
    if (event->keyDown[KEY_A])
        g_quit = 1;


    old.mousePosCoord.x = event->mousePosCoord.x;
    old.mousePosCoord.y = event->mousePosCoord.y;
    mouse_prev = mouse_now;
}


void ft_swap(s_form **ptr1, s_form **ptr2)
{
    s_form *tmp = NULL;

    tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}


int ft_event_handle(s_event *event, s_form *forms[NB_FORM])
{
    if (event->form == FORM_LINE)
    {
        if (event->mouseDownLeft)
        {
            if (event->state == STATE_IDLE)
            {
                event->current.nb_point = 1;
                event->current.point[0].x = event->mousePosCoord.x;
                event->current.point[0].y = event->mousePosCoord.y;

                event->state = STATE_IN_ACTION;
                event->mouseDownLeft = 0;
            }
            else if (event->state == STATE_IN_ACTION)
            {
                s_form *ptr = ft_form_alloc();

                ptr->point[0].x = event->current.point[0].x;
                ptr->point[0].y = event->current.point[0].y;
                ptr->point[1].x = event->mousePosCoord.x;
                ptr->point[1].y = event->mousePosCoord.y;
                ptr->nb_point = 2;
                ptr->type = FORM_LINE;
                memcpy(&ptr->color, &event->color, sizeof(s_color));
                ptr->center = ft_form_get_center(ptr);

                ft_drawing_add_form(forms, ptr);

                event->state = STATE_IDLE;
                event->mouseDownLeft = 0;
            }
        }
    }
    else if (event->form == FORM_POLYGON)
    {
        if (event->mouseDownLeft)
        {
            if (event->state == STATE_IDLE)
            {
                event->current.nb_point = 1;
                event->current.point[0].x = event->mousePosCoord.x;
                event->current.point[0].y = event->mousePosCoord.y;

                event->state = STATE_IN_ACTION;
                event->mouseDownLeft = 0;
            }
            else if (event->state == STATE_IN_ACTION)
            {
                event->current.point[event->current.nb_point].x = event->mousePosCoord.x;
                event->current.point[event->current.nb_point].y = event->mousePosCoord.y;
                event->current.nb_point ++;

                event->mouseDownLeft = 0;
            }
        }
        else if (event->mouseDownRight)
        {
            if (event->state == STATE_IN_ACTION)
            {
                /* cancel drawing */
                if (event->current.nb_point < 2)
                {
                    event->state = STATE_IDLE;
                    event->mouseDownRight = 0;
                    return 1;
                }
                else
                {
                    int j;
                    s_form *ptr = ft_form_alloc();

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
                    ptr->center = ft_form_get_center(ptr);

                    ft_drawing_add_form(forms, ptr);

                    event->state = STATE_IDLE;
                    event->mouseDownRight = 0;
                }
            }
        }
    }
    else if (event->form == FORM_EDIT_POINT)
    {
        if (event->mouseUpLeft)
        {
            event->mouseDownLeft = 0;

            event->state = STATE_IDLE;
            event->mouseUpLeft = 0;
        }
        else if (event->mouseDownLeft)
        {
            if (event->state == STATE_IDLE)
            {
                event->state = STATE_IN_ACTION;
            }
            else if (event->state == STATE_IN_ACTION)
            {
                if (event->editPoint != NULL)
                {
                    int i;

                    event->editPoint->x += event->mouseRel.x;
                    event->editPoint->y += event->mouseRel.y;

                    for (i = 0; i < NB_FORM; ++i)
                    {
                        if (forms[i] != NULL)
                            forms[i]->center = ft_form_get_center(forms[i]);
                    }
                }
            }
        }

        if (event->state == STATE_IDLE)
            event->editPoint = ft_drawing_get_closer_point(forms, &event->mousePosCoord, event->zoom);
    }
    else if (event->form == FORM_EDIT_FORM)
    {
        if (event->mouseUpLeft)
        {
            event->mouseDownLeft = 0;

            event->state = STATE_IDLE;
            event->mouseUpLeft = 0;
        }
        else if (event->mouseDownLeft)
        {
            if (event->state == STATE_IDLE)
            {
                event->state = STATE_IN_ACTION;
            }
            else if (event->state == STATE_IN_ACTION)
            {
                if (event->formId != -1)
                {
                    int i, id = event->formId;

                    for (i = 0; i < forms[id]->nb_point; ++i)
                    {
                        forms[id]->point[i].x += event->mouseRel.x;
                        forms[id]->point[i].y += event->mouseRel.y;
                    }

                    forms[id]->center = ft_form_get_center(forms[id]);
                }
            }
        }
        else if (event->mouseUpRight)
        {
            event->mouseDownRight = 0;

            event->state = STATE_IDLE;
            event->mouseUpRight = 0;
        }
        else if (event->mouseDownRight)
        {
            if (event->state == STATE_IDLE)
            {
                event->state = STATE_IN_ACTION;
            }
            else if (event->state == STATE_IN_ACTION)
            {
                if (event->formId != -1)
                {
                    int i;

                    double angleMouseOld = atan2(event->mousePosCoord.y - event->mouseRel.y, event->mousePosCoord.x - event->mouseRel.x);
                    double angleMouseNew = atan2(event->mousePosCoord.y, event->mousePosCoord.x);
                    double angleDiff = angleMouseNew - angleMouseOld;

                    /*
                    printf("old=%.2f new=%.2f diff=%.2f\n", angleMouseOld*180, angleMouseNew*180, angleMouseNew*180-angleMouseOld*180);
                    printf("%.2f\n", angleMouseNew*180);
                    printf("\n===\n\n");
                    */

                    forms[event->formId]->center = ft_form_get_center(forms[event->formId]);

                    /*
                    printf("center : x=%.0f y=%.0f\n", forms[event->formId]->center.x, forms[event->formId]->center.y);
                    */

                    for (i = 0; i < forms[event->formId]->nb_point; ++i)
                    {
                        double diffX = forms[event->formId]->point[i].x - forms[event->formId]->center.x;
                        double diffY = forms[event->formId]->point[i].y - forms[event->formId]->center.y;

                        /*
                        printf("point : x=%.0f y=%.0f\n", forms[event->formId]->point[i].x, forms[event->formId]->point[i].y);
                        printf("diff : x=%.0f y=%.0f\n", diffX, diffY);
                        */

                        double radius = sqrt(diffX*diffX + diffY*diffY);
                        double angleOld = atan2(diffY, diffX);

                        /*
                        printf("angleOld=%.3f radius=%.1f\n", angleOld*180, radius);
                        */

                        int newX = cos(angleOld + angleDiff) * radius + forms[event->formId]->center.x;
                        int newY = sin(angleOld + angleDiff) * radius + forms[event->formId]->center.y;

                        /*
                        printf("newX=%d newY=%d\n", newX, newY);
                        */

                        forms[event->formId]->point[i].x = newX;
                        forms[event->formId]->point[i].y = newY;

                        /*
                        printf("\n");
                        */
                    }
                }
            }
        }

        if (event->state == STATE_IDLE)
            event->formId = ft_drawing_get_closer_center(forms, &event->mousePosCoord, event->zoom);

        /* keyboard shortcuts */
        if (event->state == STATE_IDLE)
        {
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

                ptr = ft_form_alloc();
                memcpy(ptr, forms[event->formId], sizeof(s_form));

                for (i = 0; i < ptr->nb_point; ++i)
                {
                    ptr->point[i].x += 50;
                    ptr->point[i].y += 50;
                }
                ptr->center = ft_form_get_center(ptr);

                ft_drawing_add_form(forms, ptr);

                event->keyDown[KEY_D] = 0;
            }
        }
    }
    else
        return 0;

    return 1;
}

