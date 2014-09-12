#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED


void ft_draw_button(s_button *button, s_vector mousePos, int selected);
int ft_is_mouse_in_rect(s_rect *rect, s_vector mousePos);
void ft_draw_all(s_event *event, s_form *form, s_button *buttons);

#endif /* DRAW_H_INCLUDED */
