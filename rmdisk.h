#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include <dirent.h>
#include <errno.h>

typedef struct rmdisk
{
    char path[80];
} rm_init;

int rmContructor(int state, char str[], int cont)
{
    if(cont == -1){
        printf("    - Comando sin parametros.\n\n");
        return 0;
    }
    rm_init rm;
    memset(&rm.path,'\0',sizeof(rm.path));
    // Base Program
    char ch, buffer[100];
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

        if(ch == '\"' || ch == '\”')
        {
            read = !read;
        }

        if(isalnum(ch) || ch == '/' || ch == '.' || ch == '_')
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
            if(strcasecmp("PATH", name) == 0)
            {
                strcat(rm.path, buffer);
            }
            else
            {
                // Errores
                printf("    - Comando inexistente\n\n");
                return 0;
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    }
    if((j != 0) && read)
        {
            buffer[j] = '\0';
            // Posible optimizacion
            if(strcasecmp("PATH", name) == 0)
            {
                strcat(rm.path, buffer);
            }
            else
            {
                // Errores
                printf("    - Comando inexistente\n\n");
                return 0;
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    // printf("%s\n", rm.path);
    // /home/mis discos/DISCO Prueba
    // rmDisk &path->"/home/mis discos/DISCO Prueba/Disco_3.dsk"
    char* comando = (char*)malloc(500);

    memset(&comando[0], 0, sizeof(comando));
    strcat(comando, "sudo rm -i \"");
    strcat(comando, rm.path);
    strcat(comando, "\"");
    system(comando); // ejecuta el comando en la terminal del SO
    free(comando);
    printf("\n");
    return 0;
}
