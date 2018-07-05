#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include <dirent.h>
#include <errno.h>
#include "diskManage.h"

typedef struct mkfs
{
    char id[16];
    char tipo[16];
    char add[16];
    char unit[16];
} mkfs_init;

int blockSizeNP(int part_size){
    int value = ((part_size - (2 * sizeof(BL)))/(sizeof(BMAVD) + sizeof(AVD) + sizeof(BMDD) + sizeof(DD) + sizeof(BMTI) + sizeof(TI) + sizeof(BMBD) + sizeof(BD) + sizeof(BL)));
    return value;
}

int mkfsContructor(int state, char str[], int cont)
{
    if(cont == -1){
        printf("    - Comando sin parametros.\n\n");
        return 0;
    }
    FILE* part_arr = fopen("partition_list.txt", "rb+");
    PD data[100];
    fseek(part_arr, 0, SEEK_SET);
    fread(&data,sizeof(data),1,part_arr);
    fclose(part_arr);
    mkfs_init rm;
    memset(&rm.id,'\0',sizeof(rm.id));
    memset(&rm.tipo,'\0',sizeof(rm.tipo));
    memset(&rm.add,'\0',sizeof(rm.add));
    memset(&rm.unit,'\0',sizeof(rm.unit));
    // Base Program
    char ch, buffer[100];
    memset(&buffer,'\0',sizeof(buffer));
    int j=0;
    bool read = true;
    bool num = true;
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
            // num = !num;
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
                strcat(rm.id, buffer);
            }
            else if(strcasecmp("TIPO", name) == 0)
            {
                strcat(rm.tipo, buffer);
            }
            else if(strcasecmp("ADD", name) == 0)
            {
                strcat(rm.add, buffer);
            }
            else if(strcasecmp("UNIT", name) == 0)
            {
                strcat(rm.unit, buffer);
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
            if(strcasecmp("ID", name) == 0)
            {
                strcat(rm.id, buffer);
            }
            else if(strcasecmp("TIPO", name) == 0)
            {
                strcat(rm.tipo, buffer);
            }
            else if(strcasecmp("ADD", name) == 0)
            {
                strcat(rm.add, buffer);
            }
            else if(strcasecmp("UNIT", name) == 0)
            {
                strcat(rm.unit, buffer);
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

                int n = valExistu(data,rm.id);
                if(n != -1){
                PD aux = data[n];
                FILE* disk = fopen(aux.path, "rb+");
                MBR mbrDisk;
                fseek(disk, 0, SEEK_SET);
                fread(&mbrDisk,sizeof(MBR),1,disk);
                fclose(disk);
                /*printf("ID Disco: %d \n",mbrDisk.mbr_disk_signature);
                printf("Fecha De Creacion: %s \n",mbrDisk.mbr_creation_time);
                printf("\n");*/
                for(int i=0;i<4;i++){
                if(strcasecmp(mbrDisk.mbr_partition[i].part_name,aux.name) == 0){
                    int start = mbrDisk.mbr_partition[i].part_start;
                    int part_size = mbrDisk.mbr_partition[i].part_size;
                    int block_size = blockSizeNP(part_size);
                    printf("Cantidad de bloques creados: %d\n",block_size);
                    // Crear Super boot
                    SB superBoot;
                    memset(&superBoot.sb_nombre_hd,'\0',sizeof(superBoot.sb_nombre_hd));
                    // Nombre disco virtual
                    superBoot.sb_arbol_virtual_count = block_size;
                    superBoot.sb_detalle_directorio_count = block_size;
                    superBoot.sb_inodos_count = block_size;
                    superBoot.sb_bloques_count = block_size;
                    superBoot.sb_arbol_virtual_free = block_size;
                    superBoot.sb_detalle_directorio_free = block_size;
                    superBoot.sb_inodos_free = block_size;
                    superBoot.sb_bloques_free = block_size;
                    memset(&superBoot.sb_date_creacion,0,sizeof(superBoot.sb_date_creacion));
                    memset(&superBoot.sb_date_ultimo_montaje,0,sizeof(superBoot.sb_date_ultimo_montaje));
                    // Fecha del sistema
                    time_t tiempo = time(0);
                    struct tm *tlocal = localtime(&tiempo);
                    strftime(superBoot.sb_date_creacion,128,"%d/%m/%y %H:%M:%S",tlocal);
                    strftime(superBoot.sb_date_ultimo_montaje,128,"%d/%m/%y %H:%M:%S",tlocal);
                    superBoot.sb_montajes_count = 1;
                    superBoot.sb_ap_bitmap_arbol_directorio = start + sizeof(SB);
                    superBoot.sb_ap_arbol_directorio = superBoot.sb_ap_bitmap_arbol_directorio + block_size*sizeof(BM);
                    superBoot.sb_ap_bitmap_detalle_directorio = superBoot.sb_ap_arbol_directorio + block_size*sizeof(AVD);
                    superBoot.sb_ap_detalle_directorio = superBoot.sb_ap_bitmap_detalle_directorio + block_size*sizeof(BM);
                    superBoot.sb_ap_bitmap_tabla_inodo = superBoot.sb_ap_detalle_directorio + block_size*sizeof(DD);
                    superBoot.sb_ap_tabla_inodo = superBoot.sb_ap_bitmap_tabla_inodo + block_size*sizeof(BM);
                    superBoot.sb_ap_bitmap_bloques = superBoot.sb_ap_tabla_inodo + block_size*sizeof(TI);
                    superBoot.sb_ap_bloques = superBoot.sb_ap_bitmap_bloques + block_size*sizeof(BM);
                    superBoot.sb_ap_log = superBoot.sb_ap_bloques + block_size*sizeof(BD);
                    superBoot.sb_size_struct_arbol_directorio = sizeof(AVD);
                    superBoot.sb_size_struct_detalle_directorio =  sizeof(DD);
                    superBoot.sb_size_struct_inodo = sizeof(TI);
                    superBoot.sb_size_struct_bloque = sizeof(BD);
                    superBoot.sb_first_free_bit_arbol_directorio = 1;
                    superBoot.sb_first_free_bit_detalle_directorio = 1;
                    superBoot.sb_first_free_bit_tabla_inodo = 1;
                    superBoot.sb_first_free_bit_bloques = 1;
                    superBoot.sb_magic_num = 201504427;
                    // Guardar super boot
                    disk = fopen(aux.path, "rb+");
                    fseek(disk, start, SEEK_SET);
                    fwrite(&superBoot,sizeof(SB),1,disk);
                    fclose(disk);
                    // Escribir bitmap directorio
                    int pos = 0;
                    disk = fopen(aux.path, "rb+");
                    BM bitpart;
                    memset(&bitpart.value,'\0',sizeof(bitpart.value));
                    bitpart.value = '0';
                    for(int i=0; i< block_size; i++){
                    pos = superBoot.sb_ap_bitmap_arbol_directorio + (i*sizeof(BM));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&bitpart,sizeof(BM),1,disk);
                    }
                    fclose(disk);
                    // Escribir bloque directorio
                    disk = fopen(aux.path, "rb+");
                    AVD bloqueAVD;
                    for(int i=0; i< block_size; i++){
                    pos = superBoot.sb_ap_arbol_directorio + (i*sizeof(AVD));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&bloqueAVD,sizeof(AVD),1,disk);
                    }
                    fclose(disk);
                    // Escribir bitmap detalle directorio
                    disk = fopen(aux.path, "rb+");
                    for(int i=0; i< block_size; i++){
                    pos = superBoot.sb_ap_bitmap_detalle_directorio + (i*sizeof(BM));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&bitpart,sizeof(BM),1,disk);
                    }
                    fclose(disk);
                    // Escribir bloque detalle directorio
                    disk = fopen(aux.path, "rb+");
                    DD bloqueDD;
                    for(int i=0; i< block_size; i++){
                    pos = superBoot.sb_ap_detalle_directorio + (i*sizeof(DD));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&bloqueDD,sizeof(DD),1,disk);
                    }
                    fclose(disk);
                    // Escribir bitmap tabla inodos
                    disk = fopen(aux.path, "rb+");
                    for(int i=0; i< block_size; i++){
                    pos = superBoot.sb_ap_bitmap_tabla_inodo + (i*sizeof(BM));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&bitpart,sizeof(BM),1,disk);
                    }
                    fclose(disk);
                    // Escribir bloque detalle directorio
                    disk = fopen(aux.path, "rb+");
                    TI bloqueTI;
                    for(int i=0; i< block_size; i++){
                    pos = superBoot.sb_ap_tabla_inodo + (i*sizeof(TI));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&bloqueTI,sizeof(TI),1,disk);
                    }
                    fclose(disk);
                    // Escribir bitmap bloques
                    disk = fopen(aux.path, "rb+");
                    for(int i=0; i< block_size; i++){
                    pos = superBoot.sb_ap_bitmap_bloques + (i*sizeof(BM));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&bitpart,sizeof(BM),1,disk);
                    }
                    fclose(disk);
                    // Escribir bloques
                    disk = fopen(aux.path, "rb+");
                    BD bloqueBD;
                    for(int i=0; i< block_size; i++){
                    pos = superBoot.sb_ap_bloques + (i*sizeof(BD));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&bloqueBD,sizeof(BD),1,disk);
                    }
                    fclose(disk);
                    // Escribir Log
                    disk = fopen(aux.path, "rb+");
                    BL bloqueBL;
                    for(int i=0; i< block_size; i++){
                    pos = superBoot.sb_ap_log + (i*sizeof(BL));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&bloqueBL,sizeof(BL),1,disk);
                    }
                    fclose(disk);
                    // Escribir Copia Super boot
                    disk = fopen(aux.path, "rb+");
                    pos = superBoot.sb_ap_log + (block_size*sizeof(BL));
                    fseek(disk, pos, SEEK_SET);
                    fwrite(&superBoot,sizeof(SB),1,disk);
                    fclose(disk);
                    // Crear detalle directorio raiz
                    bitmapWrite(aux.path,superBoot.sb_ap_bitmap_detalle_directorio,0, '1');
                    DDCreate(aux.path,superBoot.sb_ap_detalle_directorio,0);
                    // Crear directorio raiz
                    bitmapWrite(aux.path,superBoot.sb_ap_bitmap_arbol_directorio,0, '1');
                    char root[1];
                    memset(&root,'\\',sizeof(root));
                    AVDCreate(aux.path, superBoot.sb_ap_arbol_directorio, 0, 0,root);

                    // Actualizar punteros
                    superBoot.sb_first_free_bit_arbol_directorio = bitmapFree(aux.path,superBoot.sb_ap_bitmap_arbol_directorio,block_size);
                    superBoot.sb_first_free_bit_detalle_directorio = bitmapFree(aux.path,superBoot.sb_ap_bitmap_detalle_directorio,block_size);
                    // Escribir directorio raiz
                    // bitmapAD = bitmapFree(aux.path,superBoot.sb_ap_bitmap_arbol_directorio,block_size);
                    // printf("BITMAP ARBOL DIRECTORIO %d\n",bitmapAD);
                }
                }
                }
                else{
                printf("    - Particion no existe.\n\n");
                return 0;
                }
    // Mkfile &id->vdt1 &name->archivo0 &contenido->" Este es un texto para el archivo que se estará generando en el sistema"
    printf("    - Formato aplicado a la particion.\n\n");
    return 0;
}


