#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

typedef struct intervalo{
    int i_start;
    int i_end;
} i_part;

typedef struct fdisk
{
    int sizePartition;
    char unit[5];
    char path[80];
    char type[5];
    char fit[5];
    char allocation[5];
    char deletePartition[5];
    char name[25];
    int addPartition;
} f_init;

void sortArr(i_part *unarray, int lalong) {
         int i, j;
         i_part aux;

         for (i = 0; i < lalong -1 ; i++) {
                 for (j = i + 1; j < lalong ; j++) {
                         if (unarray[i].i_start > unarray[j].i_start) {
                                aux = unarray[i];
                                unarray[i] = unarray[j];
                                unarray[j] = aux;
                         }
                }
         }
}

int blockSizeN(int part_size){
    int value = (part_size/(sizeof(BM) + sizeof(BLK)));
    return value;
}

int firstPartition(MBR disk){
    for(int i=0; i<4; i++){
    if(strcasecmp(disk.mbr_partition[i].part_name,"") != 0){
    return 0;
    }
    }
    return 1;
}

int availablePartition(MBR disk){
    for(int i=0; i<4; i++){
    if(strcasecmp(disk.mbr_partition[i].part_name,"") == 0){
    return i;
    }
    }
    return -1;
}


int extendExist(MBR disk){
    for(int i=0; i<4; i++){
    if(strcasecmp(disk.mbr_partition[i].part_type,"E") == 0){
    return 1;
    }
    }
    return 0;
}

int fContructor(int state, char str[], int cont)
{
    if(cont == -1){
        printf("    - Comando sin parametros.\n\n");
        return 0;
    }
    // Size Disk
    int sizeDisk = 1024;
    // Command MKDISK
    f_init fc;
    memset(&fc.name,'\0',sizeof(fc.name));
    memset(&fc.path,'\0',sizeof(fc.path));
    memset(&fc.unit,'\0',sizeof(fc.unit));
    memset(&fc.type,'\0',sizeof(fc.type));
    memset(&fc.fit,'\0',sizeof(fc.fit));
    memset(&fc.allocation,'\0',sizeof(fc.allocation));
    memset(&fc.deletePartition,'\0',sizeof(fc.deletePartition));
    memset(&fc.addPartition,'\0',sizeof(fc.addPartition));
    strcat(fc.unit, "K");
    strcat(fc.type, "P");
    strcat(fc.fit, "WF");
    strcat(fc.allocation, "IX");
    // Base Program
    char ch, buffer[100];
    memset(&buffer,'\0',sizeof(buffer));
    int j=0;
    bool read = true;
    char name[15];
    memset(&name,'\0',sizeof(name));
    while((ch = str[cont++]) != '\0')
    {
        // printf("%c %s\n",ch,read ? "true" : "false");
        // printf("%c %s\n",ch,read ? "true" : "false");
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
                fc.sizePartition = atoi(buffer);
            }
            else if(strcasecmp("ADD", name) == 0)
            {
                fc.addPartition = atoi(buffer);
            }
            else if(strcasecmp("PATH", name) == 0)
            {
                strcat(fc.path, buffer);
            }
            else if(strcasecmp("NAME", name) == 0)
            {
                strcat(fc.name, buffer);
            }
            else if(strcasecmp("TYPE", name) == 0)
            {
                memset(&fc.type,'\0',sizeof(fc.type));
                strcat(fc.type, buffer);
            }
            else if(strcasecmp("FIT", name) == 0)
            {
                memset(&fc.fit,'\0',sizeof(fc.fit));
                strcat(fc.fit, buffer);
            }
            else if(strcasecmp("ALLOCATION", name) == 0)
            {
                memset(&fc.allocation,'\0',sizeof(fc.allocation));
                strcat(fc.allocation, buffer);
            }
            else if(strcasecmp("DELETE", name) == 0)
            {
                strcat(fc.deletePartition, buffer);
            }
            else if(strcasecmp("UNIT", name) == 0)
            {
                memset(&fc.unit,'\0',sizeof(fc.unit));
                if(strcasecmp("M",buffer) == 0){
                    strcat(fc.unit, "M");
                    sizeDisk = 1024 * 1024;
                }
                else if(strcasecmp("K",buffer) == 0){
                    strcat(fc.unit, "K");
                    sizeDisk = 1024;
                }
                else if(strcasecmp("B",buffer) == 0){
                    strcat(fc.unit, "B");
                    sizeDisk = 1;
                }
                else{
                printf("    - \"%s\" no es un valor valido para el comando UNIT\n\n");
                return 0;
                }
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
            if(strcasecmp("SIZE", name) == 0)
            {
                fc.sizePartition = atoi(buffer);
            }
            else if(strcasecmp("ADD", name) == 0)
            {
                fc.addPartition = atoi(buffer);
            }
            else if(strcasecmp("PATH", name) == 0)
            {
                strcat(fc.path, buffer);
            }
            else if(strcasecmp("NAME", name) == 0)
            {
                strcat(fc.name, buffer);
            }
            else if(strcasecmp("TYPE", name) == 0)
            {
                // Validar
                memset(&fc.type,'\0',sizeof(fc.type));
                strcat(fc.type, buffer);
            }
            else if(strcasecmp("FIT", name) == 0)
            {
                memset(&fc.fit,'\0',sizeof(fc.fit));
                strcat(fc.fit, buffer);
            }
            else if(strcasecmp("ALLOCATION", name) == 0)
            {
                memset(&fc.allocation,'\0',sizeof(fc.allocation));
                strcat(fc.allocation, buffer);
            }
            else if(strcasecmp("DELETE", name) == 0)
            {
                strcat(fc.deletePartition, buffer);
            }
            else if(strcasecmp("UNIT", name) == 0)
            {
                memset(&fc.unit,'\0',sizeof(fc.unit));
                if(strcasecmp("M",buffer) == 0){
                    strcat(fc.unit, "M");
                    sizeDisk = 1024 * 1024;
                }
                else if(strcasecmp("K",buffer) == 0){
                    strcat(fc.unit, "K");
                    sizeDisk = 1024;
                }
                else if(strcasecmp("B",buffer) == 0){
                    strcat(fc.unit, "B");
                    sizeDisk = 1;
                }
                else{
                printf("    - \"%s\" no es un valor valido para el comando UNIT\n\n");
                return 0;
                }
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
    /*printf("%s\n", fc.path);
    printf("%s\n", fc.name);
    printf("%d\n", fc.sizePartition);
    printf("%s\n", fc.unit);
    printf("%s\n", fc.allocation);
    printf("%s\n", fc.type);*/
    if(fc.addPartition != NULL){
    // Agregar o quitar espacio
    }
    else if(fc.deletePartition[0] != '\0'){
    // Eliminar particion
    }
    else{
    // Crear Particion
    // Validar si el disco existe!
    if( access( fc.path, F_OK ) != -1 ) {
    // file exists
    // printf("exist");
    char* pathaux=(char*)malloc(150);
    char* permission=(char*)malloc(150);
    memset(&pathaux[0], 0, sizeof(pathaux));
    memset(&permission[0], 0, sizeof(permission));
    strcat(pathaux, "\"");
    strcat(pathaux, fc.path);
    strcat(pathaux, "\"");
    // Give All Permission
    /*strcat(permission,"sudo chmod 777 ");
    strcat(permission, pathaux);
    system(permission);*/
    FILE* disk = fopen(fc.path, "rb+");
   if(disk == NULL){
    printf("    - Disco no existe.\n\n");
    return 0;
    }
    // printf("  err %d \n", errno);
    MBR actualDisk;
    fseek(disk, 0, SEEK_SET);
    fread(&actualDisk,sizeof(MBR),1,disk);
    printf("ID Disco: %d \n",actualDisk.mbr_disk_signature);
    printf("Fecha De Creacion: %s \n",actualDisk.mbr_creation_time);
    printf("\n");
    free(permission);
    free(pathaux);
    fclose(disk);
    // Arreglo de particiones
    i_part partList[4];
    for(int i=0;i<4;i++){
        i_part aux;
        aux.i_start = actualDisk.mbr_partition[i].part_start;
        aux.i_end = actualDisk.mbr_partition[i].part_end;
        partList[i] = aux;
    }
    // Ordenar arreglo
    sortArr(partList, 4);
    // Revisar si se puede crear una particion
    int partNumber = availablePartition(actualDisk);
    int extExist = extendExist(actualDisk);
    if(partNumber != -1){
    if(strcasecmp(fc.type,"E") == 0 && extExist == 1){
        printf("    - Ya existe una particion extendidad en el disco.\n\n");
        return 0;
    }
    else if(strcasecmp(fc.type,"L") == 0 && extExist != 1){
        printf("    - No existe una particion extendidad en el disco.\n\n");
        return 0;
    }
    // Crear particion
    MBR_P partitionTarget;
    // Limpieza variables
    memset(&partitionTarget.part_fit[0], 0, sizeof(partitionTarget.part_fit));
    memset(&partitionTarget.part_name[0], 0, sizeof(partitionTarget.part_name));
    memset(&partitionTarget.part_type[0], 0, sizeof(partitionTarget.part_type));
    partitionTarget.part_status = 0;
    partitionTarget.part_status = 0;
    strcat(partitionTarget.part_fit,fc.fit);
    strcat(partitionTarget.part_name,fc.name);
    strcat(partitionTarget.part_type,fc.type);
    partitionTarget.part_size = fc.sizePartition * sizeDisk;
    // printf("%d\n",partitionTarget.part_size);
    // printf("%d\n",actualDisk.mbr_tam);
    int writePosition = sizeof(MBR);
    int writePartition = 0;
    if(partitionTarget.part_size > actualDisk.mbr_tam){
        printf("    - Particion excede el tamaño del disco.\n\n");
        return 0;
    }
    int isFirstPartition = firstPartition(actualDisk);
    int size_block = blockSizeN(partitionTarget.part_size);
    // printf("%d \n",size_block);
    // printf("%d\n",isFirstPartition);
    if(isFirstPartition == 1){
        partitionTarget.part_start = writePosition;
        partitionTarget.part_end = writePosition + partitionTarget.part_size;
        printf("Start: %d\n",partitionTarget.part_start);
        printf("End: %d\n",partitionTarget.part_end);
        if(strcasecmp(fc.type,"P") == 0){
        printf("Cantidad De Bloques: %d\n",size_block);
        partitionTarget.block_size = size_block;
        /* // Crear BitMap
        //fdisk &sizE->72 &path->"/home/carlos/Documents/MIA/MIA_Proyecto1/Discos/Disco_3.dsk" &name->Particion1
        int start = partitionTarget.part_start;
        BM bmWrite;
        bmWrite.value = '0';
        disk = fopen(fc.path, "rb+");
        for(int i=0;i<partitionTarget.block_size;i++){
            int pos = start + (i*sizeof(BM));
            fseek(disk, pos, SEEK_SET);
            fwrite(&bmWrite,sizeof(BM),1,disk);
        }
        fclose(disk);
        disk = fopen(fc.path, "rb+");
        BLK blkWrite;
        memset(&blkWrite.name, '\0', sizeof(blkWrite.name));
        strcat(blkWrite.name,"Prueba");
        memset(&blkWrite.data, '\0', sizeof(blkWrite.data));
        for(int i=0;i<partitionTarget.block_size;i++){
            int pos = start + (partitionTarget.block_size*sizeof(BM)) + i*sizeof(BLK);
            fseek(disk, pos, SEEK_SET);
            fwrite(&blkWrite,sizeof(BLK),1,disk);
        }
        fclose(disk);*/
        }
        else{
        // Particion Logica
        }
    }
    else{
    // Existen particiones en el sistema
    bool found = false;
    for(int i=0;i<4;i++){
        if(partList[i].i_start != 0){
            if((partList[i].i_start - writePosition) >= partitionTarget.part_size){
                found = true;
                break;
            }
            else{
            writePosition = partList[i].i_end;
            }
        }
        else{
            // Algo
        }
    }
    if(!found){
        // Validar Ultimo
        if((actualDisk.mbr_tam - writePosition) >= partitionTarget.part_size){
                found = true;
            }
            else{
            printf("    - Particion excede el tam. posible en disco.\n\n");
            return 0;
            }
    }
        partitionTarget.part_start = writePosition;
        partitionTarget.part_end = writePosition + partitionTarget.part_size;
        printf("Start: %d\n",partitionTarget.part_start);
        printf("End: %d\n",partitionTarget.part_end);
        if(strcasecmp(fc.type,"P") == 0){
        printf("Cantidad De Bloques: %d\n",size_block);
        partitionTarget.block_size = size_block;
        // Crear BitMap
        //fdisk &sizE->72 &path->"/home/carlos/Documents/MIA/MIA_Proyecto1/Discos/Disco_3.dsk" &name->Particion1
        /*int start = partitionTarget.part_start;
        BM bmWrite;
        bmWrite.value = '0';
        disk = fopen(fc.path, "rb+");
        for(int i=0;i<partitionTarget.block_size;i++){
            int pos = start + (i*sizeof(BM));
            fseek(disk, pos, SEEK_SET);
            fwrite(&bmWrite,sizeof(BM),1,disk);
        }
        fclose(disk);
        disk = fopen(fc.path, "rb+");
        BLK blkWrite;
        memset(&blkWrite.name, '\0', sizeof(blkWrite.name));
        strcat(blkWrite.name,"");
        memset(&blkWrite.data, '\0', sizeof(blkWrite.data));
        for(int i=0;i<partitionTarget.block_size;i++){
            int pos = start + (partitionTarget.block_size*sizeof(BM)) + i*sizeof(BLK);
            fseek(disk, pos, SEEK_SET);
            fwrite(&blkWrite,sizeof(BLK),1,disk);
        }
        fclose(disk);*/
        }
        else{
        // Particion Logica
        }
    }
    // Actualizar MBR
    actualDisk.mbr_partition[partNumber] = partitionTarget;
    disk = fopen(fc.path, "rb+");
    fseek(disk, 0, SEEK_SET);
    fwrite(&actualDisk,sizeof(MBR),1,disk);
    fclose(disk);
    }
    else{
    // Error
    printf("    - El sistema ya no admite mas particiones.\n\n");
    return 0;
    }
    } else {
    // file doesn't exist
    printf("    - El disco no existe en el sistema.\n\n");
    return 0;
    }
    }
    printf("\n");
    // fdisk &sizE->72 &path->"/home/carlos/Documents/MIA/MIA_Proyecto1/Discos/Disco_1.dsk" &name->Part1
    return 0;
}
