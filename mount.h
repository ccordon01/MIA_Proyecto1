#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include <dirent.h>
#include <errno.h>

typedef struct mount
{
    char path[80];
    char name[25];
} m_init;

int availableData(PD data[]){
    for(int i=0;i<100;i++){
        if(strcasecmp(data[i].name,"") == 0){
            return i;
        }
    }
    return 0;
}

char valExist(PD data[], char val[]){
    for(int i=0;i<100;i++){
        if(strcasecmp(data[i].name,val) == 0){
            return data[i].letter;
        }
    }
    return '\0';
}

int mContructor(int state, char str[], int cont)
{
    if(cont == -1){
        // Lista todas las particiones
    FILE* part_arr = fopen("partition_list.txt", "rb+");
    PD data[100];
    fseek(part_arr, 0, SEEK_SET);
    fread(&data,sizeof(data),1,part_arr);
    fclose(part_arr);
    for(int i=0;i<100;i++){
        if(strcasecmp(data[i].name,"") != 0){
        printf("    - %s\n",data[i].id);
        }
    }
        printf("\n");
        return 0;
    }
    m_init m;
    memset(&m.path,'\0',sizeof(m.path));
    memset(&m.name,'\0',sizeof(m.name));
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
                strcat(m.path, buffer);
            }
            else if(strcasecmp("NAME", name) == 0)
            {
                strcat(m.name, buffer);
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
                strcat(m.path, buffer);
            }
            else if(strcasecmp("NAME", name) == 0)
            {
                strcat(m.name, buffer);
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
    // mount &path->"/home/carlos/Documents/MIA/MIA_Proyecto1/Discos/Disco_3.dsk" &name->Part1
    FILE* disk = fopen(m.path, "rb+");
    if(disk == NULL){
    printf("    - Disco no existe.\n");
    printf("  err %d \n", errno);
    return 0;
    }
    PD part_val;
    memset(&part_val.id,'\0',sizeof(part_val.id));
    memset(&part_val.name,'\0',sizeof(part_val.name));
    memset(&part_val.path,'\0',sizeof(part_val.path));
    MBR actualDisk;
    fseek(disk, 0, SEEK_SET);
    fread(&actualDisk,sizeof(MBR),1,disk);
    fclose(disk);
    // Cargar Arreglo
    FILE* part_arr = fopen("partition_list.txt", "rb+");
    PD data[100];
    fseek(part_arr, 0, SEEK_SET);
    fread(&data,sizeof(data),1,part_arr);
    fclose(part_arr);
    for(int i=0;i<4;i++){
        if(strcasecmp(actualDisk.mbr_partition[i].part_name,m.name) == 0){
            srand(time(NULL));
            int randNum = 26 * (rand() / (RAND_MAX +1.0));
            randNum = randNum + 97;
            char randChar = (char) randNum;
            part_val.letter = randChar;
            char exist = valExist(data,m.name);
            strcat(part_val.name,m.name);
            strcat(part_val.path,m.path);
            if(exist != '\0'){
                part_val.letter = exist;
            }
            strcat(part_val.id,"vd");
            strcat(part_val.id,&part_val.letter);
            char numConvert[3];
            sprintf(numConvert, "%d", (i+1));
            strcat(part_val.id,numConvert);
            int posWrite = availableData(data);
            data[posWrite] = part_val;
            FILE* part_arr = fopen("partition_list.txt", "wb+");
            fseek(part_arr, 0, SEEK_SET);
            fwrite(&data,sizeof(data),1,part_arr);
            fclose(part_arr);
            printf("    - Particion montada %s\n",part_val.id);
            printf("\n");
            return 0;
        }
    }
    printf("    - Particion no existe\n");
    printf("\n");
    // mount &path->"/home/carlos/Documents/MIA/MIA_Proyecto1/Discos/Disco_1.dsk" &name->Part1
    return 0;
}
