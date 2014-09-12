
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
}


void ft_init_buttons(s_button *buttons)
{
    buttons[BUTTON_LOAD].pos.x = 0;
    buttons[BUTTON_LOAD].pos.y = 0;
    buttons[BUTTON_LOAD].pos.width = 100;
    buttons[BUTTON_LOAD].pos.height = 50;
    buttons[BUTTON_LOAD].colorBackgroundDefault = makecol(50, 50, 255);
    buttons[BUTTON_LOAD].colorBackgroundHover = makecol(80, 80, 255);
    buttons[BUTTON_LOAD].form = FORM_NONE;
    sprintf(buttons[BUTTON_LOAD].text, "Charger");

    buttons[BUTTON_SAVE].pos.x = 100;
    buttons[BUTTON_SAVE].pos.y = 0;
    buttons[BUTTON_SAVE].pos.width = 100;
    buttons[BUTTON_SAVE].pos.height = 50;
    buttons[BUTTON_SAVE].colorBackgroundDefault = makecol(50, 50, 255);
    buttons[BUTTON_SAVE].colorBackgroundHover = makecol(80, 80, 255);
    buttons[BUTTON_SAVE].form = FORM_NONE;
    sprintf(buttons[BUTTON_SAVE].text, "Sauver");

    buttons[BUTTON_LINE].pos.x = 600;
    buttons[BUTTON_LINE].pos.y = 0;
    buttons[BUTTON_LINE].pos.width = 100;
    buttons[BUTTON_LINE].pos.height = 50;
    buttons[BUTTON_LINE].colorBackgroundDefault = makecol(50, 50, 255);
    buttons[BUTTON_LINE].colorBackgroundHover = makecol(80, 80, 255);
    buttons[BUTTON_LINE].form = FORM_LINE;
    sprintf(buttons[BUTTON_LINE].text, "Ligne");

    buttons[BUTTON_POLYGON].pos.x = 700;
    buttons[BUTTON_POLYGON].pos.y = 0;
    buttons[BUTTON_POLYGON].pos.width = 100;
    buttons[BUTTON_POLYGON].pos.height = 50;
    buttons[BUTTON_POLYGON].colorBackgroundDefault = makecol(50, 50, 255);
    buttons[BUTTON_POLYGON].colorBackgroundHover = makecol(80, 80, 255);
    buttons[BUTTON_POLYGON].form = FORM_POLYGON;
    sprintf(buttons[BUTTON_POLYGON].text, "Polygone");
}

