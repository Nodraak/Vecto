#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED



#define MOUSE_LEFT  (1 << 0)
#define MOUSE_RIGHT (1 << 1)


#define NB_FORM     1000

enum e_state
{
    E_STATE_WAIT,
    E_STATE_DRAW_WAIT
};


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

typedef struct      s_form
{
    int             used;
    t_vector        p1;
    t_vector        p2;
}                   t_form;

typedef struct      s_event
{
    t_vector        mousePos;
    int             mouseLeft;
    int             mouseRight;
    enum e_state    state;
    /* todo : keyboard */
}                   t_event;


/* croix rouge de la fenetre */
extern int global_quit;

#endif /* CONSTANTES_H_INCLUDED */
