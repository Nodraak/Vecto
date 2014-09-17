#ifndef FT_BUTTON_H_INCLUDED
#define FT_BUTTON_H_INCLUDED

int ft_button_is_vector_in_rect(s_rect *rect, s_vector *vect);

void ft_button_load(s_button *buttons);
int ft_button_update(s_drawing *drawing, s_event *event, s_button *buttons);
void ft_button_update_color(s_button *buttons, s_color *color);

void ft_button_draw_all(s_drawing *drawing, s_event *event, s_button *buttons);


#endif /* FT_BUTTON_H_INCLUDED */
