#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

int ft_is_mouse_in_rect(s_rect *rect, s_vector mousePos);
void ft_scale_coord(double zoom, s_vector offset, int *x1, int *y1, int *x2, int *y2);

void my_line(BITMAP *bmp, s_event *event, int x1, int y1, int x2, int y2, int c, int flags);
void my_rect(BITMAP *bmp, s_event *event, int x1, int y1, int x2, int y2, int c);

void ft_draw_button(s_button *button, s_event *event, int selected);
void ft_draw_line(s_form *form, s_event *event);
void ft_draw_polygon(s_form *form, s_event *event);
void ft_draw_all(s_event *event, s_form *forms[NB_FORM], s_button *buttons);

#endif /* DRAW_H_INCLUDED */
