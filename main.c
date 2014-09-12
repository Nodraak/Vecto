
/*

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


BITMAP *page = NULL;

/* croix rouge de la fenetre */
int global_quit = 0;
void close_button_handler(void);
void close_button_handler(void) { global_quit = 1; }


void ft_init_allegro(void);

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

    page = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (page == NULL)
    {
        fprintf(stderr, "Error create tmp screen.\n");
        exit(EXIT_FAILURE);
    }
}


void ft_event_update(t_event *event);

void ft_event_update(t_event *event)
{
    static t_event old = {{0}};

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
}


void ft_calc_on_mouseLeft(t_event *event, t_form *form, t_label *label);

void ft_calc_on_mouseLeft(t_event *event, t_form *form, t_label *label)
{
    event->mouseLeft = 0;

    /* menu */
    if (ft_is_mouse_in_rect(&label[E_LABEL_LOAD].pos, event->mousePos))
        ft_file_load(form, "data.vecto");
    else if (ft_is_mouse_in_rect(&label[E_LABEL_SAVE].pos, event->mousePos))
        ft_file_save(form, "data.vecto");
    /* form */
    else if (ft_is_mouse_in_rect(&label[E_LABEL_LINE].pos, event->mousePos))
        event->form = E_FORM_LINE;
    else if (ft_is_mouse_in_rect(&label[E_LABEL_POLYGON].pos, event->mousePos))
        event->form = E_FORM_POLYGON;
    /* draw start / continue / end */
    else if (event->state == E_STATE_IDLE)
    {
        /* draw start */
        event->current.nb_point = 1;
        event->current.point[0].x = mouse_x;
        event->current.point[0].y = mouse_y;

        event->state = E_STATE_DRAWING;
    }
    else if (event->state == E_STATE_DRAWING)
    {
        /* draw continue / end */
        if (event->form == E_FORM_LINE)
        {
            int i = 0;
            while (form[i].type != E_FORM_NONE)
                i++;

            form[i].point[0].x = event->current.point[0].x;
            form[i].point[0].y = event->current.point[0].y;
            form[i].point[1].x = mouse_x;
            form[i].point[1].y = mouse_y;
            form[i].nb_point = 2;
            form[i].type = E_FORM_LINE;

            event->state = E_STATE_IDLE;
        }
        else if (event->form == E_FORM_POLYGON)
        {
            event->current.point[event->current.nb_point].x = mouse_x;
            event->current.point[event->current.nb_point].y = mouse_y;
            event->current.nb_point ++;
        }
        else
            printf("Error %d %s\n", __LINE__, __FILE__);

    }
}


int main(void)
{
    int i;
    t_form form[NB_FORM];
    t_event event;

    for (i = 0; i < NB_FORM; ++i)
        form[i].type = E_FORM_NONE;

    event.state = E_STATE_IDLE;
    event.form = E_FORM_LINE;

    t_label label[E_LABEL_LAST];

    ft_init_allegro();

label[E_LABEL_LOAD].pos.x = 0;
label[E_LABEL_LOAD].pos.y = 0;
label[E_LABEL_LOAD].pos.width = 100;
label[E_LABEL_LOAD].pos.height = 50;
label[E_LABEL_LOAD].colorBackgroundDefault = makecol(50, 50, 255);
label[E_LABEL_LOAD].colorBackgroundHover = makecol(80, 80, 255);
sprintf(label[E_LABEL_LOAD].text, "Charger");

label[E_LABEL_SAVE].pos.x = 100;
label[E_LABEL_SAVE].pos.y = 0;
label[E_LABEL_SAVE].pos.width = 100;
label[E_LABEL_SAVE].pos.height = 50;
label[E_LABEL_SAVE].colorBackgroundDefault = makecol(50, 50, 255);
label[E_LABEL_SAVE].colorBackgroundHover = makecol(80, 80, 255);
sprintf(label[E_LABEL_SAVE].text, "Sauver");

label[E_LABEL_LINE].pos.x = 600;
label[E_LABEL_LINE].pos.y = 0;
label[E_LABEL_LINE].pos.width = 100;
label[E_LABEL_LINE].pos.height = 50;
label[E_LABEL_LINE].colorBackgroundDefault = makecol(50, 50, 255);
label[E_LABEL_LINE].colorBackgroundHover = makecol(80, 80, 255);
sprintf(label[E_LABEL_LINE].text, "Ligne");

label[E_LABEL_POLYGON].pos.x = 700;
label[E_LABEL_POLYGON].pos.y = 0;
label[E_LABEL_POLYGON].pos.width = 100;
label[E_LABEL_POLYGON].pos.height = 50;
label[E_LABEL_POLYGON].colorBackgroundDefault = makecol(50, 50, 255);
label[E_LABEL_POLYGON].colorBackgroundHover = makecol(80, 80, 255);
sprintf(label[E_LABEL_POLYGON].text, "Polygone");




    while (!global_quit)
    {
        /* event */
        ft_event_update(&event);

        if (key[KEY_ESC])
            global_quit = 1;

        /* calc */
        if (event.mouseLeft)
            ft_calc_on_mouseLeft(&event, form, label);

        if (event.mouseRight)
        {
            event.mouseRight = 0;

            if (event.form == E_FORM_LINE)
            {

            }
            else if (event.form == E_FORM_POLYGON)
            {
                int i = 0, j;
                while (form[i].type != E_FORM_NONE)
                    i++;

                for (j = 0; j < event.current.nb_point; ++j)
                {
                    form[i].point[j].x = event.current.point[j].x;
                    form[i].point[j].y = event.current.point[j].y;
                }

                form[i].point[event.current.nb_point].x = mouse_x;
                form[i].point[event.current.nb_point].y = mouse_y;
                form[i].nb_point = event.current.nb_point+1;

                form[i].type = E_FORM_POLYGON;

                event.state = E_STATE_IDLE;
            }

        }

        /* draw */
        ft_draw_all(&event, form, label);
        acquire_screen();
        blit(page, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        release_screen();

        rest(1000/FPS);
    }

    destroy_bitmap(page);
    allegro_exit();

    return 0;
}
END_OF_MAIN()
