#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>

typedef struct rep
{
    char id[15];
    char name[15];
    char path[80];
} rep_init;

int repContructor(int state, char str[], int cont)
{
    char graf[1024];
    memset(&graf,'\0',sizeof(graf));
    strcat(graf, "digraph G {\n node [shape=record];\n  rankdir=LR\n");
    if(cont == -1){
        printf("    - Comando sin parametros.\n\n");
        return 0;
    }
    FILE* part_arr = fopen("partition_list.txt", "rb+");
    PD data[100];
    fseek(part_arr, 0, SEEK_SET);
    fread(&data,sizeof(data),1,part_arr);
    fclose(part_arr);
    rep_init rm;
    memset(&rm.id,'\0',sizeof(rm.id));
    memset(&rm.name,'\0',sizeof(rm.name));
    memset(&rm.path,'\0',sizeof(rm.path));
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
            else if(strcasecmp("PATH", name) == 0)
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
             if(strcasecmp("ID", name) == 0)
            {
                strcat(rm.id, buffer);
            }
            else if(strcasecmp("NAME", name) == 0)
            {
                strcat(rm.name, buffer);
            }
            else if(strcasecmp("PATH", name) == 0)
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
    // Mkfile &id->vdr1 &name->archivo0 &contenido->" Este es un texto para el archivo que se estará generando en el sistema"
                if(strcasecmp(rm.name, "BITMAP") == 0){
                int n = valExistu(data,rm.id);
                if(n != -1){
                PD aux = data[n];
                FILE* disk = fopen(aux.path, "rb+");
                MBR mbrDisk;
                fseek(disk, 0, SEEK_SET);
                fread(&mbrDisk,sizeof(MBR),1,disk);
                fclose(disk);
                printf("ID Disco: %d \n",mbrDisk.mbr_disk_signature);
                printf("Fecha De Creacion: %s \n",mbrDisk.mbr_creation_time);
                printf("\n");
                for(int i=0;i<4;i++){
                if(strcasecmp(mbrDisk.mbr_partition[i].part_name,aux.name) == 0){
                            int start = mbrDisk.mbr_partition[i].part_start;
                            disk = fopen(aux.path, "rb+");
                            int corte = sqrt(mbrDisk.mbr_partition[i].block_size);
                            for(int j=0;j<mbrDisk.mbr_partition[i].block_size;j++){
                                int pos = start + (j*sizeof(BM));
                                fseek(disk, pos, SEEK_SET);
                                BM bmRead;
                                fread(&bmRead,sizeof(BM),1,disk);
                                printf("%c  ",bmRead.value);
                                if(((j+1) % corte) == 0 && (j+1)>1){
                                printf("\n");
                                }
                            }
                            fclose(disk);
                    }
                }
                printf("\n\n    - Reporte MBR terminado. \n");
                }
                else{
                printf("    - Particion no existe.\n\n");
                return 0;
                }
                }
                else if(strcasecmp(rm.name, "BLOCK") == 0){
                int n = valExistu(data,rm.id);
                if(n != -1){
                PD aux = data[n];
                FILE* disk = fopen(aux.path, "rb+");
                MBR mbrDisk;
                fseek(disk, 0, SEEK_SET);
                fread(&mbrDisk,sizeof(MBR),1,disk);
                fclose(disk);
                printf("ID Disco: %d \n",mbrDisk.mbr_disk_signature);
                printf("Fecha De Creacion: %s \n",mbrDisk.mbr_creation_time);
                printf("\n");
                for(int i=0;i<4;i++){
                if(strcasecmp(mbrDisk.mbr_partition[i].part_name,aux.name) == 0){
                            int start = mbrDisk.mbr_partition[i].part_start;
                            disk = fopen(aux.path, "rb+");
                            int corte = sqrt(mbrDisk.mbr_partition[i].block_size);
                            for(int j=0;j<mbrDisk.mbr_partition[i].block_size;j++){
                                int pos = start + (j*sizeof(BM));
                                fseek(disk, pos, SEEK_SET);
                                BM bmRead;
                                fread(&bmRead,sizeof(BM),1,disk);
                                if(bmRead.value != '0'){
                                // read block
                                BLK blkRead;
                                pos = start + (mbrDisk.mbr_partition[i].block_size*sizeof(BM)) + j*sizeof(BLK);
                                fseek(disk, pos, SEEK_SET);
                                fread(&blkRead,sizeof(BLK),1,disk);
                                char numStr[10];
                                memset(&numStr,'\0',sizeof(numStr));
                                sprintf(numStr, "%d", j);
                                // struct1 [label="<f0> left|<f1> mid&#92; dle|<f2> right"];
                                strcat(graf, "  struct");
                                strcat(graf,numStr);
                                strcat(graf," [label=\"<f0> bloque");
                                strcat(graf,numStr);
                                strcat(graf,"| <f1> ");
                                strcat(graf,blkRead.name);
                                strcat(graf, "|<f2> ");
                                strcat(graf,blkRead.data);
                                strcat(graf,"\"];\n");
                                }
                            }
                            strcat(graf,"}");
                            fclose(disk);
                            //printf("%s",graf);
                            // echo "digraph G {Hello->World}" | dot -Tpng >hello.png
                            FILE* disk = fopen("reporte1.dot", "wb+");
                            fseek(disk, 0, SEEK_SET);
                            fwrite(&graf,sizeof(graf),1,disk);
                            fclose(disk);
                            char* comando = (char*)malloc(500);
                            memset(&comando[0], 0, sizeof(comando));
                            strcat(comando, "dot -Tpng reporte1.dot -oreporte1.png");
                            printf("%s\n",comando);
                            system(comando);
                            free(comando);
                    }
                }
                printf("\n    - Reporte BLOCK terminado. \n");
                }
                else{
                printf("    - Particion no existe.\n\n");
                return 0;
                }
                }
                else if(strcasecmp(rm.name, "MBR") == 0){
                int n = valExistu(data,rm.id);
                if(n != -1){
                PD aux = data[n];
                FILE* disk = fopen(aux.path, "rb+");
                MBR mbrDisk;
                fseek(disk, 0, SEEK_SET);
                fread(&mbrDisk,sizeof(MBR),1,disk);
                fclose(disk);
                printf("ID Disco: %d \n",mbrDisk.mbr_disk_signature);
                printf("Fecha De Creacion: %s \n",mbrDisk.mbr_creation_time);
                printf("Tam. Del Disco: %s \n",mbrDisk.mbr_tam);
                printf("\n");
                printf("\n    - Reporte MBR terminado. \n");
                }
                else{
                printf("    - Particion no existe.\n\n");
                return 0;
                }
                }
                else if(strcasecmp(rm.name, "bm_arbdir") == 0){
                int n = valExistu(data,rm.id);
                if(n != -1){
                PD aux = data[n];
                FILE* disk = fopen(aux.path, "rb+");
                MBR mbrDisk;
                fseek(disk, 0, SEEK_SET);
                fread(&mbrDisk,sizeof(MBR),1,disk);
                fclose(disk);
                printf("ID Disco: %d \n",mbrDisk.mbr_disk_signature);
                printf("Fecha De Creacion: %s \n",mbrDisk.mbr_creation_time);
                printf("\n");
                for(int i=0;i<4;i++){
                if(strcasecmp(mbrDisk.mbr_partition[i].part_name,aux.name) == 0){
                            int start = mbrDisk.mbr_partition[i].part_start;
                            disk = fopen(aux.path, "rb+");
                            fseek(disk, start, SEEK_SET);
                            SB bootInfo;
                            fread(&bootInfo,sizeof(SB),1,disk);
                            int corte = sqrt(bootInfo.sb_arbol_virtual_count);
                            for(int j=0;j< bootInfo.sb_arbol_virtual_count;j++){
                                int pos = bootInfo.sb_ap_bitmap_arbol_directorio + (j*sizeof(BM));
                                fseek(disk, pos, SEEK_SET);
                                BM bmRead;
                                fread(&bmRead,sizeof(BM),1,disk);
                                printf("%c  ",bmRead.value);
                                if(((j+1) % corte) == 0 && (j+1)>1){
                                printf("\n");
                                }
                            }
                            fclose(disk);
                    }
                }
                printf("\n\n    - Reporte MBR terminado. \n");
                }
                else{
                printf("    - Particion no existe.\n\n");
                return 0;
                }
                }
                // rep &id->vdn1 &Path->"/home/user/reports/reporte 3.txt" &name->BLOCK
    printf("\n");
    return 0;
}
