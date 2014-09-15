#ifndef CALC_H
#define CALC_H

void ft_swap(s_form **ptr1, s_form **ptr2);

void ft_calc_all(s_event *event, s_form *forms[NB_FORM], s_button *buttons);

void ft_calc_on_mouseDownLeft(s_event *event, s_form *forms[NB_FORM], s_button *buttons);
void ft_calc_on_mouseDownRight(s_event *event, s_form *forms[NB_FORM]);

void ft_calc_update_button_color(s_button *buttons, s_color *color);
void ft_calc_update_closer_point_or_barycenter(s_event *event, s_form *forms[NB_FORM]);
void ft_calc_update_all_barycenter(s_form *forms[NB_FORM]);

void ft_calc_get_closer_point(s_event *event, s_form *forms[NB_FORM]);
void ft_calc_get_closer_barycenter(s_event *event, s_form *forms[NB_FORM]);
s_vector ft_calc_barycenter(s_vector *points, int nb_point);

void ft_calc_add_form(s_form *forms[NB_FORM], s_form *new);

#endif /* CALC_H_INCLUDED */
