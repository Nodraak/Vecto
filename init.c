
#include <stdlib.h>
#include <stdio.h>

#include "constantes.h"
#include "init.h"

void close_button_handler(void)
{
    g_quit = 1;
}


void ft_init_allegro(void)
{
    if (allegro_init() != 0)
        exit(EXIT_FAILURE);

    set_color_depth(32);
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) != 0)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Unable to set any graphic mode - %s\n", allegro_error);
        exit(EXIT_FAILURE);
    }

    install_keyboard();
    install_mouse();

    set_close_button_callback(close_button_handler);
    show_mouse(screen);

    g_page = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (g_page == NULL)
    {
        fprintf(stderr, "Error create tmp screen.\n");
        exit(EXIT_FAILURE);
    }
    g_page_tmp = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (g_page_tmp == NULL)
    {
        fprintf(stderr, "Error create tmp screen.\n");
        exit(EXIT_FAILURE);
    }
}


void ft_init_buttons(s_button *buttons)
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


void ft_init_event(s_event *event)
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


s_form *ft_init_form_alloc(void)
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


void ft_init_form_reset(s_form **forms)
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

