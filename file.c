
#include <stdio.h>
#include <stdlib.h>


#include "constantes.h"
#include "file.h"



void ft_file_load(t_form form[NB_FORM], char *path)
{
    FILE *f = NULL;
    int ret = 0, i = 0;
    char tmp[1024];

    f = fopen(path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", path);
        exit(EXIT_FAILURE);

    }

    while (fgets(tmp, 1024-1, f) != NULL)
    {
        ret = sscanf(tmp, "%d %d %d %d", &form[i].p1.x, &form[i].p1.y, &form[i].p2.x, &form[i].p2.y);

        if (ret != 4)
            break;

        form[i].used = 1;
        i++;
    }

    fclose(f);
}


void ft_file_save(t_form form[NB_FORM], char *path)
{
    FILE *f = NULL;
    int i = 0;


    f = fopen(path, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", path);
        exit(EXIT_FAILURE);

    }

    for (i = 0; i < NB_FORM; ++i)
    {
        if (form[i].used)
            fprintf(f, "%d %d %d %d\n", form[i].p1.x, form[i].p1.y, form[i].p2.x, form[i].p2.y);
    }

    fclose(f);
}

