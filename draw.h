#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED


void ft_draw_button(t_label *button, t_vector mousePos);
int ft_is_mouse_in_rect(t_rect *rect, t_vector mousePos);
void ft_draw_all(t_event *event, t_form *form, t_label *label);

#endif /* DRAW_H_INCLUDED */
