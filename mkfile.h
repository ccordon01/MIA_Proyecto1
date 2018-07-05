#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include <dirent.h>
#include <errno.h>

typedef struct mkfile
{
    char id[15];
    char name[15];
    char mkf_data[500];
} mkf_init;

int mkfContructor(int state, char str[], int cont)
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
    mkf_init rm;
    memset(&rm.id,'\0',sizeof(rm.id));
    memset(&rm.name,'\0',sizeof(rm.name));
    memset(&rm.mkf_data,'\0',sizeof(rm.mkf_data));
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
            else if(strcasecmp("NAME", name) == 0)
            {
                strcat(rm.name, buffer);
            }
            else if(strcasecmp("CONTENIDO", name) == 0)
            {
                strcat(rm.mkf_data, buffer);
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
            else if(strcasecmp("NAME", name) == 0)
            {
                strcat(rm.name, buffer);
            }
            else if(strcasecmp("CONTENIDO", name) == 0)
            {
                strcat(rm.mkf_data, buffer);
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
                    int cont = 0;
                    char ch, buffer[50];
                    memset(&buffer,'\0',sizeof(buffer));
                    while((ch = rm.mkf_data[cont++]) != '\0'){
                        buffer[j++] = ch;
                        if(cont == 49){
                            buffer[j] = '\0';
                            j = 0;
                            // printf("....%s\n",buffer);
                            int start = mbrDisk.mbr_partition[i].part_start;
                            disk = fopen(aux.path, "rb+");
                            for(int k=0;k<mbrDisk.mbr_partition[i].block_size;k++){
                                int pos = start + (k*sizeof(BM));
                                fseek(disk, pos, SEEK_SET);
                                BM bmRead;
                                fread(&bmRead,sizeof(BM),1,disk);
                                // printf("%c\n",bmRead.value);
                                if(bmRead.value == '0'){
                                // write BitMap
                                bmRead.value = '1';
                                pos = start + (k*sizeof(BM));
                                fseek(disk, pos, SEEK_SET);
                                fwrite(&bmRead,sizeof(BM),1,disk);
                                // write block
                                BLK blkWrite;
                                memset(&blkWrite.name, '\0', sizeof(blkWrite.name));
                                strcat(blkWrite.name,rm.name);
                                memset(&blkWrite.data, '\0', sizeof(blkWrite.data));
                                strcat(blkWrite.data,buffer);
                                pos = start + (mbrDisk.mbr_partition[i].block_size*sizeof(BM)) + k*sizeof(BLK);
                                fseek(disk, pos, SEEK_SET);
                                fwrite(&blkWrite,sizeof(BLK),1,disk);
                                fclose(disk);
                                break;
                                }
                            }
                            memset(&buffer,'\0',sizeof(buffer));
                        }
                    }
                        if(j != 0){
                            buffer[j] = '\0';
                            j = 0;
                            int start = mbrDisk.mbr_partition[i].part_start;
                            disk = fopen(aux.path, "rb+");
                            for(int k=0;k<mbrDisk.mbr_partition[i].block_size;k++){
                                int pos = start + (k*sizeof(BM));
                                fseek(disk, pos, SEEK_SET);
                                BM bmRead;
                                fread(&bmRead,sizeof(BM),1,disk);
                                if(bmRead.value == '0'){
                                // write BitMap
                                bmRead.value = '1';
                                pos = start + (k*sizeof(BM));
                                fseek(disk, pos, SEEK_SET);
                                fwrite(&bmRead,sizeof(BM),1,disk);
                                // write block
                                BLK blkWrite;
                                memset(&blkWrite.name, '\0', sizeof(blkWrite.name));
                                strcat(blkWrite.name,rm.name);
                                memset(&blkWrite.data, '\0', sizeof(blkWrite.data));
                                strcat(blkWrite.data,buffer);
                                pos = start + (mbrDisk.mbr_partition[i].block_size*sizeof(BM)) + k*sizeof(BLK);
                                fseek(disk, pos, SEEK_SET);
                                fwrite(&blkWrite,sizeof(BLK),1,disk);
                                fclose(disk);
                                break;
                                }
                            }
                            memset(&buffer,'\0',sizeof(buffer));
                        }
                }
                }
                }
                else{
                printf("    - Particion no existe.\n\n");
                return 0;
                }
    // Mkfile &id->vdt1 &name->archivo0 &contenido->" Este es un texto para el archivo que se estará generando en el sistema"
    printf("    - Informacion guardada en disco\n\n");
    return 0;
}


