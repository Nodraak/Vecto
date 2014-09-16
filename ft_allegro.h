#ifndef FT_ALLEGRO_H_INCLUDED
#define FT_ALLEGRO_H_INCLUDED

void ft_allegro_scale_coord(double zoom, s_vector offset, int *x1, int *y1, int *x2, int *y2);
void close_button_handler(void);

void ft_allegro_init(void);

void ft_allegro_line(BITMAP *bmp, s_event *event, int x1, int y1, int x2, int y2, int c, int flags);
void ft_allegro_rect(BITMAP *bmp, s_event *event, int x1, int y1, int x2, int y2, int c);

#endif /* FT_ALLEGRO_H_INCLUDED */
