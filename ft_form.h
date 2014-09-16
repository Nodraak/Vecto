#ifndef FT_FORM_H_INCLUDED
#define FT_FORM_H_INCLUDED


s_form *ft_form_alloc(void);
s_vector ft_form_get_center(s_form *form);

void ft_form_draw_all(s_form *forms[NB_FORM], s_event *event);
void ft_form_draw_hovered(s_event *event, s_form *forms[NB_FORM]);

void ft_form_draw_line(s_form *form, s_event *event);
void ft_form_draw_polygon(s_form *form, s_event *event);


#endif /* FT_FORM_H_INCLUDED */
