
/*

TODO

Base : (16/20)
- pouvoir construire un dessin comparable � l'exemple ci dessus (polygones color�s pleins)
- objets group�s hi�rarchiquement en "objets" (carreaux -> fen�tres -> fa�ade -> maison...)
- possibilit� de grouper/d�grouper, entrer/sortir des groupes
- possibilit� de s�lectionner et copier/couper/coller/supprimer et translater la s�lection
- pouvoir g�rer et modifier l'ordre d'affichage des groupes : passer un objet devant/derri�re un autre
- sauvegarde/chargement dans un fichier, format libre mais vectoriel, pas au niveau pixel :
  un dessin peut �tre sauv�, l'application termin�e et relanc�e,
   le dessin peut �tre recharg� et reste �ditable comme si on �tait rest� dans l'appli
- respecter les crit�res de programmation modulaire, pr�senter une conception claire et bien d�coup�e

Bonus :
Exemples : zoom et scroll, mode �dition des sommets, rotations, undo/redo, barre d'outils, lisser polygones, textes etc...

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

    if ((mouse_b & MOUSE_LEFT) && (old.mouseLeft == 0))
    {
        event->mouseLeft = 1;
        old.mouseLeft = 1;
    }
    if ((mouse_b & MOUSE_LEFT) == 0 && (old.mouseLeft == 1))
    {
        event->mouseLeft = 0;
        old.mouseLeft = 0;
    }

    if ((mouse_b & MOUSE_RIGHT) && (old.mouseRight == 0))
    {
        event->mouseRight = 1;
        old.mouseRight = 1;
    }
    if ((mouse_b & MOUSE_RIGHT) == 0 && (old.mouseRight == 1))
    {
        event->mouseRight = 0;
        old.mouseRight = 0;
    }

    if (key[KEY_ESC])
        g_quit = 1;

}


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

    while (!g_quit)
    {
        /* event */
        ft_event_update(&event);

        /* calc */
        if (event.mouseLeft)
        {
            event.mouseLeft = 0;
            ft_calc_on_mouseLeft(&event, forms, buttons);
        }

        if (event.mouseRight)
        {
            event.mouseRight = 0;
            ft_calc_on_mouseRight(&event, forms);
        }

        ft_calc_update_button_color(buttons, &event.color);

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

    return 0;
}
END_OF_MAIN()
