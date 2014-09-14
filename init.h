
#ifndef INIT_H
#define INIT_H

void close_button_handler(void);
void ft_init_allegro(void);
void ft_init_buttons(s_button *buttons);
void ft_init_event(s_event *event);

s_form *ft_init_form_new(void);
void ft_init_form_reset(s_form **forms);

#endif /* INIT_H */
