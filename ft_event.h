#ifndef FT_EVENT_H_INCLUDED
#define FT_EVENT_H_INCLUDED


void ft_event_init(s_event *event);
void ft_event_update(s_drawing *drawing, s_event *event);

void ft_swap(s_form **ptr1, s_form **ptr2);
int ft_event_handle(s_drawing *drawing, s_event *event);

#endif /* FT_EVENT_H_INCLUDED */
