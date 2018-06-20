#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"
#include "mount.h"
#include "unmount.h"
#include "mkfile.h"
#include "rmfile.h"
#include "rep.h"
#define MAX 8

typedef struct command
{
    int id;
    char name[15];
} cmd;

struct command keywords[MAX];

void load()
{
    char ch, buffer[80];
    memset(&buffer,'\0',sizeof(buffer));
    FILE *fp;
    int j=0, line=0;
    fp = fopen("command.txt","r");

    if(fp == NULL)
    {
        printf("error while opening the file\n");
        exit(0);
    }

    while((ch = fgetc(fp)) != EOF)
    {
        if(isalnum(ch))
        {
            buffer[j++] = ch;
        }
        else if((ch == '\n') && (j != 0))
        {
            buffer[j] = '\0';
            j = 0;
            int id = 0;
            char name[15]="";
            memset(&name,'\0',sizeof(name));
            sscanf( buffer, "%u %s",&id, name);
            cmd mkd;
            mkd.id = id;
            memset(&mkd.name,'\0',sizeof(mkd.name));
            strcat(mkd.name, name);
            keywords[line] = mkd;
            line++;
            memset(&buffer,'\0',sizeof(buffer));
        }

    }
    fclose(fp);
    FILE *f1;
    f1 = fopen ("partition_list.txt", "wb+");
    if (f1 == NULL)
    {
        perror("    - No se puede abrir partition_list.txt");
        return -1;
    }
    PD part_list[100];
    PD part_val;
    memset(&part_val.id,'\0',sizeof(part_val.id));
    memset(&part_val.name,'\0',sizeof(part_val.name));
    strcat(part_val.name,"");
    strcat(part_val.id,"");
    for(int i=0;i<100;i++){
    part_list[i] = part_val;
    }
    fseek(f1, 0, SEEK_SET);
    fwrite(&part_list,sizeof(part_list),1,f1);
    fclose(f1);
}

int isKeyword(char buffer[])
{
    int i;
    for(i = 0; i < MAX; ++i)
    {
        if(strcasecmp(keywords[i].name, buffer) == 0)
        {
            return keywords[i].id;
        }
    }
    return 0;
}


void commandAnalyzer(int state, char str[], int start)
{
    switch(state)
    {
    case 1:
        mkContructor(state,str,start);
        break;
    case 2:
        rmContructor(state,str,start);
    case 3:
        fContructor(state,str,start);
    break;
    case 4:
        mContructor(state,str,start);
    break;
    case 5:
        umContructor(state,str,start);
    break;
    case 6:
        mkfContructor(state,str,start);
    break;
    case 7:
        rmfContructor(state,str,start);
    break;
    case 8:
        repContructor(state,str, start);
        break;
    }
}

int analyzer(char str[])
{
    char ch, buffer[150];
    memset(&buffer,'\0',sizeof(buffer));
    int i=0,j=0;
    while((ch = str[i++]) != '\0')
    {
        if(isalnum(ch))
        {
            buffer[j++] = ch;
        }

        else if((ch == ' ' || ch == '\n') && (j != 0))
        {
            buffer[j] = '\0';
            j = 0;
            int state = isKeyword(buffer);
            if(state != 0)
            {
                // printf("%s is keyword\n", buffer);
                commandAnalyzer(state, str, i);
            }
            else
                printf("%s No es un comando valido!\n", buffer);
            return 0;
        }
    }
    int state = isKeyword(buffer);
            if(state != 0)
            {
                // printf("%s is keyword\n", buffer);
                commandAnalyzer(state, str, -1);
            }
            else
                printf("%s No es un comando valido!\n", buffer);
            return 0;
}
