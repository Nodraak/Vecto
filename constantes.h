#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <allegro.h>

#define MOUSE_LEFT          (1 << 0)
#define MOUSE_RIGHT         (1 << 1)

#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       600

#define FPS                 50
#define NB_FORM             100
#define NB_POINT            100

enum e_state
{
    E_STATE_IDLE,
    E_STATE_DRAWING
};

enum e_label
{
    E_LABEL_LOAD,
    E_LABEL_SAVE,
    E_LABEL_LINE,
    E_LABEL_POLYGON,
    E_LABEL_LAST
};

enum e_form
{
    E_FORM_LINE = 0,
    E_FORM_POLYGON = 1,
    E_FORM_NONE = 42
};

/* basic */

typedef struct      s_vector
{
    int             x;
    int             y;
}                   t_vector;

typedef struct      s_rect
{
    int             x;
    int             y;
    int             width;
    int             height;
}                   t_rect;

/* extended */

typedef struct      s_label
{
    t_rect          pos;
    char            text[1024];
    int             colorBackgroundDefault;
    int             colorBackgroundHover;
}                   t_label;


typedef struct      s_form
{
    enum e_form     type;
    int             nb_point;
    t_vector        point[NB_POINT];
}                   t_form;

typedef struct      s_event
{
    t_vector        mousePos;
    int             mouseLeft;
    int             mouseRight;
    /* todo : keyboard */

    enum e_state    state;
    enum e_form     form;
    t_form          current;
}                   t_event;


extern int global_quit;
extern BITMAP *page;

#endif /* CONSTANTES_H_INCLUDED */
