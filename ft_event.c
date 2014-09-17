
#include <math.h>

#include "constantes.h"
#include "ft_event.h"
#include "ft_form.h"
#include "ft_drawing.h"


void ft_event_init(s_event *event)
{
    memset(event, 0, sizeof(s_event));
}


void ft_event_update(s_drawing *drawing, s_event *event)
{
    static s_event old;
    static int mouse_prev = ~0;
    int i, mouse_now, mouse_click, mouse_unclick;

    event->mousePosPxl.x = mouse_x;
    event->mousePosPxl.y = mouse_y;

    event->mousePosCoord.x = ft_pxl_to_coord(mouse_x, drawing->zoom, drawing->offset.x);
    event->mousePosCoord.y = ft_pxl_to_coord(mouse_y, drawing->zoom, drawing->offset.y);

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
        drawing->actionState = STATE_IDLE;
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


int ft_event_handle(s_drawing *drawing, s_event *event)
{
    if (drawing->actionType == ACTION_TYPE_LINE)
    {
        if (event->mouseDownLeft)
        {
            if (drawing->actionState == STATE_IDLE)
            {
                drawing->drawingForm.nb_point = 1;
                drawing->drawingForm.point[0].x = event->mousePosCoord.x;
                drawing->drawingForm.point[0].y = event->mousePosCoord.y;

                drawing->actionState = STATE_IN_ACTION;
                event->mouseDownLeft = 0;
            }
            else if (drawing->actionState == STATE_IN_ACTION)
            {
                s_form *ptr = ft_form_alloc();

                ptr->point[0].x = drawing->drawingForm.point[0].x;
                ptr->point[0].y = drawing->drawingForm.point[0].y;
                ptr->point[1].x = event->mousePosCoord.x;
                ptr->point[1].y = event->mousePosCoord.y;
                ptr->nb_point = 2;
                ptr->type = ACTION_TYPE_LINE;
                memcpy(&ptr->color, &drawing->color, sizeof(s_color));
                ptr->center = ft_form_get_center(ptr);

                ft_drawing_add_form(drawing->forms, ptr);

                drawing->actionState = STATE_IDLE;
                event->mouseDownLeft = 0;
            }
        }
    }
    else if (drawing->actionType == ACTION_TYPE_POLYGON)
    {
        if (event->mouseDownLeft)
        {
            if (drawing->actionState == STATE_IDLE)
            {
                drawing->drawingForm.nb_point = 1;
                drawing->drawingForm.point[0].x = event->mousePosCoord.x;
                drawing->drawingForm.point[0].y = event->mousePosCoord.y;

                drawing->actionState = STATE_IN_ACTION;
                event->mouseDownLeft = 0;
            }
            else if (drawing->actionState == STATE_IN_ACTION)
            {
                drawing->drawingForm.point[drawing->drawingForm.nb_point].x = event->mousePosCoord.x;
                drawing->drawingForm.point[drawing->drawingForm.nb_point].y = event->mousePosCoord.y;
                drawing->drawingForm.nb_point ++;

                event->mouseDownLeft = 0;
            }
        }
        else if (event->mouseDownRight)
        {
            if (drawing->actionState == STATE_IN_ACTION)
            {
                /* cancel drawing */
                if (drawing->drawingForm.nb_point < 2)
                {
                    drawing->actionState = STATE_IDLE;
                    event->mouseDownRight = 0;
                    return 1;
                }
                else
                {
                    int j;
                    s_form *ptr = ft_form_alloc();

                    for (j = 0; j < drawing->drawingForm.nb_point; ++j)
                    {
                        ptr->point[j].x = drawing->drawingForm.point[j].x;
                        ptr->point[j].y = drawing->drawingForm.point[j].y;
                    }

                    ptr->point[drawing->drawingForm.nb_point].x = event->mousePosCoord.x;
                    ptr->point[drawing->drawingForm.nb_point].y = event->mousePosCoord.y;
                    ptr->nb_point = drawing->drawingForm.nb_point+1;
                    ptr->type = ACTION_TYPE_POLYGON;
                    memcpy(&ptr->color, &drawing->color, sizeof(s_color));
                    ptr->center = ft_form_get_center(ptr);

                    ft_drawing_add_form(drawing->forms, ptr);

                    drawing->actionState = STATE_IDLE;
                    event->mouseDownRight = 0;
                }
            }
        }
    }
    else if (drawing->actionType == ACTION_TYPE_EDIT_POINT)
    {
        if (event->mouseUpLeft)
        {
            event->mouseDownLeft = 0;

            drawing->actionState = STATE_IDLE;
            event->mouseUpLeft = 0;
        }
        else if (event->mouseDownLeft)
        {
            if (drawing->actionState == STATE_IDLE)
            {
                drawing->actionState = STATE_IN_ACTION;
            }
            else if (drawing->actionState == STATE_IN_ACTION)
            {
                if (drawing->editPoint != NULL)
                {
                    int i;

                    drawing->editPoint->x += event->mouseRel.x;
                    drawing->editPoint->y += event->mouseRel.y;

                    for (i = 0; i < NB_FORM; ++i)
                    {
                        if (drawing->forms[i] != NULL)
                            drawing->forms[i]->center = ft_form_get_center(drawing->forms[i]);
                    }
                }
            }
        }

        if (drawing->actionState == STATE_IDLE)
            drawing->editPoint = ft_drawing_get_closer_point(drawing->forms, &event->mousePosCoord, drawing->zoom);
    }
    else if (drawing->actionType == ACTION_TYPE_EDIT_FORM)
    {
        if (event->mouseUpLeft)
        {
            event->mouseDownLeft = 0;

            drawing->actionState = STATE_IDLE;
            event->mouseUpLeft = 0;
        }
        else if (event->mouseDownLeft)
        {
            if (drawing->actionState == STATE_IDLE)
            {
                drawing->actionState = STATE_IN_ACTION;
            }
            else if (drawing->actionState == STATE_IN_ACTION)
            {
                if (drawing->editFormId != -1)
                {
                    int i;
                    s_form *ptr = drawing->forms[drawing->editFormId];

                    for (i = 0; i < ptr->nb_point; ++i)
                    {
                        ptr->point[i].x += event->mouseRel.x;
                        ptr->point[i].y += event->mouseRel.y;
                    }

                    ptr->center = ft_form_get_center(ptr);
                }
            }
        }
        else if (event->mouseUpRight)
        {
            event->mouseDownRight = 0;

            drawing->actionState = STATE_IDLE;
            event->mouseUpRight = 0;
        }
        else if (event->mouseDownRight)
        {
            if (drawing->actionState == STATE_IDLE)
            {
                drawing->actionState = STATE_IN_ACTION;
            }
            else if (drawing->actionState == STATE_IN_ACTION)
            {
                if (drawing->editFormId != -1)
                {
                    int i;
                    s_form *ptr = drawing->forms[drawing->editFormId];

                    double angleMouseOld = atan2(event->mousePosCoord.y - event->mouseRel.y, event->mousePosCoord.x - event->mouseRel.x);
                    double angleMouseNew = atan2(event->mousePosCoord.y, event->mousePosCoord.x);
                    double angleDiff = angleMouseNew - angleMouseOld;

                    /*
                    printf("old=%.2f new=%.2f diff=%.2f\n", angleMouseOld*180, angleMouseNew*180, angleMouseNew*180-angleMouseOld*180);
                    printf("%.2f\n", angleMouseNew*180);
                    printf("\n===\n\n");
                    */

                    ptr->center = ft_form_get_center(ptr);

                    /*
                    printf("center : x=%.0f y=%.0f\n", forms[event->actionTypeId]->center.x, forms[event->actionTypeId]->center.y);
                    */

                    for (i = 0; i < ptr->nb_point; ++i)
                    {
                        double diffX = ptr->point[i].x - ptr->center.x;
                        double diffY = ptr->point[i].y - ptr->center.y;

                        /*
                        printf("point : x=%.0f y=%.0f\n", forms[event->actionTypeId]->point[i].x, forms[event->actionTypeId]->point[i].y);
                        printf("diff : x=%.0f y=%.0f\n", diffX, diffY);
                        */

                        double radius = sqrt(diffX*diffX + diffY*diffY);
                        double angleOld = atan2(diffY, diffX);

                        /*
                        printf("angleOld=%.3f radius=%.1f\n", angleOld*180, radius);
                        */

                        int newX = cos(angleOld + angleDiff) * radius + ptr->center.x;
                        int newY = sin(angleOld + angleDiff) * radius + ptr->center.y;

                        /*
                        printf("newX=%d newY=%d\n", newX, newY);
                        */

                        ptr->point[i].x = newX;
                        ptr->point[i].y = newY;

                        /*
                        printf("\n");
                        */
                    }
                }
            }
        }

        if (drawing->actionState == STATE_IDLE)
            drawing->editFormId = ft_drawing_get_closer_center(drawing->forms, &event->mousePosCoord, drawing->zoom);

        /* keyboard shortcuts */
        if (drawing->actionState == STATE_IDLE)
        {
            /* delete */
            if (event->keyDown[KEY_S] && drawing->editFormId != -1)
            {
                /* in forms tab */
                free(drawing->forms[drawing->editFormId]);
                drawing->forms[drawing->editFormId] = NULL;
                /* working form */
                drawing->editFormId = -1;

                event->keyDown[KEY_S] = 0;
            }
            /* move to background */
            else if (event->keyDown[KEY_R] && drawing->editFormId != -1 && drawing->editFormId > 0)
            {
                ft_swap(&drawing->forms[drawing->editFormId], &drawing->forms[drawing->editFormId-1]);
                drawing->editFormId --;
                event->keyDown[KEY_R] = 0;
            }
            /* move to foreground */
            else if (event->keyDown[KEY_Q] && drawing->editFormId != -1 && drawing->editFormId < NB_FORM-1)
            {
                ft_swap(&drawing->forms[drawing->editFormId], &drawing->forms[drawing->editFormId+1]);
                drawing->editFormId ++;
                event->keyDown[KEY_Q] = 0;
            }
            /* copy */
            else if (event->keyDown[KEY_D] && drawing->editFormId != -1)
            {
                int i = 0;
                s_form *ptr = NULL;

                ptr = ft_form_alloc();
                memcpy(ptr, drawing->forms[drawing->editFormId], sizeof(s_form));

                for (i = 0; i < ptr->nb_point; ++i)
                {
                    ptr->point[i].x += 50;
                    ptr->point[i].y += 50;
                }
                ptr->center = ft_form_get_center(ptr);

                ft_drawing_add_form(drawing->forms, ptr);

                event->keyDown[KEY_D] = 0;
            }
        }
    }
    else
        return 0;

    return 1;
}

