#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include <dirent.h>
#include <errno.h>

typedef struct unmount
{
    char id[15];
} um_init;

int valExistu(PD data[], char val[]){
    for(int i=0;i<100;i++){
        if(strcasecmp(data[i].id,val) == 0){
            return i;
        }
    }
    return -1;
}

int umContructor(int state, char str[], int cont)
{
    if(cont == -1){
        printf("    - Comando sin parametros.\n\n");
        return 0;
    }
    um_init rm;
    memset(&rm.id,'\0',sizeof(rm.id));
    // Base Program
    char ch, buffer[100];
    memset(&buffer,'\0',sizeof(buffer));
    int j=0;
    bool read = true;
    bool num = false;
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
            num = !num;
            memset(&buffer,'\0',sizeof(buffer));
        }

        if(ch == '\"' || ch == '\”')
        {
            read = !read;
        }
        if(isdigit(ch) && num){
            buffer[j++] = ch;
        }

        if(isalpha(ch) || ch == '/' || ch == '.' || ch == '_')
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
            if(strcasecmp("ID", name) == 0)
            {
            num = !num;
                strcat(rm.id, buffer);
FILE* part_arr = fopen("partition_list.txt", "rb+");
    PD data[100];
    fseek(part_arr, 0, SEEK_SET);
    fread(&data,sizeof(data),1,part_arr);
    fclose(part_arr);
                int n = valExistu(data,buffer);
                if(n != -1){
    PD part_val;
    memset(&part_val.id,'\0',sizeof(part_val.id));
    memset(&part_val.name,'\0',sizeof(part_val.name));
    strcat(part_val.name,"");
    data[n] = part_val;
    FILE* part_arr = fopen("partition_list.txt", "wb+");
            fseek(part_arr, 0, SEEK_SET);
            fwrite(&data,sizeof(data),1,part_arr);
            fclose(part_arr);
    printf("    - Particion desmontada.\n");
                }
                else{
    printf("    - Particion no existe.\n");
                }
            }
            else
            {
                // Errores
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    }
    if((j != 0) && read)
        {
            buffer[j] = '\0';
            // Posible optimizacion
            if(strcasecmp("ID", name) == 0)
            {
            num = !num;
                strcat(rm.id, buffer);
FILE* part_arr = fopen("partition_list.txt", "rb+");
    PD data[100];
    fseek(part_arr, 0, SEEK_SET);
    fread(&data,sizeof(data),1,part_arr);
    fclose(part_arr);
                int n = valExistu(data,buffer);
                if(n != -1){
    PD part_val;
    memset(&part_val.id,'\0',sizeof(part_val.id));
    memset(&part_val.name,'\0',sizeof(part_val.name));
    strcat(part_val.name,"");
    strcat(part_val.id,"");
    data[n] = part_val;
    FILE* part_arr = fopen("partition_list.txt", "wb+");
            fseek(part_arr, 0, SEEK_SET);
            fwrite(&data,sizeof(data),1,part_arr);
            fclose(part_arr);
    printf("    - Particion desmontada.\n");
                }
                else{
    printf("    - Particion no existe.\n");
                }
            }
            else
            {
                // Errores
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    // unmount &id1->vda1
    printf("\n");
    return 0;
}

