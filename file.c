
#include <stdio.h>
#include <stdlib.h>


#include "constantes.h"
#include "file.h"



int ft_file_goto_next_nb(char *str, int index)
{
    /* goto space */
    while (str[index] != '\0' && str[index] != ' ')
        index ++;
    /* skip space */
    index ++;

    return index;
}


void ft_file_load(s_form forms[NB_FORM], char *path)
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

    /* for each line = for each form */
    while (fgets(tmp, 1024-1, f) != NULL)
    {
        i_tmp = 0;

        /* get tipy of form */
        ret = sscanf(tmp, "%d", (int*)&forms[i_form].type);
        if (ret != 1)
        {
            printf("Error %d %s\n", __LINE__, __FILE__);
            break;
        }

        i_tmp = ft_file_goto_next_nb(tmp, i_tmp);

        /* for each data of form */
        i_point = 0;
        for (;;)
        {
            ret = sscanf(&tmp[i_tmp], "%d %d", &forms[i_form].point[i_point].x, &forms[i_form].point[i_point].y);
            if (ret != 2) /* end of line (or unexpeted error, but osef) */
                break;

            i_tmp = ft_file_goto_next_nb(tmp, i_tmp);
            i_tmp = ft_file_goto_next_nb(tmp, i_tmp);

            i_point ++;
        }

        /* save form type */
        if (i_point == 2)
            forms[i_form].type = FORM_LINE;
        else if (i_point > 2)
            forms[i_form].type = FORM_POLYGON;
        else
            printf("Error %d %s - %d\n", __LINE__, __FILE__, i_point);

        forms[i_form].nb_point = i_point;

        i_form ++;
    }

    /* set other free slot to unused */
    for (; i_form < NB_FORM; ++i_form)
        forms[i_form].type = FORM_NONE;

    fclose(f);
}


void ft_file_save(s_form forms[NB_FORM], char *path)
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
        if (forms[i].type != FORM_NONE)
        {
            fprintf(f, "%d ", forms[i].type);

            for (j = 0; j < forms[i].nb_point; ++j)
                fprintf(f, "%d %d ", forms[i].point[j].x, forms[i].point[j].y);

            fprintf(f, "\n");
        }
    }

    fclose(f);
}

