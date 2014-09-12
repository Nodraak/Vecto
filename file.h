#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED


int ft_file_goto_next_nb(char *str, int index);
void ft_file_load(t_form form[NB_FORM], char *path);
void ft_file_save(t_form form[NB_FORM], char *path);


#endif /* FILE_H_INCLUDED */
