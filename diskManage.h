#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include <dirent.h>
#include <errno.h>

int bitmapFree(char diskPath[], int start, int blockSize){
    FILE* disk = fopen(diskPath, "rb+");
    for(int i = 0; i < blockSize; i++){
        int pos = start + (i*sizeof(BM));
        fseek(disk, pos, SEEK_SET);
        BM bmRead;
        fread(&bmRead,sizeof(BM),1,disk);
        if(bmRead.value == '0'){
        fclose(disk);
        return i;
        }
    }
    return -1;
}

void bitmapWrite(char diskPath[], int start, int bitPos, char val){
    BM bmWrite;
    bmWrite.value = val;
    FILE* disk = fopen(diskPath, "rb+");
    int pos = start + (bitPos*sizeof(BM));
    fseek(disk, pos, SEEK_SET);
    fwrite(&bmWrite,sizeof(BM),1,disk);
    fclose(disk);
}

void AVDCreate(char diskPath[], int start, int avdPos, int ddPos, char folderName[]){
    // Creando Folder
    AVD folder;
    memset(&folder.avd_fecha_creacion,'\0',sizeof(folder.avd_fecha_creacion));
    memset(&folder.avd_nombre_directorio,'\0',sizeof(folder.avd_nombre_directorio));
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    strftime(folder.avd_fecha_creacion,128,"%d/%m/%y %H:%M:%S",tlocal);
    for(int i=0; i<6; i++){
        folder.avd_ap_array_subdirectorios[i] = 0;
    }
    folder.avd_ap_detalle_directorio = ddPos;
    folder.avd_ap_arbol_virtual_directorio = 0;
    folder.avd_proper = 201504427;
    folder.avd_ap_detalle_directorio = ddPos;
    int pos = start + (avdPos*sizeof(AVD));
    FILE* disk = fopen(diskPath, "rb+");
    fseek(disk, pos, SEEK_SET);
    fwrite(&folder,sizeof(AVD),1,disk);
    fclose(disk);
}

void DDCreate(char diskPath[], int start, int ddPos){
    DD detalleFolder;
    detalleFolder.dd_ap_detalle_directorio = 0;
    for(int i=0; i<5; i++){
        DDF detalleFile;
        memset(&detalleFile.dd_file_nombre,'\0',sizeof(detalleFile.dd_file_nombre));
        detalleFolder.dd_array_files[i] = detalleFile;
    }
    int pos = start + (ddPos*sizeof(DDF));
    FILE* disk = fopen(diskPath, "rb+");
    fseek(disk, pos, SEEK_SET);
    fwrite(&detalleFolder,sizeof(DDF),1,disk);
    fclose(disk);
}
