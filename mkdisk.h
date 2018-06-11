#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

typedef struct mkdisk
{
    int sizeDisk;
    char path[80];
    char name[25];
    char unit[5];
} mk_init;

typedef struct param
{
    char name[10];
    char value[50];
} pMK;

int mkContructor(int state, char str[], int cont)
{
    mk_init mk;
    memset(&mk.name,'\0',sizeof(mk.name));
    memset(&mk.path,'\0',sizeof(mk.path));
    memset(&mk.unit,'\0',sizeof(mk.unit));
    strcat(mk.unit, "M");
    // Base Program
    char ch, buffer[45];
    memset(&buffer,'\0',sizeof(buffer));
    int j=0;
    bool read = true;
    char name[10];
    memset(&name,'\0',sizeof(name));
    while((ch = str[cont++]) != '\0')
    {
        if(ch == '-' && read)
        {
            buffer[j] = '\0';
            memset(&name,'\0',sizeof(name));
            strcat(name, buffer);
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }

        if(ch == '\"')
        {
            read = !read;
        }

        if(isalnum(ch) || ch == '/' || ch == '.')
        {
            buffer[j++] = ch;
        }

        if(ch == ' ' && !read)
        {
            buffer[j++] = ch;
        }

        else if((ch == ' ' || ch == '\n') && (j != 0) && read)
        {
            buffer[j] = '\0';
            // Posible optimizacion
            if(strcasecmp("SIZE", name) == 0)
            {
                mk.sizeDisk = atoi(buffer);
            }
            else if(strcasecmp("PATH", name) == 0)
            {
                strcat(mk.path, buffer);
            }
            else if(strcasecmp("NAME", name) == 0)
            {
                strcat(mk.name, buffer);
            }
            else if(strcasecmp("UNIT", name) == 0)
            {
                memset(&mk.unit,'\0',sizeof(mk.unit));
                strcat(mk.unit, buffer);
            }
            else
            {
                // Errores
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
            free(buffer);
        }
    }
    printf("%s\n", mk.name);
    printf("%s\n", mk.path);
    printf("%d\n", mk.sizeDisk);
    printf("%s\n", mk.unit);
    return 0;
}
