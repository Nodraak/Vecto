
#include <stdio.h>
#include <stdlib.h>


#include "constantes.h"
#include "file.h"
#include "init.h"


int ft_file_goto_next_nb(char *str, int index)
{
    /* goto space */
    while (str[index] != '\0' && str[index] != ' ')
        index ++;

    if (str[index] == '\0')
        return index;

    /* skip space */
    index ++;

    return index;
}


void ft_file_load(s_form *forms[NB_FORM], char *path)
{
    FILE *f = NULL;
    int ret = 0, i_form = 0, i_tmp, i_point;
    char tmp[1024];

    /* open file */
    f = fopen(path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", path);
        exit(EXIT_FAILURE);
    }

    /* clear old forms */
    ft_init_form_reset(forms);

    /* for each line = for each form */
    i_form = 0;
    while (fgets(tmp, 1024-1, f) != NULL)
    {
        if (i_form >= NB_FORM)
        {
            printf("Error no space for another form %d %s\n", __LINE__, __FILE__);
            exit(EXIT_FAILURE);
        }

        i_tmp = 0;
        forms[i_form] = ft_init_form_alloc();

        /* get type of form */
        ret = sscanf(&tmp[i_tmp], "%d", (int*)&forms[i_form]->type);
        if (ret != 1)
        {
            printf("Error form.type %d %s\n", __LINE__, __FILE__);
            break;
        }

        i_tmp = ft_file_goto_next_nb(tmp, i_tmp);

        /* color */
        ret = sscanf(&tmp[i_tmp], "%d %d %d", &forms[i_form]->color.r, &forms[i_form]->color.g, &forms[i_form]->color.b);
        if (ret != 3)
        {
            printf("Error form.color %d %s\n", __LINE__, __FILE__);
            break;
        }

        i_tmp = ft_file_goto_next_nb(tmp, i_tmp);
        i_tmp = ft_file_goto_next_nb(tmp, i_tmp);
        i_tmp = ft_file_goto_next_nb(tmp, i_tmp);

        /* for each point of form */
        i_point = 0;
        for (;;)
        {
            ret = sscanf(&tmp[i_tmp], "%d %d", &forms[i_form]->point[i_point].x, &forms[i_form]->point[i_point].y);
            if (ret != 2) /* end of line (or unexpeted error, but osef) */
                break;

            i_point ++;

            i_tmp = ft_file_goto_next_nb(tmp, i_tmp);
            i_tmp = ft_file_goto_next_nb(tmp, i_tmp);

            if (tmp[i_tmp] == '\0')
                break;
        }

        /* save nb_point */
        if (i_point < 2)
            printf("Error form.point %d %s - %d\n", __LINE__, __FILE__, i_point);

        forms[i_form]->nb_point = i_point;

        i_form ++;
    }

    fclose(f);
}


void ft_file_save(s_form *forms[NB_FORM], char *path)
{
    FILE *f = NULL;
    int i = 0, j = 0;

    f = fopen(path, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", path);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < NB_FORM; ++i)
    {
        if (forms[i] != NULL)
        {
            fprintf(f, "%d", forms[i]->type);
            fprintf(f, " %d %d %d", forms[i]->color.r, forms[i]->color.g, forms[i]->color.b);

            for (j = 0; j < forms[i]->nb_point; ++j)
                fprintf(f, " %d %d", forms[i]->point[j].x, forms[i]->point[j].y);

            fprintf(f, "\n");
        }
    }

    fclose(f);
}

