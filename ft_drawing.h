#ifndef FT_DRAWING_H_INCLUDED
#define FT_DRAWING_H_INCLUDED

void ft_drawing_init(s_drawing *drawing);
void ft_drawing_reset_forms(s_drawing *drawing);

int ft_file_goto_next_nb(char *str, int index);
void ft_drawing_load(s_drawing *drawing);
void ft_drawing_save(s_drawing *drawing);

void ft_drawing_add_form(s_form *forms[NB_FORM], s_form *new);
int ft_drawing_get_closer_center(s_form *forms[NB_FORM], s_vector *mousePosCoord, double zoom);
s_vector *ft_drawing_get_closer_point(s_form *forms[NB_FORM], s_vector *mousePosCoord, double zoom);

void ft_drawing_update_coord(s_drawing *drawing, s_event *event);

void ft_drawing_draw_tmp_form(s_drawing *drawing, s_event *event);


#endif /* FT_DRAWING_H_INCLUDED */
