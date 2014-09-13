#ifndef CALC_H
#define CALC_H

void ft_calc_on_mouseLeft(s_event *event, s_form *forms, s_button *buttons);
void ft_calc_on_mouseRight(s_event *event, s_form *forms);

int ft_is_button_for_color(e_button button);
void ft_calc_update_button_color(s_button *buttons, s_color *color);
void ft_calc_update_closer_point(s_event *event, s_form *forms);

#endif /* CALC_H_INCLUDED */
