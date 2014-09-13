#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <allegro.h>

#define MOUSE_LEFT          (1 << 0)
#define MOUSE_RIGHT         (1 << 1)

#define FPS                 50
#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       600

#define NB_FORM             100
#define NB_POINT            100
#define COLOR_STEP          15

typedef enum    _e_state
{
    STATE_IDLE,
    STATE_DRAWING
}               e_state;

typedef enum    _e_button
{
    BUTTON_LOAD,
    BUTTON_SAVE,

    BUTTON_LINE,
    BUTTON_POLYGON,

    BUTTON_R_MINUS,
    BUTTON_R_PLUS,
    BUTTON_G_MINUS,
    BUTTON_G_PLUS,
    BUTTON_B_MINUS,
    BUTTON_B_PLUS,

    BUTTON_LAST
}               e_button;

typedef enum    _e_form
{
    FORM_NONE = 0,
    FORM_LINE = 1,
    FORM_POLYGON = 2
}               e_form;

/* basic */

typedef struct      _s_vector
{
    int             x;
    int             y;
}                   s_vector;

typedef struct      _s_rect
{
    int             x;
    int             y;
    int             width;
    int             height;
}                   s_rect;

typedef struct      _s_color
{
    int             r;
    int             g;
    int             b;
}                   s_color;

/* extended */

typedef struct      _s_button
{
    s_rect          pos;
    char            text[1024];
    int             colorBackgroundDefault;
    int             colorBackgroundHover;
    e_form          form;
}                   s_button;


typedef struct      _s_form
{
    e_form          type;
    int             nb_point;
    s_vector        point[NB_POINT];
    s_color         color;
}                   s_form;

typedef struct      _s_event
{
    s_vector        mousePos;
    int             mouseLeft;
    int             mouseRight;
    /* todo : keyboard */

    e_state         state;
    e_form          form;
    s_form          current;
    s_color         color;
}                   s_event;


extern int g_quit;
extern BITMAP *g_page;
extern BITMAP *g_page_tmp;

#endif /* CONSTANTES_H_INCLUDED */
