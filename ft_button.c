
#include "stdio.h"

#include "constantes.h"
#include "ft_button.h"
#include "ft_allegro.h"
#include "ft_drawing.h"


int ft_button_is_vector_in_rect(s_rect *rect, s_vector *vect)
{
    return vect->x > rect->x && vect->x < rect->x + rect->width && vect->y > rect->y && vect->y < rect->y + rect->height;
}


void ft_button_load(s_button *buttons)
{
    int i, ret;
    FILE *f = NULL;
    s_button tmp;

    f = fopen("buttons.data", "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < BUTTON_LAST; ++i)
    {
        ret = fscanf(f, "%d %d %d %d | %d %[^\n]",
                     &tmp.pos.x, &tmp.pos.y, &tmp.pos.width, &tmp.pos.height,
                     (int *)&tmp.form, tmp.text);

        if (ret != 6)
        {
            fprintf(stderr, "Error loading button %d (0 indexed).\n", i);
            exit(EXIT_FAILURE);
        }

        tmp.colorBackgroundDefault = BUTTON_COLOR_DEFAULT;
        tmp.colorBackgroundHover = BUTTON_COLOR_HOVER;

        memcpy(&buttons[i], &tmp, sizeof(s_button));
    }
}


void ft_button_update_color(s_button *buttons, s_color *color)
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


int ft_button_update(s_event *event, s_form *forms[NB_FORM], s_button *buttons)
{
    /* menu */
    if (ft_button_is_vector_in_rect(&buttons[BUTTON_NEW].pos, &event->mousePosPxl))
        ft_drawing_reset_forms(forms);
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_LOAD].pos, &event->mousePosPxl))
        ft_drawing_load(forms);
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_SAVE].pos, &event->mousePosPxl))
        ft_drawing_save(forms);
    /* form */
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_EDIT_POINT].pos, &event->mousePosPxl))
        event->form = FORM_EDIT_POINT;
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_EDIT_FORM].pos, &event->mousePosPxl))
        event->form = FORM_EDIT_FORM;
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_LINE].pos, &event->mousePosPxl))
        event->form = FORM_LINE;
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_POLYGON].pos, &event->mousePosPxl))
        event->form = FORM_POLYGON;
    /* colors */
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_R_MINUS].pos, &event->mousePosPxl))
        event->color.r -= COLOR_STEP;
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_R_PLUS].pos, &event->mousePosPxl))
        event->color.r += COLOR_STEP;
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_G_MINUS].pos, &event->mousePosPxl))
        event->color.g -= COLOR_STEP;
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_G_PLUS].pos, &event->mousePosPxl))
        event->color.g += COLOR_STEP;
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_B_MINUS].pos, &event->mousePosPxl))
        event->color.b -= COLOR_STEP;
    else if (ft_button_is_vector_in_rect(&buttons[BUTTON_B_PLUS].pos, &event->mousePosPxl))
        event->color.b += COLOR_STEP;
    else
        return 0;

    /* for all */
    event->state = STATE_IDLE;
    event->mouseDownLeft = 0;
    event->mouseUpLeft = 0;
    event->mouseDownRight = 0;
    event->mouseUpRight = 0;

    return 1;
}


void ft_button_draw_all(s_button *buttons, s_event *event)
{
    int i;
    char tmp[1024];

    for (i = 0; i < BUTTON_LAST; ++i)
    {
        s_rect *pos = &buttons[i].pos;

        /* bg */
        if (ft_button_is_vector_in_rect(pos, &event->mousePosPxl))
            rectfill(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, buttons[i].colorBackgroundHover);
        else
            rectfill(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, buttons[i].colorBackgroundDefault);

        rect(g_page, pos->x, pos->y, pos->x+pos->width, pos->y+pos->height, makecol(128, 128, 128));

        /* text */
        textout_centre_ex(g_page, font, buttons[i].text, pos->x+pos->width/2, pos->y+pos->height/2-3, makecol(0, 0, 0), -1);

        /* selected */
        if (event->form == buttons[i].form)
            ft_allegro_line(g_page, event, pos->x+10, pos->y+35, pos->x+90, pos->y+35, makecol(128, 128, 128), FLAG_FAT_LINE);
    }

    /* current color components */
    sprintf(tmp, "Rouge : %d", event->color.r);
    textout_right_ex(g_page, font, tmp, 730, 65-3, makecol(0, 0, 0), -1);
    sprintf(tmp, "Vert : %d", event->color.g);
    textout_right_ex(g_page, font, tmp, 730, 95-3, makecol(0, 0, 0), -1);
    sprintf(tmp, "Bleu : %d", event->color.b);
    textout_right_ex(g_page, font, tmp, 730, 125-3, makecol(0, 0, 0), -1);
}
