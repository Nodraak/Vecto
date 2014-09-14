
/*

tab / list de GROUP
GROUP = tab / list de FORM

->  G1    -    G2    -    G3
    |          |          |
    F1 - F2    F1 - F2    F1 - F2


TODO

Base : (16/20)
- pouvoir construire un dessin comparable à l'exemple ci dessus (polygones colorés pleins)
- objets groupés hiérarchiquement en "objets" (carreaux -> fenêtres -> façade -> maison...)
- possibilité de grouper/dégrouper, entrer/sortir des groupes
- possibilité de sélectionner et copier/couper/coller/supprimer et translater la sélection
- pouvoir gérer et modifier l'ordre d'affichage des groupes : passer un objet devant/derrière un autre
- sauvegarde/chargement dans un fichier, format libre mais vectoriel, pas au niveau pixel :
  un dessin peut être sauvé, l'application terminée et relancée,
   le dessin peut être rechargé et reste éditable comme si on était resté dans l'appli
- respecter les critères de programmation modulaire, présenter une conception claire et bien découpée

Bonus :
Exemples : zoom et scroll, mode édition des sommets, rotations, undo/redo, barre d'outils, lisser polygones, textes etc...

Doc -> http://alleg.sourceforge.net/stabledocs/en/allegro.html

*/


#include <stdio.h>
#include <stdlib.h>

#include <allegro.h>

#include "constantes.h"
#include "file.h"
#include "draw.h"
#include "calc.h"
#include "init.h"


int g_quit = 0;
BITMAP *g_page = NULL;
BITMAP *g_page_tmp = NULL;


void ft_event_update(s_event *event);

void ft_event_update(s_event *event)
{
    static s_event old;

    event->mousePos.x = mouse_x;
    event->mousePos.y = mouse_y;

    event->mouseRel.x = mouse_x - old.mousePos.x;
    event->mouseRel.y = mouse_y - old.mousePos.y;

    if ((mouse_b & MOUSE_LEFT) && (old.mouseDownLeft == 0))
    {
        event->mouseDownLeft = 1;
        event->mouseUpLeft = 0;
        old.mouseDownLeft = 1;
    }
    if ((mouse_b & MOUSE_LEFT) == 0 && (old.mouseDownLeft == 1))
    {
        event->mouseDownLeft = 0;
        event->mouseUpLeft = 1;
        old.mouseDownLeft = 0;
    }

    if ((mouse_b & MOUSE_RIGHT) && (old.mouseDownRight == 0))
    {
        event->mouseDownRight = 1;
        event->mouseUpRight = 0;
        old.mouseDownRight = 1;
    }
    if ((mouse_b & MOUSE_RIGHT) == 0 && (old.mouseDownRight == 1))
    {
        event->mouseDownRight = 0;
        event->mouseUpRight = 1;
        old.mouseDownRight = 0;
    }

    if (key[KEY_ESC])
        g_quit = 1;

    old.mousePos.x = mouse_x;
    old.mousePos.y = mouse_y;
}


int _mangled_main(void);
int __attribute__((__stdcall__)) WinMain(void *hInst, void *hPrev, char *Cmd, int nShow);


int main(void)
{
    s_form forms[NB_FORM];
    s_event event;
    s_button buttons[BUTTON_LAST];

    ft_init_allegro();
    ft_init_buttons(buttons);
    ft_init_form_reset(forms);

    event.state = STATE_IDLE;
    event.form = FORM_LINE;
    event.color.r = 120;
    event.color.g = 120;
    event.color.b = 120;
    event.pointHovered = NULL;

    while (!g_quit)
    {
        /* event */
        ft_event_update(&event);

        /* calc */
        if (event.mouseDownLeft)
        {
            event.mouseDownLeft = 0;
            ft_calc_on_mouseDownLeft(&event, forms, buttons);
        }

        if (event.mouseDownRight)
        {
            event.mouseDownRight = 0;
            ft_calc_on_mouseDownRight(&event, forms);
        }

        if (event.mouseUpLeft)
        {
            event.mouseUpLeft = 0;
            if (event.form == FORM_EDIT)
                event.state = STATE_IDLE;
        }

        if (event.mouseUpRight)
        {
            event.mouseUpRight = 0;
        }

        ft_calc_update_button_color(buttons, &event.color);
        ft_calc_update_closer_point(&event, forms);

        /* draw */
        ft_draw_all(&event, forms, buttons);
        acquire_screen();
        blit(g_page, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        release_screen();

        rest(1000/FPS);
    }

    destroy_bitmap(g_page);
    destroy_bitmap(g_page_tmp);
    allegro_exit();

    return EXIT_SUCCESS;
}
END_OF_MAIN()

