
/*

S : suprimer
A : avancer (Q en qwerty)
R : reculer
D : duppliquer
P : zoom plus
M : zoom moins
Q : quitter (A en qwerty)
ESC : abandonenr la forme en cours

tab / list de GROUP
GROUP = tab / list de FORM

->  G1    -    G2    -    G3
    |          |          |
    F1 - F2    F1 - F2    F1 - F2


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
#include "ft_drawing.h"
#include "ft_form.h"
#include "ft_event.h"
#include "ft_button.h"
#include "ft_allegro.h"


int g_quit = 0;
BITMAP *g_page = NULL;
BITMAP *g_page_tmp = NULL;



int _mangled_main(void);
int __attribute__((__stdcall__)) WinMain(void *hInst, void *hPrev, char *Cmd, int nShow);


int main(void)
{
    s_form *forms[NB_FORM] = {0};
    s_event event;
    s_button buttons[BUTTON_LAST];

    ft_allegro_init();
    ft_button_load(buttons);
    ft_drawing_reset_forms(forms);
    ft_event_init(&event);

    while (!g_quit)
    {
        /* event */
        ft_event_update(&event);

        /* calc */
        if (event.mouseDownLeft)
            ft_button_update(&event, forms, buttons);
        ft_event_handle(&event, forms);
        ft_drawing_update_coord(&event);
        ft_button_update_color(buttons, &event.color);

        /* draw */
        clear_to_color(g_page, makecol(255, 255, 255));

        ft_form_draw_all(forms, &event);
        ft_drawing_draw_tmp_form(&event);
        ft_form_draw_hovered(&event, forms);
        ft_button_draw_all(buttons, &event);

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

