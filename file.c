
#include <stdio.h>
#include <stdlib.h>


#include "constantes.h"
#include "file.h"



void ft_file_load(t_form form[NB_FORM], char *path)
{
    FILE *f = NULL;
    int ret = 0, i_form = 0, i_tmp, i_point;
    char tmp[1024];

    f = fopen(path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", path);
        exit(EXIT_FAILURE);
    }

    while (fgets(tmp, 1024-1, f) != NULL)
    {
        ret = sscanf(tmp, "%d", (int*)&form[i_form].type);
        if (ret != 1)
            break;

        i_tmp = 0;
        i_point = 0;
        do {
            while (tmp[i_tmp] != ' ')
                i_tmp ++;
            i_tmp ++;

            ret = sscanf(&tmp[i_tmp], "%d %d", &form[i_form].point[i_point].x, &form[i_form].point[i_point].x);
        } while (ret == 2);

        form[i_form].used = 1;
        i_form ++;
    }

    for (; i_form < NB_FORM; ++i_form)
        form[i_form].used = 0;

    fclose(f);
}


void ft_file_save(t_form form[NB_FORM], char *path)
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
        if (form[i].used)
        {
            fprintf(f, "%d ", form[i].type);

            for (j = 0; j < form[i].nb_point; ++j)
                fprintf(f, "%d %d ", form[i].point[j].x, form[i].point[j].y);

            fprintf(f, "\n");
        }
    }

    fclose(f);
}

