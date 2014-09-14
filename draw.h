#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

int ft_is_mouse_in_rect(s_rect *rect, s_vector mousePos);
void my_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int c);

void ft_draw_button(s_button *button, s_vector mousePos, int selected);
void ft_draw_line(s_form *form);
void ft_draw_polygon(s_form *form);
void ft_draw_all(s_event *event, s_form *forms[NB_FORM], s_button *buttons);

#endif /* DRAW_H_INCLUDED */
