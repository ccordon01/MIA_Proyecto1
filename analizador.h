#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "mkdisk.h"
#define MAX 2

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
    }
}

int analyzer(char str[])
{
    char ch, buffer[15];
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
    return 0;
}
