


#include <stdio.h>
#include <stdlib.h>

#include <allegro.h>

#define MOUSE_LEFT (1 << 0)
#define MOUSE_RIGHT (1 << 1)


enum e_state
{
    E_STATE_WAIT,
    E_STATE_DRAW_WAIT,
};


int main(void)
{
    int quit = 0, screenRefresh = 1;
    enum e_state state = E_STATE_WAIT;

    if (allegro_init() != 0)
        return 1;

    set_color_depth(32);
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0) != 0)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Unable to set any graphic mode - %s\n", allegro_error);
        return 1;
    }

    install_keyboard();
    int nb = install_mouse();

    printf("%d\n", nb);

    while (!quit)
    {

        // event


        // calcssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEAz7mICuGpBlw4B6aqOpl7fK71tzltsCPd5EJbhkhqdIyUJU1g1/Gaz6X0CCi99yuhi8FzvMp6FXEnoJ7jiUIq08eVkjldPtmLGUaSzRWDqNz2S/DZ1EeObrE/Ffaow/qnl+TPh3gCMHExslObrNKT8C8x1Ewd25NEIgBQ5ZhZPVGp9xxMwdytbiE0UquN6jI3z6zOYA/lsj3w2sLkGNYp7mBp1hJC/Sw9B6ZpVvDMtgTw6uF7ofI9Yx0+feuDPRod2gOxZ0hphu8cAJkRTWmBpAk5aV29bwbFsgzSaW2YCP8WW0++HJOiIo7thwwkyE7dxsRV6XbuoVEMgZe4/dSd5w== adrienchardon@mailoo.org

        if (mouse_b & MOUSE_LEFT)
        {
            if (state == E_STATE_WAIT)
            {
                // draw start

                state = E_STATE_DRAW_WAIT;
            }
            else if (state == E_STATE_DRAW_WAIT)
            {
                // draw end

                state = E_STATE_WAIT;
            }
        }

        if (mouse_b & MOUSE_LEFT)
        {
            state = E_STATE_WAIT;
        }




        // draw
        if (screenRefresh)
        {
            clear_to_color(screen, makecol(255, 255, 255));

            acquire_screen();
            line(screen, 0, 0, 200, 200, makecol(255, 0, 0));
            textout_centre_ex(screen, font, "Hello world !", SCREEN_W/2, SCREEN_H/2, makecol(0, 0, 0), -1);
            release_screen();

            screenRefresh = 0;
        }

        // if (readkey())
            //quit = 1;
    }

    // readkey();








    return 0;
}
END_OF_MAIN()
