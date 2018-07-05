#include <stdio.h>
#include <stdlib.h>

typedef struct mbr_partition{
    int part_status;
    char part_type[4];
    char part_fit[4];
    int part_start;
    int part_end;
    int part_size;
    int block_size;
    char part_name[16];
} MBR_P;

typedef struct master_boot_record{
    int mbr_tam;
    char mbr_creation_time[128];
    int mbr_disk_signature;
    MBR_P mbr_partition[4];
} MBR;

typedef struct ebr_partition{
    int part_status;
    char part_fit[4];
    int part_start;
    int part_end;
    int part_size;
    int part_next;
    char part_name[16];
} EBR_P;

typedef struct block{
    char name[15];
    char data[50];
} BLK;

typedef struct bitmap{
    char value;
} BM;

typedef struct part_data{
    char id[15];
    char name[15];
    char path[80];
    char letter;
} PD;

typedef struct super_boot{
    char sb_nombre_hd[16];
    int sb_arbol_virtual_count;
    int sb_detalle_directorio_count;
    int sb_inodos_count;
    int sb_bloques_count;
    int sb_arbol_virtual_free;
    int sb_detalle_directorio_free;
    int sb_inodos_free;
    int sb_bloques_free;
    char sb_date_creacion[128];
    char sb_date_ultimo_montaje[128];
    int sb_montajes_count;
    int sb_ap_bitmap_arbol_directorio;
    int sb_ap_arbol_directorio;
    int sb_ap_bitmap_detalle_directorio;
    int sb_ap_detalle_directorio;
    int sb_ap_bitmap_tabla_inodo;
    int sb_ap_tabla_inodo;
    int sb_ap_bitmap_bloques;
    int sb_ap_bloques;
    int sb_ap_log;
    int sb_size_struct_arbol_directorio;
    int sb_size_struct_detalle_directorio;
    int sb_size_struct_inodo;
    int sb_size_struct_bloque;
    int sb_first_free_bit_arbol_directorio;
    int sb_first_free_bit_detalle_directorio;
    int sb_first_free_bit_tabla_inodo;
    int sb_first_free_bit_bloques;
    int sb_magic_num;
} SB;

typedef struct bitmap_arbol_virutal_directorio{
    char value;
} BMAVD;

typedef struct arbol_virutal_directorio{
    char avd_fecha_creacion[128];
    char avd_nombre_directorio[16];
    int avd_ap_array_subdirectorios[6];
    int avd_ap_detalle_directorio;
    int avd_ap_arbol_virtual_directorio;
    int avd_proper;
} AVD;

typedef struct bitmap_detalle_directorio{
    char value;
} BMDD;

typedef struct detalle_directorio_file{
    char dd_file_nombre[16];
    int dd_file_ap_inodo;
    char dd_file_date_creacion[128];
    char dd_file_date_modificacion[128];
} DDF;

typedef struct detalle_directorio{
    DDF dd_array_files[5];
    int dd_ap_detalle_directorio;
} DD;

typedef struct bitmap_tabla_inodo{
    char value;
} BMTI;

typedef struct tabla_inodo{
    int i_count_inodo;
    int i_size_archivo;
    int i_count_bloques_asignados;
    int i_array_bloques;
    int i_ap_indirecto;
    int I_id_proper;
} TI;

typedef struct bitmap_bloque_datos{
    char value;
} BMBD;

typedef struct bloque_datos{
    char db_data[25];
} BD;

typedef struct log{
    int log_tipo_operacion;
    int log_tipo;
    char log_nombre[16];
    char log_path[80];
    char log_contenido[500];
    char log_fecha[128];
} BL;
