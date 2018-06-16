#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include <dirent.h>
#include <errno.h>
#include "structures.h"

typedef struct mkdisk
{
    int sizeDisk;
    char path[80];
    char name[25];
    char unit[5];
} mk_init;

int mkContructor(int state, char str[], int cont)
{
    if(cont == -1){
        printf("    - Comando sin parametros.\n\n");
        return 0;
    }
    // Size Disk
    int sizeDisk = 1024*1024;
    // Command MKDISK
    mk_init mk;
    memset(&mk.name,'\0',sizeof(mk.name));
    memset(&mk.path,'\0',sizeof(mk.path));
    memset(&mk.unit,'\0',sizeof(mk.unit));
    strcat(mk.unit, "M");
    // Base Program
    char ch, buffer[100];
    memset(&buffer,'\0',sizeof(buffer));
    int j=0;
    bool read = true;
    char name[10];
    memset(&name,'\0',sizeof(name));
    while((ch = str[cont++]) != '\0')
    {
        // printf("%c %s\n",ch,read ? "true" : "false");
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
                if(strcasecmp("M",buffer)){
                    strcat(mk.unit, "M");
                    sizeDisk = 1024 * 1024;
                }
                else if(strcasecmp("K",buffer)){
                    strcat(mk.unit, "K");
                    sizeDisk = 1024;
                }
                else{
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
                if(strcasecmp("M",buffer)){
                    strcat(mk.unit, "M");
                    sizeDisk = 1024 * 1024;
                }
                else if(strcasecmp("K",buffer)){
                    strcat(mk.unit, "K");
                    sizeDisk = 1024;
                }
                else{
                }
            }
            else
            {
                // Errores
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    /*printf("%s\n", mk.name);
    printf("%s\n", mk.path);
    printf("%d\n", mk.sizeDisk);
    printf("%s\n", mk.unit);
    printf("%d\n", mk.sizeDisk*sizeDisk);*/
    // mkdisk &SiZe->8 &pAth->"/home/carlos/Documents/MIA/MIA_Proyecto1/Discos/" &namE->Disco_3.dsk
    char* comando = (char*)malloc(500);
    memset(&comando[0], 0, sizeof(comando));
    // Crear Path
    DIR* dir = opendir(mk.path);
    if (dir)
    {
        /* Directory exists. */
    closedir(dir);
    }
    else if (ENOENT == errno)
    {
        strcat(comando, "sudo mkdir -p \"");
        strcat(comando, mk.path);
        strcat(comando, "\"");
        // printf(comando);
        system(comando);
        /* Directory does not exist. */
    }
    else
    {
        /* opendir() failed for some other reason. */
    }
    // Crear el disco /home/carlos/Documents/MIA/MIA_Proyecto1
    char* megas=(char*)malloc(15);
    memset(&comando[0], 0, sizeof(comando));
    memset(&megas[0], 0, sizeof(megas));
    strcat(comando, "sudo dd if=/dev/zero of=\"");
    strcat(comando, mk.path);
    //strcat(comando, "/");
    strcat(comando, mk.name);
    //"strcat(comando, ".dsk ");
    strcat(comando, "\" bs=");
    strcat(comando, mk.unit);
    strcat(comando, " count=");
    sprintf(megas, "%d", mk.sizeDisk);
    strcat(comando, megas);
    system(comando); // ejecuta el comando en la terminal del SO
    // Path
    char* pathaux=(char*)malloc(150);
    char* pathperm=(char*)malloc(150);
    char* permission=(char*)malloc(150);
    memset(&pathperm[0], 0, sizeof(pathaux));
    memset(&pathaux[0], 0, sizeof(pathaux));
    memset(&permission[0], 0, sizeof(permission));
    strcat(pathaux, mk.path);
    strcat(pathaux, mk.name);
    strcat(pathperm, "\"");
    strcat(pathperm, mk.path);
    strcat(pathperm, mk.name);
    strcat(pathperm, "\"");
    // Give All Permission
    strcat(permission,"sudo chmod 777 ");
    strcat(permission, pathperm);
    system(permission);

    // Crear MBR
    MBR diskInfo;
    MBR_P blankPart;
    blankPart.part_start = 0;
    blankPart.part_end = 0;
    memset(&blankPart.part_name,'\0',sizeof(blankPart.part_name));
    strcat(blankPart.part_name,"");
    diskInfo.mbr_partition[0] = blankPart;
    diskInfo.mbr_partition[1] = blankPart;
    diskInfo.mbr_partition[2] = blankPart;
    diskInfo.mbr_partition[3] = blankPart;
    diskInfo.mbr_tam = mk.sizeDisk * sizeDisk;
    memset(&diskInfo.mbr_creation_time,0,sizeof(diskInfo.mbr_creation_time));
    // Fecha del sistema
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    strftime(diskInfo.mbr_creation_time,128,"%d/%m/%y %H:%M:%S",tlocal);
    // random number
    srand(time(NULL));
    diskInfo.mbr_disk_signature = rand();
    // strcat(pathaux, ".dsk");

    FILE* disk = fopen(pathaux, "rb+");
    /*if(disk == NULL){
    printf("fallo");
    }
    printf("  err %d \n", errno);*/
    // Guardar MBR
    fseek(disk, 0, SEEK_SET);
    fwrite(&diskInfo,sizeof(MBR),1,disk);
    MBR test;
    fseek(disk, 0, SEEK_SET);
    fread(&test,sizeof(MBR),1,disk);
    printf("ID Disco: %d \n",test.mbr_disk_signature);
    printf("Fecha De Creacion: %s \n",test.mbr_creation_time);
    printf("\n");
    /*if(strcasecmp(test.mbr_partition_1.part_name,"") == 0){
    printf("Si es nulo");
    }*/
    free(pathaux);
    fclose(disk);
    return 0;
    // mkdisk &SiZe->8 &pAth->"/home/carlos/Documents/MIA/MIA_Proyecto1/Discos/" &namE->Disco_1.dsk
}
